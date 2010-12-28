/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2008 Valroft <http://www.aspiredev.org/>
 * Copyright (C) 2010-2011 Megax <http://www.megaxx.info/>
 * 
 * Schumix is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Schumix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Schumix.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../StdAfx.h"

initialiseSingleton(IRCSession);

IRCSession::IRCSession(string host, uint32 port)
{
	m_Host = host;
	m_Port = port;
	m_Socket = SocketPointer(new Socket());

	if(!m_Socket->Connect(host, port))
	{
		Log.Error("IRCSession", "Kapcsolodas ide: %s sikertelen.", host.c_str());
		Sleep(5000);
		return;
	}
	else
		Log.Success("IRCSession", "Kapcsolodas ide: %s sikeres.", host.c_str());

	RehashConfig();
	printf("\n");

	m_NickTarolo = m_NickName[0];

	DIR* pDir = opendir(m_LogHelye.c_str());
	if(pDir == NULL)
#if PLATFORM == PLATFORM_WINDOWS
		CreateDirectoryA(m_LogHelye.c_str(), 0);
#else
		mkdir(m_LogHelye.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
	closedir(pDir);

	m_ConnState = CONN_CONNECTED;
	m_running = true;
	AutoMode = false;

	//mHasFullMotd = false;
	m_LastPing = getMSTime();

	// Populate the giant IRCSession handler table
	InitHandler();

	Log.Debug("IRCSession", "Commands indul...");
	m_Commands = CommandsPointer(new Commands());

	Log.Debug("IRCSession", "Reconnect Thread indul...");
	Thread t(&RunUpdateProc, this);

	printf("\n");
	Update();
}

void IRCSession::RehashConfig()
{
	m_LogHelye = Config.MainConfig.GetStringDefault("Log", "Loghelye", "szoba");

	// Irc Config
	m_NickName[0] = Config.MainConfig.GetStringDefault("User", "Nick", "Test");
	m_NickName[1] = Config.MainConfig.GetStringDefault("User", "Nick2", "Test2");
	m_NickName[2] = Config.MainConfig.GetStringDefault("User", "Nick3", "Test3");
	m_UserName = Config.MainConfig.GetStringDefault("User", "Username", "Test");

	m_UseNickServ = Config.MainConfig.GetBoolDefault("NickServ", "Enable", false);
	m_NickServPassword = Config.MainConfig.GetStringDefault("NickServ", "Password", "");
	m_UseHostServ = Config.MainConfig.GetBoolDefault("HostServ", "Enable", false);
	m_vhost = Config.MainConfig.GetBoolDefault("HostServ", "Vhost", false);
	m_ParancsElojel = Config.MainConfig.GetStringDefault("Parancs", "Elojel", "&");
	m_Uzemelteto = Config.MainConfig.GetStringDefault("Parancs", "Uzemelteto", "Schumix");

	QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT szoba, jelszo FROM channel");
	if(db)
	{
		do 
		{
			string szoba = db->Fetch()[0].GetString();
			string jelszo = db->Fetch()[1].GetString();

			m_ChannelLista.insert(make_pair(szoba, jelszo));
		} while(db->NextRow());

		Log.Success("Config", "Config adatbazis betoltve.");
	}
	else
		Log.Error("Config", "Lekerdezesi hiba! Betoltes sikertelen!");
}

IRCSession::~IRCSession()
{
#ifdef _DEBUG_MOD
	Log.Notice("IRCSession", "~IRCSession()");
#endif
}

void IRCSession::InitHandler()
{
	RegisterHandler(RPL_SUCCESSFUL_AUTH,    cast_default(IRCCallback, &IRCSession::HandleSuccessfulAuth));
	RegisterHandler(RPL_MOTDSTART,          cast_default(IRCCallback, &IRCSession::HandleMotdStart));
	RegisterHandler(RPL_MOTD,               cast_default(IRCCallback, &IRCSession::HandleMotd));
	RegisterHandler(RPL_ENDOFMOTD,          cast_default(IRCCallback, &IRCSession::HandleMotdStop));
	RegisterHandler(RPL_NOTICE,             cast_default(IRCCallback, &IRCSession::HandleNotice));
	RegisterHandler(RPL_PRIVMSG,            cast_default(IRCCallback, &IRCSession::HandlePrivmsg));
	RegisterHandler(RPL_PING,               cast_default(IRCCallback, &IRCSession::HandlePing));
	RegisterHandler(RPL_PONG,               cast_default(IRCCallback, &IRCSession::HandlePong));
	RegisterHandler(RPL_NICK_ERROR,         cast_default(IRCCallback, &IRCSession::HandleNickError));
	RegisterHandler(RPL_WHOIS,              cast_default(IRCCallback, &IRCSession::HandleWhois));
	RegisterHandler(RPL_KICK,               cast_default(IRCCallback, &IRCSession::HandleKick));
	RegisterHandler(RPL_MODE,               cast_default(IRCCallback, &IRCSession::HandleMode));
	RegisterHandler(RPL_JOIN,               cast_default(IRCCallback, &IRCSession::HandleJoin));
	RegisterHandler(RPL_LEFT,               cast_default(IRCCallback, &IRCSession::HandleLeft));
	RegisterHandler(RPL_QUIT,               cast_default(IRCCallback, &IRCSession::HandleQuit));
	RegisterHandler(RPL_404,                cast_default(IRCCallback, &IRCSession::HandleReJoin));
	RegisterHandler(RPL_NICK,               cast_default(IRCCallback, &IRCSession::HandleNick));
	RegisterHandler(RPL_NoChannel_jelszo,   cast_default(IRCCallback, &IRCSession::HandleNoChannelJelszo));
	RegisterHandler(RPL_Channel_ban,        cast_default(IRCCallback, &IRCSession::HandleChannelBan));

	Log.Notice("IRCSession", "Osszes IRC handler regisztralva.");
}

void IRCSession::RegisterHandler(string code, IRCCallback method)
{
	MessageHandlerMap.insert(make_pair(code, method));
}

void IRCSession::BejovoInfo(string SInfo)
{
	IRCMessage mess;

	char* hostmask = new char[256];
	char* opcode = new char[256];
	char* channel = new char[256];
	char* args = new char[4096];

	// Kizárja a null adatot
	memset(hostmask, '\0', 255);
	memset(opcode, '\0', 255);
	memset(channel, '\0', 255);
	memset(args, '\0', 4095);

	uint8 r = sscanf(SInfo.c_str(), ":%255s %255s %255s :%4095[^\r\n]", hostmask, opcode, channel, args);
	if(r != 4)
		r = sscanf(SInfo.c_str(), ":%s %s %s", hostmask, opcode, args); // Parsing failed, let's go to the fallback method. :P

	mess.Minden = SInfo;
	mess.Hostmask = string(hostmask);
	mess.Opcode = string(opcode);
	mess.Args = string(args);
	mess.Channel = string(channel);

	delete[] hostmask;
	delete[] opcode;
	delete[] channel;
	delete[] args;

	// A hostmask részeit darabolja fel.
	uint32 pos = cast_uint32(mess.Hostmask.find('!'));
	mess.Nick = mess.Hostmask.substr(0, pos);
	mess.Host = mess.Hostmask.substr(pos + 1);

	pos = cast_uint32(mess.Host.find('@'));
	mess.User = mess.Host.substr(0, pos);
	mess.Host = mess.Host.substr(pos + 1);

	map<string, IRCCallback>::iterator itr = MessageHandlerMap.find(mess.Opcode);
	if(itr == MessageHandlerMap.end())
	{
		if(sConsole.GetConsoleLog())
		{
			// Információ melyik opcode fut le ha nincs pl használva.
			Log.Notice("IRCSession", "Received unhandled opcode: %s", mess.GetOpcode());
		}
		return;
	}

	// Pass this on to the correct handler.
	IRCCallback cb = itr->second;
	(this->*cb)(mess);
}

void IRCSession::Update()
{
	Log.Notice("IRCSession", "Komunikacio az irc szerverrel megindult.");

	while(Running())
	{
		if(!Running())
			break;

		if(m_ConnState == CONN_CONNECTED)
		{
			WriteLine("NICK %s", m_NickTarolo.c_str());
			WriteLine("USER %s 8 * :%s", m_UserName.c_str(), m_UserName.c_str());
			m_ConnState = CONN_REGISTERING;
		}

		if(m_Socket->HasLine())
		{
			string info = m_Socket->GetLine();
			BejovoInfo(info);
		}

		// ~ 15 másodpercenként pinggel a szerver felé
		if(getMSTime() - m_LastPing >= 15000)
		{
			WriteLine("PING :%s", m_Host.c_str());
			m_LastPing = getMSTime();
		}

		m_Socket->UpdateQueue();

		Sleep(100);
	}
}

void IRCSession::ReConnect()
{
	Log.Success("IRCSession", "Reconnect Thread elindult.");

	while(Running())
	{
		if(!Running())
			break;


		if(FSelect(RECONNECT) == bekapcsol)
		{
			if(!m_Socket->IsConnected())
			{
				if(!m_Socket->Connect(m_Host, m_Port))
				{
					Log.Error("IRCSession", "Ujrakapcsolodas sikertelen ide: %s", m_Host.c_str());
					SocketDisconnect();
				}


				else
				{
					Log.Success("IRCSession", "Ujrakapcsolodva ide: %s", m_Host.c_str());
					m_ConnState = CONN_CONNECTED;

					if(m_ConnState == CONN_CONNECTED)
					{
						Sleep(1000);
						WriteLine("NICK %s", m_NickTarolo.c_str());
						WriteLine("USER %s 8 * :%s", m_UserName.c_str(), m_UserName.c_str());
						Log.Notice("IRCSession", "Nick %s", m_NickTarolo.c_str());
						printf("\n");
						Log.Notice("IRCSession", "Komunikacio az irc szerverrel megindult.");
					}

					if(m_Socket->HasLine())
					{
						string info = m_Socket->GetLine();
						BejovoInfo(info);
					}

					m_Socket->UpdateQueue();
				}
			}

			Sleep(30*1000);
		}
		else
			Sleep(100);
	}

	ThreadExit(0);
}

void IRCSession::SendChatMessage(MessageType type, const char* target, const char* format, ...)
{
	m_mutex.Acquire();
	char* obuf = new char[65536];
	va_list ap;

	va_start(ap, format);
	vsnprintf(obuf, 65536, format, ap);
	va_end(ap);

	if(*obuf == '\0')
	{
		delete[] obuf;
		return;
	}

	string oss = "";
	if(type == PRIVMSG)
		oss += "PRIVMSG";
	else if(type == NOTICE)
		oss += "NOTICE";

	oss = oss + " " + target + " :" + obuf + NEWLINE;
	delete[] obuf;
	m_mutex.Release();

	WriteLine(oss.c_str());
}

void IRCSession::WriteLine(const char* format, ...)
{
	m_mutex.Acquire();
	char* obuf = new char[65536];
	va_list ap;

	va_start(ap, format);
	vsnprintf(obuf, 65536, format, ap);
	va_end(ap);

	if(*obuf == '\0')
	{
		delete[] obuf;
		return;
	}

	string send = string(obuf) + NEWLINE;
	delete[] obuf;

	m_Socket->SendLine(send);
	m_mutex.Release();
}

void IRCSession::WriteLineForce(const char* format, ...)
{
	m_mutex.Acquire();
	char* obuf = new char[65536];
	va_list ap;

	va_start(ap, format);
	vsnprintf(obuf, 65536, format, ap);
	va_end(ap);

	if(*obuf == '\0')
	{
		delete[] obuf;
		return;
	}

	string send = string(obuf) + NEWLINE;
	delete[] obuf;

	m_Socket->SendForcedLine(send);
	m_mutex.Release();
}

int IRCSession::writer(char* data, size_t size, size_t nmemb, string *buffer)
{
	int result = NULL;

	if(buffer != NULL)
	{
		buffer->append(data, size* nmemb);
		result = size* nmemb;
	}
	return result;
}

string IRCSession::RandomString(int length, bool letters, bool numbers, bool symbols)
{
	string str, allPossible;

	if(letters)
	{
		for(int i = 65; i <= 90; i++)
		{
			allPossible += cast_char(i);
			allPossible += cast_char(i+32);
		}
	}

	if(numbers)
	{
		for(int i = 48; i <= 57; i++)
			allPossible += cast_char(i);
	}

	if(symbols)
	{
		for(int i = 33; i <= 47; i++)
			allPossible += cast_char(i);

		for(int i = 58; i <= 64; i++)
			allPossible += cast_char(i);

		for(int i = 91; i <= 96; i++)
			allPossible += cast_char(i);

		for(int i = 123; i <= 126; i++)
			allPossible += cast_char(i);
	}

	int numberOfPossibilities = allPossible.length();
	for(int i = 0; i < length; i++)
		str += allPossible[rand() % numberOfPossibilities];

	return str;
}

void IRCSession::SocketDisconnect()
{
	m_Socket->Disconnect();
}

void IRCSession::Leallas()
{
	m_running = false;
	SocketDisconnect();

	Log.Notice("IRCSession", "IRCSession leallt.");
}

Thread_void IRCSession::RunUpdateProc(void* smg)
{
	cast_default(IRCSession*, smg)->ReConnect();
	return NULL;
}

void IRCSession::CNick(IRCMessage& recvData)
{
	if(cast_int(recvData.Channel.find("#")) == string::npos)
		recvData.Channel = recvData.Nick;
}
