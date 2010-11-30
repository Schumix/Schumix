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

void IRCSession::HandlePrivmsg(IRCMessage& recvData)
{
	if(sConsole.GetConsoleLog() == bekapcsol)
	{
		Log.Color(TYELLOW);
		printf("[%s] <%s> %s%s", recvData.GetChannel(), recvData.GetNick(), recvData.GetArgs(), NEWLINE);
		Log.Color(TNORMAL);
	}

	// Log
	Logfajl(recvData);

	if(FSelect(PARANCSOK) == bekapcsol)
	{
		if(FSelectChannel(PARANCSOK, recvData.Channel) != bekapcsol && cast_int(recvData.Channel.find("#")) != string::npos)
			return;

		if(AutoKick(recvData, "privmsg"))
			return;

		if(FSelect(MODE) == bekapcsol && FSelectChannel(MODE, recvData.Channel) == bekapcsol)
		{
			AutoMode = true;
			ModeChannel = recvData.Channel;
			WriteLine("NICKSERV STATUS %s", recvData.GetNick());
		}
#ifdef _DEBUG_MOD
		else
			Log.Warning("Funkcio", "A %s funkcio nem uzemel!", MODE);
#endif

		// Hl üzenet
		HLUzenet(recvData);

		// Schumix
		Schumix(recvData);

		// Parancs előjel
		if(recvData.Args.substr(0, 1) != m_ParancsElojel)
			return;

		firstSpace = recvData.Args.find(' ');
		if(firstSpace == string::npos)
			firstSpace = recvData.Args.length();

		string cmd = recvData.Args.substr(1, firstSpace - 1);
		transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

		// parancsok
		if(cmd == "xbot") // id 1
		{
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Verzió: 10%s", revision);
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Parancsok: %sinfo | %shelp | %sxrev | %sido | %sdatum | %sirc | %sroll | %sszam | %skeres | %sfordit | %ssha1 | %smd5 | %suzenet | %swhois | %sjegyzet", m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str());
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "Programmed by: 3Csaba");
			return;
		}

		// Admin parancsok
		if(cmd == "admin") // id 2
			Admin(recvData);

		// Hozzáférés az admin parancsokhoz
		if(cmd == "hozzaferes") // id 3
			Hozzaferes(recvData);

		// Admin jelszó cserélő
		if(cmd == "ujjelszo") // id 4
			Ujjelszo(recvData);

		// Jegyzetek
		if(cmd == "jegyzet") // id 5
			Jegyzet(recvData);

		// Bot help
		if(cmd == "help") // id 6
		{
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ha egy parancs mögé irod a help-et segít a használatában!");
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "Fő parancsom: %sxbot", m_ParancsElojel.c_str());
			return;
		}

		// Irc szinek
		if(cmd == "szinek") // id 7
		{
			if(!Admin(recvData.Nick, recvData.Host, Operator))
				return;

			SendChatMessage(PRIVMSG, recvData.GetChannel(), "1teszt1 2teszt2 3teszt3 4teszt4 5teszt5 6teszt6 7teszt7 8teszt8");
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "9teszt9 10teszt10 11teszt11 12teszt12 13teszt13 14teszt14 15teszt15");
			return;
		}

		// info
		if(cmd == "info") // id 8
		{
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "Programozóm: Csaba, Twl és Scymex. Üzemeltető: %s.", m_Uzemelteto.c_str());
			return;
		}

		if(cmd == "online") // id 9
		{
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nem üzemel a funkció!");
			return;
		}

		// Helyi idő
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
			SendChatMessage(PRIVMSG, recvData.GetChannel(), "Százalékos aránya %u!", adat);
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

		// AutoFunkcio
		if(cmd == "autofunkcio") // id 37
			AutoFunkcio(recvData);

		// Bot leállitása
		if(cmd == "kikapcs") // id 38
			Kikapcsolas(recvData);
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", PARANCSOK);
#endif
}
