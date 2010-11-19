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
	m_ServerRealName = recvData.hostmask;

	// Nickserv identification
	if(m_UseNickServ)
	{
		Log.Notice("NickServ", "Sending NickServ identification.");
		SendChatMessage(PRIVMSG, "NickServ", "identify %s", m_NickServPassword.c_str());
	}

	// Kitakaritja a channel funkciokat nehogy ütközés legyen.
	m_ChannelFunkcio.clear();
	m_ChannelPrivmsg = m_NickTarolo;

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
		}
	}
}

void IRCSession::HandleMotdStart(IRCMessage& recvData)
{
	// The MOTD is coming 
	//mHasFullMotd = false;
	//mLastMotd = "";
}

void IRCSession::HandleMotd(IRCMessage& recvData)
{
	// Part of the MOTD, append it to the rest.
	//if(mHasFullMotd)
	//	return;

	//mLastMotd += recvData.args + NEWLINE;
}

void IRCSession::HandleMotdStop(IRCMessage& recvData)
{
	// The MOTD is over.
	//Log.Notice("IRCSession", "Server Message of the Day received.");
	//mHasFullMotd = true;
}

void IRCSession::HandleNotice(IRCMessage& recvData)
{
	if(sConsole.GetConsolLog() == bekapcsol)
	{
		Log.Color(TRED);
		printf("%s", recvData.source_nick.c_str());
		Log.Color(TYELLOW);
		printf(" sends notice: ");
		Log.Color(TNORMAL);
		printf("%s%s", recvData.args.c_str(), NEWLINE);
	}
}

void IRCSession::HandlePrivmsg(IRCMessage& recvData)
{
	if(sConsole.GetConsolLog() == bekapcsol)
	{
		Log.Color(TYELLOW);
		printf("[%s] <%s> %s%s", recvData.target.c_str(), recvData.source_nick.c_str(), recvData.args.c_str(), NEWLINE);
		Log.Color(TNORMAL);
	}

	// Log
	Logfajl(recvData);

	// Hl üzenet
	HLUzenet(recvData);

	if(FSelect(PARANCSOK) == bekapcsol)
	{
		if(FSelectChannel(PARANCSOK, recvData.target) != bekapcsol && cast_int(recvData.target.find("#")) != string::npos)
			return;

		// Schumix
		Schumix(recvData);

		// Parancs elõjel
		if(recvData.args.substr(0, 1) != m_ParancsElojel)
			return;

		firstSpace = recvData.args.find(' ');
		if(firstSpace == NULL)
			firstSpace = recvData.args.length();

		string cmd = recvData.args.substr(1, firstSpace - 1);
		transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

		// parancsok
		if(cmd == "xbot") // id 1
		{
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Verzió: 10%s", revision);
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Parancsok: %sinfo | %shelp | %sxrev | %sido | %sdatum | %sirc | %sroll | %sszam | %skeres | %sfordit | %ssha1 | %smd5 | %suzenet | %swhois | %sjegyzet", m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str());
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Programmed by: 3Csaba");
			return;
		}

		// Admin parancsok
		if(cmd == "admin") // id 2
			Admin(recvData);

		// Hozzáférés az admin parancsokhoz
		if(cmd == "hozzaferes") // id 3
			Hozzaferes(recvData);

		// Admin jelszó cserélõ
		if(cmd == "ujjelszo") // id 4
			Ujjelszo(recvData);

		// Jegyzetek
		if(cmd == "jegyzet") // id 5
			Jegyzet(recvData);

		// Bot help
		if(cmd == "help") // id 6
		{
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Ha egy parancs mögé irod a help-et segít a használatában!");
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Fõ parancsom: %sxbot", m_ParancsElojel.c_str());
			return;
		}

		// Irc szinek
		if(cmd == "szinek") // id 7
		{
			if(!Admin(recvData.source_nick, recvData.source_host, Operator))
				return;

			SendChatMessage(PRIVMSG, recvData.target.c_str(), "1teszt1 2teszt2 3teszt3 4teszt4 5teszt5 6teszt6 7teszt7 8teszt8");
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "9teszt9 10teszt10 11teszt11 12teszt12 13teszt13 14teszt14 15teszt15");
			return;
		}

		// info
		if(cmd == "info") // id 8
		{
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Programozóm: Csaba, Twl és Scymex. Üzemeltetõ: %s.", m_Uzemelteto.c_str());
			return;
		}

		if(cmd == "online") // id 9
		{
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nem üzemel a funkció!");
			return;
		}

		// Helyi idõ
		if(cmd == "ido") // id 10
			Ido(recvData);

		// Datum
		if(cmd == "datum") // id 11
			Datum(recvData);

		// Irc parancsok
		if(cmd == "irc") // id 12
			Irc(recvData);

		// Nick csere
		if(cmd == "nick") // id 13
			Nick(recvData);

		// Join egy channelra
		if(cmd == "join") // id 14
			Join(recvData);

		// Left egy channelról
		if(cmd == "left") // id 15
			Left(recvData);

		// Kirugás
		if(cmd == "kick") // id 16
			Kick(recvData);

		// Rang adása
		if(cmd == "mode") // id 17
			Mode(recvData);

		// revision
		if(cmd == "xrev") // id 18
			Xrev(recvData);

		// Kis wow :D
		if(cmd == "roll") // id 19
		{
			uint32 adat = rand()%101;
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Százalékos aránya %u!", adat);
		}

		// Keresés meik nick meik channelon van fent
		if(cmd == "whois") // id 20
			Whois(recvData);

		// Adminok születésnapja
		if(cmd == "sznap") // id 21
			Sznap(recvData);

		// Simán teszt
		if(cmd == "teszt") // id 22
			Teszt(recvData);

		// Számológép
		if(cmd == "szam") // id 23
			Szam(recvData);

		// Google
		if(cmd == "keres") // id 24
			Keres(recvData);

		// Fordítás
		if(cmd == "fordit") // id 25
			Forditas(recvData);

		// Sha1 kodolás
		if(cmd == "sha1") // id 26
			Sha1(recvData);

		// Md5 kodolás
		if(cmd == "md5") // id 27
			Md5(recvData);

		// Funkciók kezelése
		if(cmd == "funkcio") // id 28
			Funkciok(recvData);

		// Éppen hova irjon a bot conzolból
		if(cmd == "szoba") // id 29
			Szoba(recvData);

		// Channel informáciok
		if(cmd == "channel") // id 30
			Channel(recvData);

		// Üzenet küldése egy nick-nek
		if(cmd == "uzenet") // id 31
			Uzenet(recvData);

		// Hl üzenet
		if(cmd == "hluzenet") // id 32
			HLFunkcio(recvData);

		// Svn kezelés
		if(cmd == "svn") // id 33
			Svn(recvData);

		// Git kezelés
		if(cmd == "git") // id 34
			Git(recvData);

		// Hg kezelés
		if(cmd == "hg") // id 35
			Hg(recvData);

		// Reload
		if(cmd == "reload") // id 36
			Reload(recvData);

		// Bot leállitása
		if(cmd == "kikapcs") // id 37
			Kikapcsolas(recvData);
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", PARANCSOK);
#endif
}

void IRCSession::HandlePing(IRCMessage& recvData)
{
	// Ping? Pong!
	WriteLine("PONG :%s", recvData.args.c_str());
	//Log.Notice("IRCSession", "Ping? Pong!");
}

void IRCSession::HandlePong(IRCMessage& recvData)
{
	// Ping? Pong!
	WriteLine("PONG :%s", recvData.args.c_str());
	//Log.Notice("IRCSession", "Ping? Pong!");
}

void IRCSession::HandleKick(IRCMessage& recvData)
{
	vector<string> res(1);
	sVezerlo.split(recvData.minden, " ", res);

	if(res.size() < 5)
	{
		res.clear();
		return;
	}

	if(res[4] == m_NickTarolo)
	{
		if(FSelect(REJOIN) == bekapcsol && FSelectChannel(REJOIN, recvData.target) == bekapcsol)
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
		if(sConsole.GetConsolLog() == bekapcsol)
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

			printf("%s kickelte a kovetkezo felhasznalot: %s oka: %s\n", recvData.source_nick.c_str(), res[4].c_str(), oka.substr(2).c_str());
		}
	}

#ifdef _DEBUG_MOD
	Log.Warning("Funkcio", "A %s funkcio nem uzemel!", REJOIN);
#endif

	res.clear();
}

void IRCSession::HandleJoin(IRCMessage& recvData)
{
	if(recvData.source_nick == m_NickTarolo)
		return;

	HJoin(recvData);
}

void IRCSession::HandleLeft(IRCMessage& recvData)
{
	if(recvData.source_nick == m_NickTarolo)
		return;

	HLeft(recvData);
}

void IRCSession::HandleQuit(IRCMessage& recvData)
{
	//HQuit(recvData);
}

void IRCSession::HandleReJoin(IRCMessage& recvData)
{
	ReJoin(recvData);
}

void IRCSession::HandleMode(IRCMessage& recvData)
{
	if(sConsole.GetConsolLog() == bekapcsol)
	{
		if(recvData.target == m_NickTarolo)
			return;

		vector<string> res(1);
		sVezerlo.split(recvData.minden, " ", res);
		if(res.size() < 6)
		{
			res.clear();
			return;
		}

		printf("Megvaltoztatta %s rangjat: ", res[5].c_str());
		Log.Color(TBLUE);
		printf("%s. ", recvData.source_nick.c_str());
		Log.Color(TNORMAL);
		printf("Helye: ");
		Log.Color(TRED);
		printf("%s\n", recvData.target.c_str());
		Log.Color(TNORMAL);

		res.clear();
	}
}

void IRCSession::HandleNick(IRCMessage& recvData)
{
	if(sConsole.GetConsolLog() == bekapcsol)
	{
		vector<string> res(1);
		sVezerlo.split(recvData.args, ":", res);

		Log.Color(TRED);
		printf("%s", recvData.source_nick.c_str());
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
	sVezerlo.split(recvData.minden, " ", res);
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
	sVezerlo.split(recvData.minden, " ", res);
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
	sVezerlo.split(recvData.minden, " ", res);
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
