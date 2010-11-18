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

void IRCSession::Teszt(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Administrator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string iras = res[1];

	if(iras == "svn")
	{
		/*m_Curl = curl_easy_init();
		if(m_Curl)
		{
			CURLcode result;
			string bufferdata;

			curl_easy_setopt(m_Curl, CURLOPT_URL, "http://github.com/mangos/mangos/commits/master.atom");
			curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, IRCSession::writer);
			curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl);

			curl_easy_cleanup(m_Curl);

			if(result == CURLE_OK)
			{
				boost::regex re("</name>\\s*</author>\\s*<content type=\\\"html\\\">\\s*&lt;pre>(.*)&lt;/pre>\\s*&lt;pre style='white-space:pre-wrap;width:81ex'>(.*)&lt;/pre>\\s*</content>");
				boost::cmatch matches;

				boost::regex_search(bufferdata.c_str(), matches, re);
				string matched(matches[2].first, matches[2].second);

				cout << bufferdata.c_str() << "\n";
				cout << matched.c_str();
			}
		}*/
	}
	else
	{
		printf("%s\n", recvData.hostmask.c_str());
		printf("%s\n", recvData.opcode.c_str());
		printf("%s\n", recvData.target.c_str());
		printf("%s\n", recvData.args.c_str());
		printf("%s\n", recvData.source_nick.c_str());
		printf("%s\n", recvData.source_user.c_str());
		printf("%s\n", recvData.source_host.c_str());
	}

	res.clear();
}

void IRCSession::Szoba(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Administrator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	if(res[1] == Help)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Segitség a konzol szoba váltásához!");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Funkció használata: %sszoba <ide jön a szoba>", m_ParancsElojel.c_str());
	}
	else
		m_SQLConn->Query("UPDATE schumix SET irc_cim = '%s' WHERE entry = '1'", res[1].c_str());

	res.clear();
}

void IRCSession::Reload(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Administrator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string info = res[1];

	if(info == Help)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Alparancsok használata:");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Reload: %sreload <nev>", m_ParancsElojel.c_str());
	}
	else if(info == INFO)
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Most újraindithatóak: IRCSession, SvnInfo, GitInfo, HgInfo, Console");
	else
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s", sVezerlo.Reload(info).c_str());

	res.clear();
}

void IRCSession::Kikapcsolas(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Administrator))
		return;

	SendChatMessage(PRIVMSG, recvData.target.c_str(), "Viszlát :(");
	WriteLine("QUIT :%s leallitott parancsal.", recvData.source_nick.c_str());
	Sleep(1000);
	sVezerlo.Leallas();
}
