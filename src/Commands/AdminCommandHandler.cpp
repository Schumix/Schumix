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

#include "../StdAfx.h"

void CommandMgr::HandleTeszt(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Administrator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

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
		printf("%s\n", recvData.GetChannel());
		printf("%s\n", recvData.GetArgs());
		printf("%s\n", recvData.GetNick());
		printf("%s\n", recvData.GetUser());
		printf("%s\n", recvData.GetHost());
	}

	res.clear();
}

void CommandMgr::HandleSzoba(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Administrator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	if(res[1] == Help)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Segitség a konzol szoba váltásához!");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Funkció használata: %sszoba <ide jön a szoba>", sIRCSession.GetParancsElojel());
	}
	else
		sVezerlo.GetSQLConn()->Query("UPDATE schumix SET irc_cim = '%s' WHERE entry = '1'", res[1].c_str());

	res.clear();
}

void CommandMgr::HandleReload(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Administrator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string info = res[1];

	if(info == Help)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Alparancsok használata:");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Reload: %sreload <nev>", sIRCSession.GetParancsElojel());
	}
	else if(info == INFO)
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Most újraindithatóak: IRCSession, SvnInfo, GitInfo, HgInfo, Console");
	else
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s", sVezerlo.Reload(info).c_str());

	res.clear();
}

void CommandMgr::HandleKikapcsolas(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Administrator))
		return;

	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Viszlát :(");
	sIRCSession.WriteLine("QUIT :%s leallitott parancsal.", recvData.GetNick());
	Sleep(1000);
	sVezerlo.Leallas();
}
