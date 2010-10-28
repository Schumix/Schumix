/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2008 Valroft <http://www.aspiredev.org/>
 * Copyright (C) 2010 Megax <http://www.megaxx.info/>
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

#include "StdAfx.h"

initialiseSingleton(IRCSession);
MessageHandlerMap IRCMessageHandlerMap;

IRCSession::IRCSession(string host, uint32 port, string sqlhost, string user, string pass, string database)
{
	m_Host = host;
	m_Port = port;
	m_Socket = SimpleSocketPointer(new SimpleSocket());

	if(!m_Socket->Connect(host, port))
	{
		Log.Error("IRCSession", "Kapcsolodas ide: %s sikertelen.", host.c_str());
		exit(1);
		return;
	}
	else
		Log.Success("IRCSession", "Kapcsolodas ide: %s sikeres.", host.c_str());

	RehashConfig(sqlhost, user, pass, database);
	printf("\n");

	m_NickTarolo = m_NickName[0];

	DIR* pDir = opendir(LogHelye);
	if(pDir == NULL)
#if PLATFORM == PLATFORM_WINDOWS
		CreateDirectoryA(LogHelye, 0);
#else
		mkdir(LogHelye, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

	m_ConnState = CONN_CONNECTED;
	m_running = true;

	//mHasFullMotd = false;
	m_LastPing = getMSTime();

	// Populate the giant IRCSession handler table
	ADD_CODE( RPL_SUCCESSFUL_AUTH,	&IRCSession::HandleSuccessfulAuth )
	ADD_CODE( RPL_MOTDSTART,		&IRCSession::HandleMotdStart )
	ADD_CODE( RPL_MOTD,				&IRCSession::HandleMotd )
	ADD_CODE( RPL_ENDOFMOTD,		&IRCSession::HandleMotdStop )
	ADD_CODE( RPL_NOTICE,			&IRCSession::HandleNotice )
	ADD_CODE( RPL_PRIVMSG,			&IRCSession::HandlePrivmsg )
	ADD_CODE( RPL_PING,				&IRCSession::HandlePing )
	ADD_CODE( RPL_PONG,				&IRCSession::HandlePong )
	ADD_CODE( RPL_NICK_ERROR,		&IRCSession::HandleNickError )
	ADD_CODE( RPL_319,				&IRCSession::HandleWhois )
	ADD_CODE( RPL_KICK,				&IRCSession::HandleKick )
	ADD_CODE( RPL_MODE,				&IRCSession::HandleMode )
	ADD_CODE( RPL_JOIN,				&IRCSession::HandleJoin )
	ADD_CODE( RPL_LEFT,				&IRCSession::HandleLeft )
	ADD_CODE( RPL_QUIT,				&IRCSession::HandleQuit )
	ADD_CODE( RPL_404,				&IRCSession::HandleReJoin )
	ADD_CODE( RPL_NICK,				&IRCSession::HandleNick )
	ADD_CODE( RPL_NoChannel_jelszo,	&IRCSession::HandleNoChannelJelszo )
	ADD_CODE( RPL_Channel_ban,		&IRCSession::HandleChannelBan )

	Log.Debug("IRCSession", "Reconnect Thread indul...");
	Thread t(&RunUpdateProc, this);

	printf("\n");
	Update();
}

void IRCSession::RehashConfig(string host, string user, string pass, string database)
{
	// Mysql Config
	_mysql[0] = host;
	_mysql[1] = user;
	_mysql[2] = pass;
	_mysql[3] = database;

	m_SQLConn = MySQLConnectionPointer(new MySQLConnection(_mysql[0], _mysql[1], _mysql[2]));
	m_SQLConn->UseDatabase(_mysql[3]);

	if(!m_SQLConn->GetSqlError())
		Log.Notice("IRCSession", "Mysql adatbazishoz sikeres a kapcsolodas.");
	else
		Log.Error("IRCSession", "Mysql adatbazishoz sikertelen a kapcsolodas.");

	// Irc Config
	m_NickName[0] = Config.MainConfig.GetStringDefault("User", "Nick", "Test");
	m_NickName[1] = Config.MainConfig.GetStringDefault("User", "Nick2", "Test2");
	m_NickName[2] = Config.MainConfig.GetStringDefault("User", "Nick3", "Test3");
	m_UserName = Config.MainConfig.GetStringDefault("User", "Username", "Test");
	m_HostName = Config.MainConfig.GetStringDefault("User", "Host", "schumix");
	m_ServerName = m_HostName;

	m_UseNickServ = Config.MainConfig.GetBoolDefault("NickServ", "Enable", false);
	m_NickServPassword = Config.MainConfig.GetStringDefault("NickServ", "Password", "");
	m_ParancsElojel = Config.MainConfig.GetStringDefault("Parancs", "Elojel", "&");

	QueryResultPointer db = m_SQLConn->Query("SELECT szoba, jelszo FROM channel");
	if(db)
	{
		do 
		{
			string szoba = db->Fetch()[0].GetString();
			string jelszo = db->Fetch()[1].GetString();

			m_ChannelLista.insert(make_pair(szoba, jelszo));
		} while(db->NextRow());
	}

	Log.Success("Config", "Config adatbazis betoltve.");
}

IRCSession::~IRCSession()
{
#ifdef _DEBUG_MOD
	Log.Notice("IRCSession", "~IRCSession()");
#endif

	if(m_Socket)
		SocketDisconnect();
}

void IRCSession::BejovoInfo(string SInfo)
{
	IRCMessage mess;

	char* hostmask = new char[256];
	char* opcode = new char[256];
	char* target = new char[256];
	char* args = new char[4096];

	// Kizárja a null adatatot
	memset(hostmask, '\0', 255);
	memset(opcode, '\0', 255);
	memset(target, '\0', 255);
	memset(args, '\0', 4095);

	uint8 r = sscanf(SInfo.c_str(), ":%255s %255s %255s :%4095[^\r\n]", hostmask, opcode, target, args);
	if(r != 4)
		r = sscanf(SInfo.c_str(), ":%s %s %s", hostmask, opcode, args); // Parsing failed, let's go to the fallback method. :P

	mess.minden = SInfo;
	mess.hostmask = string(hostmask);
	mess.opcode = string(opcode);
	mess.args = string(args);
	mess.target = string(target);

	delete[] hostmask;
	delete[] opcode;
	delete[] target;
	delete[] args;

	// A hostmask részeit darabolja fel.
	uint32 pos = cast_uint32(mess.hostmask.find('!'));
	mess.source_nick = mess.hostmask.substr(0, pos);
	mess.source_host = mess.hostmask.substr(pos + 1);

	pos = cast_uint32(mess.source_host.find('@'));
	mess.source_user = mess.source_host.substr(0, pos);
	mess.source_host = mess.source_host.substr(pos + 1);

	MessageHandlerMap::iterator itr = IRCMessageHandlerMap.find(mess.opcode);
	if(itr == IRCMessageHandlerMap.end())
	{
		if(sConsole.GetConsolLog() == bekapcsol)
		{
			// Információ melyik opcode fut le ha nincs pl használva.
			Log.Notice("IRCSession", "Received unhandled opcode: %s", mess.opcode.c_str());
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

		// ~ 15 másodpercenként pinegel a szerver felé
		if(getMSTime() - m_LastPing >= 15000)
		{
			WriteLine("PING :%s", m_Host.c_str());
			m_LastPing = getMSTime();
		}

		m_Socket->UpdateQueue();

		Sleep(20);
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

			Sleep(10*1000);
		}
		else
			Sleep(100);
	}

	ThreadExit(0);
}

void IRCSession::SendChatMessage(MessageType type, const char * target, const char * format, ...)
{
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
	WriteLine(oss.c_str());
}

void IRCSession::WriteLine(const char * format, ...)
{
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
}

void IRCSession::WriteLineForce(const char * format, ...)
{
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

bool IRCSession::Admin(string nick)
{
	string admin;
	transform(nick.begin(), nick.end(), nick.begin(), ::tolower);

	QueryResultPointer db = m_SQLConn->Query("SELECT nev FROM adminok WHERE nev = '%s'", nick.c_str());
	if(db)
		admin = db->Fetch()[0].GetString();

	if(nick != admin)
		return false;

	return true;
}

bool IRCSession::Admin(string nick, AdminFlag Flag)
{
	string admin;
	int flag;
	transform(nick.begin(), nick.end(), nick.begin(), ::tolower);

	QueryResultPointer db = m_SQLConn->Query("SELECT nev, flag FROM adminok WHERE nev = '%s'", nick.c_str());
	if(db)
	{
		admin = db->Fetch()[0].GetString();
		flag = cast_int(db->Fetch()[1].GetUInt8());
	}

	if(nick != admin)
		return false;

	if(Flag != flag)
		return false;

	return true;
}

bool IRCSession::Admin(string nick, string nick_ip, AdminFlag Flag)
{
	string admin;
	string ip;
	int flag;
	transform(nick.begin(), nick.end(), nick.begin(), ::tolower);

	QueryResultPointer db = m_SQLConn->Query("SELECT nev, ip, flag FROM adminok WHERE nev = '%s'", nick.c_str());
	if(db)
	{
		admin = db->Fetch()[0].GetString();
		ip = db->Fetch()[1].GetString();
		flag = cast_int(db->Fetch()[2].GetUInt8());
	}

	if(nick != admin)
		return false;

	if(nick_ip != ip)
		return false;

	if(flag == 1 && Flag == NULL)
		return true;

	if(Flag != flag)
		return false;

	return true;
}

string IRCSession::randomString(int length, bool letters, bool numbers, bool symbols)
{
	string str;
	string allPossible;

	if(letters == true)
	{
		for(int i = 65; i <= 90; i++)
		{
			allPossible += cast_char(i);
			allPossible += cast_char(i+32);
		}
	}

	if(numbers == true)
	{
		for(int i = 48; i <= 57; i++)
			allPossible += cast_char(i);
	}

	if(symbols == true)
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
