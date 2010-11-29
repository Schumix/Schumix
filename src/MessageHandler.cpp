/*
 * This file is part of Schumix.
 * 
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

void IRCSession::HandleSuccessfulAuth(IRCMessage& recvData)
{
	// We're good to go.
	m_ConnState = CONN_REGISTERED;
	Log.Success("IRCSession", "A komunikacio letrejot.");
	m_ServerRealName = recvData.Hostmask;

	// Nickserv identification
	if(m_UseNickServ)
	{
		Log.Notice("NickServ", "NickServ azonosito kuldese.");
		SendChatMessage(PRIVMSG, "NickServ", "identify %s", m_NickServPassword.c_str());
	}

	if(m_UseHostServ)
	{
		h_Aktivitas = true;
		Log.Notice("HostServ", "HostServ bevan kapcsolva.");
		SendChatMessage(PRIVMSG, "HostServ", "on");
	}
	else
	{
		Log.Notice("HostServ", "HostServ kivan kapcsolva.");
		if(m_vhost)
			SendChatMessage(PRIVMSG, "HostServ", "off");

		// Kitakaritja a channel funkciokat nehogy ütközés legyen.
		m_ChannelFunkcio.clear();
		m_ChannelPrivmsg = m_NickTarolo;

		Log.Debug("IRCSession", "Kapcsolodas a szobakhoz...");
		bool error = false;

		// Join az adatbázisban szereplõ channel-okra. Bármi hiba van a kapcsolodáskor eltárolodik a hiba oka.
		map<string, string>::iterator itr = m_ChannelLista.begin();
		for(; itr != m_ChannelLista.end(); itr++)
		{
			string szoba = itr->first;
			string join = itr->first;
			if(itr->second != "")
				join += " " + itr->second;

			WriteLine("JOIN %s", join.c_str());
			m_SQLConn->Query("UPDATE channel SET aktivitas = '', error = '' WHERE szoba = '%s'", szoba.c_str());

			QueryResultPointer db = m_SQLConn->Query("SELECT funkciok FROM channel WHERE szoba = '%s'", szoba.c_str());
			if(db)
			{
				string funkcio = db->Fetch()[0].GetString();
				vector<string> res(1);
				sVezerlo.split(funkcio, ",", res);
				int resAdat = res.size();

				for(int i = 1; i < resAdat; i++)
				{
					string szobaadat = szoba + "." + res[i];
					m_ChannelFunkcio.push_back(szobaadat);
				}

				res.clear();
			}

			QueryResultPointer db1 = m_SQLConn->Query("SELECT aktivitas FROM channel WHERE szoba = '%s'", szoba.c_str());
			if(db1)
			{
				string aktivitas = db1->Fetch()[0].GetString();

				if(aktivitas != "nem aktiv")
					m_SQLConn->Query("UPDATE channel SET aktivitas = 'aktiv' WHERE szoba = '%s'", szoba.c_str());
				else
					error = true;
			}
		}

		if(!error)
			Log.Success("IRCSession", "Sikeresen kapcsolodva szobakhoz.");
		else
			Log.Warning("IRCSession", "Nehany kapcsolodas sikertelen!");
	}
}

void IRCSession::HandleMotdStart(IRCMessage& recvData)
{
	//Log.Notice("Motd", "MOTD Start");
	// The MOTD is coming 
	//mHasFullMotd = false;
	//mLastMotd = "";
}

void IRCSession::HandleMotd(IRCMessage& recvData)
{
	// Part of the MOTD, append it to the rest.
	//if(mHasFullMotd)
	//	return;

	//mLastMotd += recvData.Args + NEWLINE;
}

void IRCSession::HandleMotdStop(IRCMessage& recvData)
{
	// The MOTD is over.
	Log.Notice("Motd", "Server Message of the Day received.");
	//mHasFullMotd = true;
}

void IRCSession::HandleNotice(IRCMessage& recvData)
{
	if(sConsole.GetConsoleLog() == bekapcsol)
	{
		Log.Color(TRED);
		printf("%s", recvData.GetNick());
		Log.Color(TYELLOW);
		printf(" sends notice: ");
		Log.Color(TNORMAL);
		printf("%s%s", recvData.GetArgs(), NEWLINE);
	}

	if(AutoMode)
	{
		vector<string> res(1);
		sVezerlo.split(recvData.Args, " ", res);
		if(res.size() < 4)
		{
			res.clear();
			return;
		}

		if(res[3] == "3")
		{
			transform(res[2].begin(), res[2].end(), res[2].begin(), ::tolower);
			QueryResultPointer db = m_SQLConn->Query("SELECT rang FROM modelista WHERE nick = '%s' AND channel = '%s'", res[2].c_str(), ModeChannel.c_str());
			if(db)
			{
				string rang = db->Fetch()[0].GetString();
				WriteLine("MODE %s %s %s", ModeChannel.c_str(), rang.c_str(), res[2].c_str());
			}
			else
				WriteLine("MODE %s -aohv %s %s %s %s", ModeChannel.c_str(), res[2].c_str(), res[2].c_str(), res[2].c_str(), res[2].c_str());
		}

		AutoMode = false;
		res.clear();
	}

	if(m_UseHostServ)
	{
		if(cast_int(recvData.Args.find("Your vhost of")) != string::npos && h_Aktivitas)
		{
			// Kitakaritja a channel funkciokat nehogy ütközés legyen.
			m_ChannelFunkcio.clear();
			m_ChannelPrivmsg = m_NickTarolo;

			Log.Debug("IRCSession", "Kapcsolodas a szobakhoz...");
			bool error = false;

			// Join az adatbázisban szereplõ channel-okra. Bármi hiba van a kapcsolodáskor eltárolodik a hiba oka.
			map<string, string>::iterator itr = m_ChannelLista.begin();
			for(; itr != m_ChannelLista.end(); itr++)
			{
				string szoba = itr->first;
				string join = itr->first;
				if(itr->second != "")
					join += " " + itr->second;

				WriteLine("JOIN %s", join.c_str());
				m_SQLConn->Query("UPDATE channel SET aktivitas = '', error = '' WHERE szoba = '%s'", szoba.c_str());

				QueryResultPointer db = m_SQLConn->Query("SELECT funkciok FROM channel WHERE szoba = '%s'", szoba.c_str());
				if(db)
				{
					string funkcio = db->Fetch()[0].GetString();
					vector<string> res(1);
					sVezerlo.split(funkcio, ",", res);
					int resAdat = res.size();

					for(int i = 1; i < resAdat; i++)
					{
						string szobaadat = szoba + "." + res[i];
						m_ChannelFunkcio.push_back(szobaadat);
					}

					res.clear();
				}

				QueryResultPointer db1 = m_SQLConn->Query("SELECT aktivitas FROM channel WHERE szoba = '%s'", szoba.c_str());
				if(db1)
				{
					string aktivitas = db1->Fetch()[0].GetString();

					if(aktivitas != "nem aktiv")
						m_SQLConn->Query("UPDATE channel SET aktivitas = 'aktiv' WHERE szoba = '%s'", szoba.c_str());
					else
						error = true;
				}
			}

			if(!error)
				Log.Success("IRCSession", "Sikeresen kapcsolodva szobakhoz.");
			else
				Log.Warning("IRCSession", "Nehany kapcsolodas sikertelen!");

			h_Aktivitas = false;
		}
	}
}

void IRCSession::HandlePing(IRCMessage& recvData)
{
	// Ping? Pong!
	WriteLine("PONG :%s", recvData.GetArgs());
	//Log.Notice("IRCSession", "Ping? Pong!");
}

void IRCSession::HandlePong(IRCMessage& recvData)
{
	// Ping? Pong!
	WriteLine("PONG :%s", recvData.GetArgs());
	//Log.Notice("IRCSession", "Ping? Pong!");
}

void IRCSession::HandleKick(IRCMessage& recvData)
{
	vector<string> res(1);
	sVezerlo.split(recvData.Minden, " ", res);

	if(res.size() < 5)
	{
		res.clear();
		return;
	}

	if(res[4] == m_NickTarolo)
	{
		if(FSelect(REJOIN) == bekapcsol && FSelectChannel(REJOIN, recvData.Channel) == bekapcsol)
		{
			map<string, string>::iterator itr = m_ChannelLista.begin();
			for(; itr != m_ChannelLista.end(); itr++)
			{
				string join = itr->first;

				if(itr->second != "")
					join += " " + itr->second;

				WriteLine("JOIN %s", join.c_str());
			}
		}
	}
	else
	{
		if(sConsole.GetConsoleLog() == bekapcsol)
		{
			if(res.size() < 6)
			{
				res.clear();
				return;
			}

			string oka;
			int resAdat = res.size();

			for(int i = 5; i < resAdat; i++)
				oka += " " + res[i];

			printf("%s kickelte a kovetkezo felhasznalot: %s oka: %s\n", recvData.GetNick(), res[4].c_str(), oka.substr(2).c_str());
		}
	}

#ifdef _DEBUG_MOD
	Log.Warning("Funkcio", "A %s funkcio nem uzemel!", REJOIN);
#endif

	res.clear();
}

void IRCSession::HandleJoin(IRCMessage& recvData)
{
	if(recvData.Nick == m_NickTarolo)
		return;

	if(AutoKick(recvData, "join"))
		return;

	uint8 szokoz = recvData.Channel.find(':');
	string channel = recvData.Channel.substr(szokoz+1);

	if(FSelect(MODE) == bekapcsol && FSelectChannel(MODE, channel) == bekapcsol)
	{
		AutoMode = true;
		ModeChannel = channel;
		WriteLine("NICKSERV STATUS %s", recvData.GetNick());
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", MODE);
#endif

	if(FSelect(KOSZONES) == bekapcsol && FSelectChannel(KOSZONES, channel) == bekapcsol)
	{
		string Koszones;
		int ido = sVezerlo.Ora();

		switch(rand()%13)
		{
			case 0:
				Koszones = "Hello";
				break;
			case 1:
				Koszones = "Csáó";
				break;
			case 2:
				Koszones = "Hy";
				break;
			case 3:
				Koszones = "Szevasz";
				break;
			case 4:
				Koszones = "Üdv";
				break;
			case 5:
				Koszones = "Szervusz";
				break;
			case 6:
				Koszones = "Aloha";
				break;
			case 7:
				Koszones = "Jó napot";
				break;
			case 8:
				Koszones = "Szia";
				break;
			case 9:
				Koszones = "Hi";
				break;
			case 10:
				Koszones = "Szerbusz";
				break;
			case 11:
				Koszones = "Hali";
				break;
			case 12:
				Koszones = "Szeva";
				break;
		}

		if(ido <= 9)
			SendChatMessage(PRIVMSG, channel.c_str(), "Jó reggelt %s", recvData.GetNick());
		else if(ido >= 20)
			SendChatMessage(PRIVMSG, channel.c_str(), "Jó estét %s", recvData.GetNick());
		else
		{
			if(Admin(recvData.Nick))
				SendChatMessage(PRIVMSG, channel.c_str(), "Üdv fõnök");
			else
				SendChatMessage(PRIVMSG, channel.c_str(), "%s %s", Koszones.c_str(), recvData.GetNick());
		}
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", KOSZONES);
#endif
}

void IRCSession::HandleLeft(IRCMessage& recvData)
{
	if(recvData.Nick == m_NickTarolo)
		return;

	if(FSelect(KOSZONES) == bekapcsol && FSelectChannel(KOSZONES, recvData.Channel) == bekapcsol)
	{
		string elkoszones;

		switch(rand()%2)
		{
			case 0:
				elkoszones = "Viszlát";
				break;
			case 1:
				elkoszones = "Bye";
				break;
		}

		if(sVezerlo.Ora() >= 20)
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "Jóét %s", recvData.GetNick());
		else
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s %s", elkoszones.c_str(), recvData.GetNick());
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", KOSZONES);
#endif
}

void IRCSession::HandleQuit(IRCMessage& recvData)
{
/*	if(FSelect(KOSZONES) == bekapcsol && FSelectChannel(KOSZONES, recvData.Channel) == bekapcsol)
	{
		string elkoszones;

		switch(rand()%2)
		{
			case 0:
				elkoszones = "Viszlát";
				break;
			case 1:
				elkoszones = "Bye";
				break;
		}

		if(sVezerlo.Ora() >= 20)
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "Jóét %s", recvData.GetNick());
		else
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s %s", elkoszones.c_str(), recvData.GetNick());
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", KOSZONES);
#endif*/
}

void IRCSession::HandleReJoin(IRCMessage& recvData)
{
	if(FSelect(REJOIN) == bekapcsol)
	{
		map<string, string>::iterator itr = m_ChannelLista.begin();
		for(; itr != m_ChannelLista.end(); itr++)
		{
			string join1 = itr->first;

			if(FSelectChannel(REJOIN, join1) == bekapcsol)
			{
				string join = itr->first;

				if(itr->second != "")
					join += " " + itr->second;

				WriteLine("JOIN %s", join.c_str());
			}
		}
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", REJOIN);
#endif
}

void IRCSession::HandleMode(IRCMessage& recvData)
{
	if(sConsole.GetConsoleLog() == bekapcsol)
	{
		if(recvData.Channel == m_NickTarolo)
			return;

		vector<string> res(1);
		sVezerlo.split(recvData.Minden, " ", res);
		if(res.size() < 6)
		{
			res.clear();
			return;
		}

		printf("Megvaltoztatta %s rangjat: ", res[5].c_str());
		Log.Color(TBLUE);
		printf("%s. ", recvData.GetNick());
		Log.Color(TNORMAL);
		printf("Helye: ");
		Log.Color(TRED);
		printf("%s\n", recvData.GetChannel());
		Log.Color(TNORMAL);

		res.clear();
	}
}

void IRCSession::HandleNick(IRCMessage& recvData)
{
	if(sConsole.GetConsoleLog() == bekapcsol)
	{
		vector<string> res(1);
		sVezerlo.split(recvData.Args, ":", res);

		Log.Color(TRED);
		printf("%s", recvData.GetNick());
		Log.Color(TWHITE);
		printf(" nev cserelve erre: ");
		Log.Color(TRED);
		printf("%s\n", res[1].c_str());
		Log.Color(TNORMAL);

		res.clear();
	}
}

void IRCSession::HandleNickError(IRCMessage& recvData)
{
	if(m_NickTarolo == m_NickName[0])
	{
		m_NickTarolo = m_NickName[1];
		m_ConnState = CONN_CONNECTED;
		return;
	}
	else if(m_NickTarolo == m_NickName[1])
	{
		m_NickTarolo = m_NickName[2];
		m_ConnState = CONN_CONNECTED;
		return;
	}
	else if(m_NickTarolo == m_NickName[2])
	{
		m_NickTarolo = "_Schumix";
		m_ConnState = CONN_CONNECTED;
		return;
	}
	else if(m_NickTarolo == "_Schumix")
	{
		m_NickTarolo = "__Schumix";
		m_ConnState = CONN_CONNECTED;
		return;
	}
	else if(m_NickTarolo == "__Schumix")
	{
		m_NickTarolo = "_Schumix_";
		m_ConnState = CONN_CONNECTED;
		return;
	}
	else if(m_NickTarolo == "_Schumix_")
	{
		m_NickTarolo = "__Schumix_";
		m_ConnState = CONN_CONNECTED;
		return;
	}
	else if(m_NickTarolo == "__Schumix_")
	{
		m_NickTarolo = "__Schumix__";
		m_ConnState = CONN_CONNECTED;
		return;
	}
	else if(m_NickTarolo == "__Schumix__")
	{
		m_NickTarolo = m_NickName[0];
		m_ConnState = CONN_CONNECTED;
		return;
	}
}

void IRCSession::HandleNoChannelJelszo(IRCMessage& recvData)
{
	vector<string> res(1);
	sVezerlo.split(recvData.Minden, " ", res);
	if(res.size() < 5)
	{
		res.clear();
		return;
	}

	m_SQLConn->Query("UPDATE channel SET aktivitas = 'nem aktiv', error = 'hibas channel jelszo' WHERE szoba = '%s'", res[4].c_str());
	SendChatMessage(PRIVMSG, m_ChannelPrivmsg.c_str(), "%s: hibás channel jelszó", res[4].c_str());
	m_ChannelPrivmsg = m_NickTarolo;

	res.clear();
}

void IRCSession::HandleChannelBan(IRCMessage& recvData)
{
	vector<string> res(1);
	sVezerlo.split(recvData.Minden, " ", res);
	if(res.size() < 5)
	{
		res.clear();
		return;
	}

	m_SQLConn->Query("UPDATE channel SET aktivitas = 'nem aktiv', error = 'channel ban' WHERE szoba = '%s'", res[4].c_str());
	SendChatMessage(PRIVMSG, m_ChannelPrivmsg.c_str(), "%s: channel ban", res[4].c_str());
	m_ChannelPrivmsg = m_NickTarolo;

	res.clear();
}

void IRCSession::HandleWhois(IRCMessage& recvData)
{
	vector<string> res(1);
	sVezerlo.split(recvData.Minden, " ", res);
	if(res.size() < 6)
	{
		res.clear();
		return;
	}

	string alomany;
	int resAdat = res.size();

	for(int i = 5; i < resAdat; i++)
		alomany += " " + res[i];

	SendChatMessage(PRIVMSG, m_WhoisPrivmsg.c_str(), "Jelenleg itt van fent: %s", alomany.substr(2).c_str());
	m_WhoisPrivmsg = m_NickTarolo;

	res.clear();
}
