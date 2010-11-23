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

void IRCSession::Schumix(IRCMessage& recvData)
{
	if(FSelectChannel(PARANCSOK, recvData.target) != bekapcsol)
		return;

	string _nick = m_NickName[0];
	int vesszo = recvData.args.find(",");
	transform(_nick.begin(), _nick.end(), _nick.begin(), ::tolower);

	if(recvData.args.substr(0, vesszo) == _nick)
	{
		uint8 elsoszokoz = recvData.args.find(' ');
		if(elsoszokoz == NULL)
			elsoszokoz = recvData.args.length();

		if(recvData.args.length() <= elsoszokoz+1)
		{
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
			return;
		}

		vector<string> res(1);
		sVezerlo.split(recvData.args.substr(elsoszokoz+1), " ", res);

		if(res.size() < 2)
		{
			res.clear();
			return;
		}

		string iras = res[1];

		if(iras == "info")
		{
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Verzió: 10%s", revision);
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Platform: %s", PLATFORM_TEXT);
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Programnyelv: c++");
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Uptime: %s", sVezerlo.GetUptimeString().c_str());
		}
		else if(iras == "nick")
		{
			if(!Admin(recvData.source_nick, recvData.source_host, Operator))
				return;

			if(res.size() < 3)
			{
				res.clear();
				return;
			}

			if(res[2] == "identify")
			{
				m_NickTarolo = m_NickName[0];
				WriteLine("NICK %s", m_NickName[0].c_str());
				Log.Notice("NickServ", "Sending NickServ identification.");
				SendChatMessage(PRIVMSG, "NickServ", "identify %s", m_NickServPassword.c_str());
			}
			else
			{
				string nick = res[2];
				m_NickTarolo = nick;
				WriteLine("NICK %s", nick.c_str());
			}
		}
		else if(iras == "ghost")
		{
			if(!Admin(recvData.source_nick, recvData.source_host, Operator))
				return;

			SendChatMessage(PRIVMSG, "NickServ", "ghost %s %s", m_NickName[0].c_str(), m_NickServPassword.c_str());
		}
		else if(iras == "sys")
		{
#if PLATFORM == PLATFORM_WINDOWS
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Memoria használat: %f MB", ((sVezerlo.MemoryInfo(GetCurrentProcessId())/1024)/1024));
#else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Memoria használat: %f MB", ((sVezerlo.MemoryInfo()/1000)*0.3762));
#endif
		}
		else if(iras == "help")
		{
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Parancsok: info | ghost | nick | sys");
		}
		else
		{
			//számolás
			setConsts();
			makepolishform(recvData.args.substr(elsoszokoz+1));
			calculate(recvData.target);
		}

		res.clear();
	}
}

void IRCSession::HJoin(IRCMessage& recvData)
{
	if(AutoKick(recvData, "join"))
		return;

	uint8 szokoz = recvData.target.find(':');
	string channel = recvData.target.substr(szokoz+1);

	if(FSelect(MODE) == bekapcsol && FSelectChannel(MODE, channel) == bekapcsol)
	{
		AutoMode = true;
		ModeChannel = channel;
		WriteLine("NICKSERV STATUS %s", recvData.source_nick.c_str());
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
			SendChatMessage(PRIVMSG, channel.c_str(), "Jó reggelt %s", recvData.source_nick.c_str());
		else if(ido >= 20)
			SendChatMessage(PRIVMSG, channel.c_str(), "Jó estét %s", recvData.source_nick.c_str());
		else
		{
			if(Admin(recvData.source_nick))
				SendChatMessage(PRIVMSG, channel.c_str(), "Üdv fõnök");
			else
				SendChatMessage(PRIVMSG, channel.c_str(), "%s %s", Koszones.c_str(), recvData.source_nick.c_str());
		}
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", KOSZONES);
#endif
}

void IRCSession::HLeft(IRCMessage& recvData)
{
	if(FSelect(KOSZONES) == bekapcsol && FSelectChannel(KOSZONES, recvData.target) == bekapcsol)
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
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Jóét %s", recvData.source_nick.c_str());
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s %s", elkoszones.c_str(), recvData.source_nick.c_str());
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", KOSZONES);
#endif
}

void IRCSession::HQuit(IRCMessage& recvData)
{
/*	if(FSelect(KOSZONES) == bekapcsol && FSelectChannel(KOSZONES, recvData.target) == bekapcsol)
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
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Jóét %s", recvData.source_nick.c_str());
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s %s", elkoszones.c_str(), recvData.source_nick.c_str());
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", KOSZONES);
#endif*/
}

void IRCSession::ReJoin(IRCMessage& recvData)
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

string IRCSession::FSelect(string nev)
{
	string status;

	QueryResultPointer db = m_SQLConn->Query("SELECT funkcio_status FROM schumix WHERE funkcio_nev = '%s'", nev.c_str());
	if(db)
		status = db->Fetch()[0].GetString();

	return status;
}

string IRCSession::FSelectChannel(string nev, string channel)
{
	string status;

	vector<string>::iterator it;
	for(it = m_ChannelFunkcio.begin(); it < m_ChannelFunkcio.end(); it++)
	{
		string szobak = (*it);
		uint32 szobaszokoz = szobak.find(".");
		string szoba = szobak.substr(0, szobaszokoz);
		string funkciok = szobak.substr(szobaszokoz+1);

		vector<string> res(1);
		sVezerlo.split(funkciok, ":", res);

		if(szoba == channel)
		{
			int resAdat = res.size();

			for(int i = 1; i < resAdat; i++)
			{
				if(res[i] == bekapcsol)
					continue;
				else if(res[i] == kikapcsol)
					continue;
				else
				{
					if(res[i] == nev)
						status = res[i+1];
				}
			}
		}

		res.clear();
	}

	return status;
}

void IRCSession::HLUzenet(IRCMessage& recvData)
{
	if(FSelect(HL) == bekapcsol && FSelectChannel(HL, recvData.target) == bekapcsol)
	{
		vector<string> res(1);
		sVezerlo.split(recvData.args, " ", res);
		int resAdat = res.size();

		for(int i = 1; i < resAdat; i++)
		{
			transform(res[i].begin(), res[i].end(), res[i].begin(), ::tolower);
			QueryResultPointer db = m_SQLConn->Query("SELECT info, alapot FROM hluzenet WHERE nick = '%s'", res[i].c_str());
			if(db)
			{
				string info = db->Fetch()[0].GetString();
				string alapot = db->Fetch()[1].GetString();

				if(alapot != bekapcsol)
					return;

				SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s", info.c_str());
				break;
			}
		}

		res.clear();
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", HL);
#endif
}

void IRCSession::SvnSandshroud(int rev, string channel)
{
	char* url = new char[400];
	snprintf(url, 400, "http://trac6.assembla.com/Sandshroud/changeset/%i", rev);
	string urlcim = url;
	delete[] url;

	string commit;
	string author;

	// commit
	m_Curl = curl_easy_init();
	if(m_Curl)
	{
		string bufferdata;

		curl_easy_setopt(m_Curl, CURLOPT_URL, urlcim.c_str());
		curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, IRCSession::writer);
		curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, &bufferdata);
		CURLcode result = curl_easy_perform(m_Curl);

		curl_easy_cleanup(m_Curl);

		if(result == CURLE_OK)
		{
			boost::regex re("<p>\\s*(.*)<br />\\s*</p>");
			boost::cmatch matches;

			boost::regex_search(bufferdata.c_str(), matches, re);
			string matched(matches[1].first, matches[1].second);

			vector<string> res(1);
			sVezerlo.split(matched, "\n", res);
			int resAdat = res.size();
			string alomany;

			for(int i = 1; i < resAdat; i++)
				alomany += " " + res[i];

			commit = alomany;
			res.clear();
		}
		else
		{
			Log.Error("IRCSession", "Hiba a Http lekerdezesben.");
			return;
		}
	}

	// author
	m_Curl = curl_easy_init();
	if(m_Curl)
	{
		string bufferdata;

		curl_easy_setopt(m_Curl, CURLOPT_URL, urlcim.c_str());
		curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, IRCSession::writer);
		curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, &bufferdata);
		CURLcode result = curl_easy_perform(m_Curl);

		curl_easy_cleanup(m_Curl);

		if(result == CURLE_OK)
		{
			boost::regex re("Author:</dt>\\s*<dd class=.author.>(.*)</dd>\\s*<dt class=.property message.>Message:</dt>");
			boost::cmatch matches;

			boost::regex_search(bufferdata.c_str(), matches, re);
			string matched(matches[1].first, matches[1].second);
			author = matched;
		}
		else
		{
			Log.Error("IRCSession", "Hiba a Http lekerdezesben.");
			return;
		}
	}

	if(commit == "" && author == "")
		SendChatMessage(PRIVMSG, channel.c_str(), "Nincs ilyen rev.");
	else
		SendChatMessage(PRIVMSG, channel.c_str(), "3Sandshroud rev 10%i. Fejlesztõ: 12%s. Commit:%s", rev, author.c_str(), commit.c_str());
}

void IRCSession::ChannelFunkcioReload()
{
	m_ChannelFunkcio.clear();

	map<string, string>::iterator itr = m_ChannelLista.begin();
	for(; itr != m_ChannelLista.end(); itr++)
	{
		string szoba = itr->first;

		QueryResultPointer db = m_SQLConn->Query("SELECT funkciok FROM channel WHERE szoba = '%s'", szoba.c_str());
		if(db)
		{
			string funkciok = db->Fetch()[0].GetString();
			vector<string> res(1);
			sVezerlo.split(funkciok, ",", res);
			int resAdat = res.size();

			for(int i = 1; i < resAdat; i++)
			{
				string szobaadat = szoba + "." + res[i];
				m_ChannelFunkcio.push_back(szobaadat);
			}

			res.clear();
		}
	}
}

void IRCSession::ChannelListaReload()
{
	m_ChannelLista.clear();
	QueryResultPointer db = m_SQLConn->Query("SELECT szoba, jelszo FROM channel");
	if(db)
	{
		do 
		{
			string channel = db->Fetch()[0].GetString();
			string jelszo = db->Fetch()[1].GetString();
			m_ChannelLista.insert(make_pair(channel, jelszo));
		} while(db->NextRow());
	}
}

string IRCSession::ChannelFunkciok(string nev, string status, string channel)
{
	string funkcio;

	vector<string>::iterator it;
	for(it = m_ChannelFunkcio.begin(); it < m_ChannelFunkcio.end(); it++)
	{
		string szobak = (*it);
		uint32 szobaszokoz = szobak.find(".");
		string szoba = szobak.substr(0, szobaszokoz);
		string funkciok = szobak.substr(szobaszokoz+1);

		vector<string> res(1);
		sVezerlo.split(funkciok, ":", res);
		if(res.size() < 2)
		{
			res.clear();
			continue;
		}

		if(szoba == channel)
		{
			if(res[1] != nev)
				funkcio += "," + funkciok;
		}

		res.clear();
	}

	vector<string>::iterator it1;
	for(it1 = m_ChannelFunkcio.begin(); it1 < m_ChannelFunkcio.end(); it1++)
	{
		string szobak = (*it1);
		uint32 szobaszokoz = szobak.find(".");
		string szoba = szobak.substr(0, szobaszokoz);
		string funkciok = szobak.substr(szobaszokoz+1);

		vector<string> res(1);
		sVezerlo.split(funkciok, ":", res);
		if(res.size() < 2)
		{
			res.clear();
			continue;
		}

		if(szoba == channel)
		{
			if(res[1] == nev)
				funkcio += "," + nev + ":" + status;
		}

		res.clear();
	}

	return funkcio;
}

string IRCSession::ChannelFunkciokInfo(string channel)
{
	string be, ki;

	vector<string>::iterator it;
	for(it = m_ChannelFunkcio.begin(); it < m_ChannelFunkcio.end(); it++)
	{
		string szobak = (*it);
		uint32 szobaszokoz = szobak.find(".");
		string szoba = szobak.substr(0, szobaszokoz);
		string funkciok = szobak.substr(szobaszokoz+1);

		vector<string> res(1);
		sVezerlo.split(funkciok, ":", res);

		if(szoba == channel)
		{
			int resAdat = res.size();

			for(int i = 1; i < resAdat; i++)
			{
				if(res[i] == bekapcsol)
					continue;
				else if(res[i] == kikapcsol)
					continue;
				else
				{
					if(res[i+1] == bekapcsol)
						be += res[i] + " ";
					else
						ki += res[i] + " ";
				}
			}
		}

		res.clear();
	}

	return be + "|" + ki;
}

bool IRCSession::AutoKick(IRCMessage& recvData, string allapot)
{
	if(allapot == "join")
	{
		uint8 szokoz = recvData.target.find(':');
		string channel = recvData.target.substr(szokoz+1);

		if(FSelect(KICK) == bekapcsol && FSelectChannel(KICK, channel) == bekapcsol)
		{
			transform(recvData.source_nick.begin(), recvData.source_nick.end(), recvData.source_nick.begin(), ::tolower);
			QueryResultPointer db = m_SQLConn->Query("SELECT channel, oka FROM kicklista WHERE nick = '%s'", recvData.source_nick.c_str());
			if(db)
			{
				channel = db->Fetch()[0].GetString();
				string oka = db->Fetch()[1].GetString();
				WriteLine("KICK %s %s :%s", channel.c_str(), recvData.source_nick.c_str(), oka.c_str());
				return true;
			}
		}
#ifdef _DEBUG_MOD
		else
			Log.Warning("Funkcio", "A %s funkcio nem uzemel!", KICK);
#endif
		return false;
	}

	if(allapot == "privmsg")
	{
		if(FSelect(KICK) == bekapcsol && FSelectChannel(KICK, recvData.target) == bekapcsol)
		{
			transform(recvData.source_nick.begin(), recvData.source_nick.end(), recvData.source_nick.begin(), ::tolower);
			QueryResultPointer db = m_SQLConn->Query("SELECT channel, oka FROM kicklista WHERE nick = '%s'", recvData.source_nick.c_str());
			if(db)
			{
				string channel = db->Fetch()[0].GetString();
				string oka = db->Fetch()[1].GetString();
				WriteLine("KICK %s %s :%s", channel.c_str(), recvData.source_nick.c_str(), oka.c_str());
				return true;
			}
		}
#ifdef _DEBUG_MOD
		else
			Log.Warning("Funkcio", "A %s funkcio nem uzemel!", KICK);
#endif
		return false;
	}

	return false;
}
