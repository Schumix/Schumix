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

initialiseSingleton(HgInfo);
boost::mutex h_mutex;

HgInfo::HgInfo()
{
	uint32 Threadszam = NULL;
	m_Lido = cast_uint16(Config.MainConfig.GetIntDefault("LekerdezesiIdo", "Hginfo", 15));

	QueryResultPointer adatbazis = sVezerlo.GetSQLConn()->Query("SELECT id FROM hginfo");
	if(adatbazis)
	{
		do 
		{
			uint32 id = adatbazis->Fetch()[0].GetUInt32();
			Feltoltes(id);

			m_running[id] = true;
			MultiThread Mt(this, id);
			boost::thread t(Mt);
			Threadszam++;
		} while(adatbazis->NextRow());
	}

	string status;
	Log.Debug("HgInfo", "%u Thread indult el.", Threadszam);

	QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT funkcio_status FROM schumix WHERE funkcio_nev = 'hg'");
	if(db)
		status = db->Fetch()[0].GetString();

	if(status == bekapcsol)
		Log.Success("HgInfo", "A funkcio elindult.");
	else
		Log.Warning("HgInfo", "Az %s funkcio nem uzemel!", HG);

	printf("\n");
}

HgInfo::~HgInfo()
{
#ifdef _DEBUG_MOD
	Log.Notice("HgInfo", "~HgInfo()");
#endif
}

void HgInfo::Feltoltes(uint32 id)
{
	int szam = 1;

	QueryResultPointer adatbazis = sVezerlo.GetSQLConn()->Query("SELECT * FROM hginfo WHERE id = '%u'", id);
	if(adatbazis)
	{
		nev[id] = adatbazis->Fetch()[szam++].GetString();
		url[id] = adatbazis->Fetch()[szam++].GetString();
		oldal[id] = adatbazis->Fetch()[szam++].GetString();

		if(oldal[id] == "google")
		{
			regex[id] = "/>\\s*<title>(.*)</title>";
			regex2[id] = "<author>\\s*<name>(.+)</name>";
			string neve = nev[id];
			transform(neve.begin(), neve.end(), neve.begin(), ::tolower);
			regex3[id] = "<id>http://code.google.com/feeds/p/" + neve + "/hgchanges/basic/(.+)</id>";
		}
		else if(oldal[id] == "bitbucket")
		{
			regex[id] = "<item>\\s*<title>(.*)</title>";
			regex2[id] = "<author>(.+)</author>";
			regex3[id] = "<link>http://bitbucket.org/skyne/neocore/changeset/(.+)</link>\\s*<description>";
		}

		engedely[id] = adatbazis->Fetch()[szam++].GetUInt8();
		account[id] = adatbazis->Fetch()[szam++].GetString();
		password[id] = adatbazis->Fetch()[szam++].GetString();
	}
}

void HgInfo::NewThread(uint32 id)
{
	if(id >= MaxHgID)
		return;

	Log.Notice("HgInfo", "New thread: %s", nev[id].c_str());
	Feltoltes(id);

	m_running[id] = true;
	MultiThread Mt(this, id);
	boost::thread t(Mt);

	Log.Success("HgInfo", "Thread indult: %s", nev[id].c_str());
}

void HgInfo::StopThread(uint32 id)
{
	if(id >= MaxHgID)
		return;

	m_running[id] = false;
	MultiThread Mt(this, id);
	boost::thread t(Mt);

	Log.Success("HgInfo", "Thread leallt: %s", nev[id].c_str());
}

void HgInfo::ReloadAllThread()
{
	uint32 Threadszam = NULL;

	QueryResultPointer adatbazis = sVezerlo.GetSQLConn()->Query("SELECT id FROM hginfo");
	if(adatbazis)
	{
		do 
		{
			uint32 id = adatbazis->Fetch()[0].GetUInt32();
			m_running[id] = false;
			Threadszam++;
		} while(adatbazis->NextRow());
	}

	Log.Notice("HgInfo", "Reload %u threads.", Threadszam);
	Threadszam = NULL;
	Sleep(2000);

	QueryResultPointer adatbazis1 = sVezerlo.GetSQLConn()->Query("SELECT id FROM hginfo");
	if(adatbazis1)
	{
		do 
		{
			uint32 id = adatbazis1->Fetch()[0].GetUInt32();
			Feltoltes(id);

			m_running[id] = true;
			MultiThread Mt(this, id);
			boost::thread t(Mt);
			Threadszam++;
		} while(adatbazis1->NextRow());
	}

	Log.Success("HgInfo", "%u Thread indult ujra.", Threadszam);
}

void HgInfo::ReloadThread(uint32 id)
{
	if(id >= MaxHgID)
		return;

	Log.Notice("HgInfo", "Reload thread: %s", nev[id].c_str());
	m_running[id] = false;
	Sleep(2000);

	Feltoltes(id);

	m_running[id] = true;
	MultiThread Mt(this, id);
	boost::thread t(Mt);

	Log.Success("HgInfo", "Thread ujraindult: %s", nev[id].c_str());
}

void HgInfo::Thread(uint32 id)
{
	string status;
	bool kilepes = false;
	Lekerdezes(id);

	while(Running(id))
	{
		if(!Running(id))
			break;

		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT funkcio_status FROM schumix WHERE funkcio_nev = 'hg'");
		if(db)
			status = db->Fetch()[0].GetString();

		if(status == bekapcsol)
		{
			Kiiras(id);
			for(int x = 0; x < m_Lido; x++)
			{
				if(!Running(id))
				{
					kilepes = true;
					break;
				}
				else
					lekerdezesi_ido();
			}

			if(kilepes)
				break;
		}
		else
			Sleep(100);
	}

#ifdef _DEBUG_MOD
	Log.Warning("HgInfo", "Thread leallt: %s", nev[id].c_str());
#endif
	ThreadExit(0);
}

int HgInfo::writer(char* data, size_t size, size_t nmemb, string *buffer)
{
	int result = NULL;

	if(buffer != NULL)
	{
		buffer->append(data, size* nmemb);
		result = size* nmemb;
	}
	return result;
}

string HgInfo::titleUrl(uint32 id)
{
	m_Curl[id] = curl_easy_init();
	if(m_Curl[id])
	{
		CURLcode result;
		string bufferdata;

		if(engedely[id] == 1)
		{
			curl_easy_setopt(m_Curl[id], CURLOPT_URL, url[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_USERNAME, account[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_PASSWORD, password[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, HgInfo::writer);
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl[id]);
		}
		else
		{
			curl_easy_setopt(m_Curl[id], CURLOPT_URL, url[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, HgInfo::writer);
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl[id]);
		}

		curl_easy_cleanup(m_Curl[id]);

		if(result == CURLE_OK)
		{
			boost::regex re(regex[id]);
			boost::cmatch matches;

			boost::regex_search(bufferdata.c_str(), matches, re);
			string matched(matches[1].first, matches[1].second);
			if(matched == "")
				return "nincs adat";

			int szokoz = matched.find("</title>");
			if(szokoz == string::npos)
				return "nincs adat";

			return matched.substr(0, szokoz);
		}
		else
		{
			Log.Error("HgInfo", "Title: Hiba a Http lekerdezesben. %s thread", nev[id].c_str());
			return "nincs adat";
		}
	}

	return "nincs adat";
}

string HgInfo::revUrl(uint32 id)
{
	m_Curl[id] = curl_easy_init();
	if(m_Curl[id])
	{
		CURLcode result;
		string bufferdata;

		if(engedely[id] == 1)
		{
			curl_easy_setopt(m_Curl[id], CURLOPT_URL, url[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_USERNAME, account[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_PASSWORD, password[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, HgInfo::writer);
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl[id]);
		}
		else
		{
			curl_easy_setopt(m_Curl[id], CURLOPT_URL, url[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, HgInfo::writer);
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl[id]);
		}

		curl_easy_cleanup(m_Curl[id]);

		if(result == CURLE_OK)
		{
			boost::regex re(regex3[id]);
			boost::cmatch matches;

			boost::regex_search(bufferdata.c_str(), matches, re);
			string matched(matches[1].first, matches[1].second);
			if(matched == "")
				return "nincs adat";

			int szokoz = matched.find("</");
			if(szokoz == string::npos)
				return "nincs adat";

			return matched.substr(0, szokoz);
		}
		else
		{
			Log.Error("HgInfo", "Rev: Hiba a Http lekerdezesben. %s thread", nev[id].c_str());
			return "nincs adat";
		}
	}

	return "nincs adat";
}

string HgInfo::authorUrl(uint32 id)
{
	m_Curl[id] = curl_easy_init();
	if(m_Curl[id])
	{
		CURLcode result;
		string bufferdata;

		if(engedely[id] == 1)
		{
			curl_easy_setopt(m_Curl[id], CURLOPT_URL, url[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_USERNAME, account[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_PASSWORD, password[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, HgInfo::writer);
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl[id]);
		}
		else
		{
			curl_easy_setopt(m_Curl[id], CURLOPT_URL, url[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, HgInfo::writer);
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl[id]);
		}

		curl_easy_cleanup(m_Curl[id]);

		if(result == CURLE_OK)
		{
			boost::regex re(regex2[id]);
			boost::cmatch matches;

			boost::regex_search(bufferdata.c_str(), matches, re);
			string matched(matches[1].first, matches[1].second);
			if(matched == "")
				return "nincs adat";

			int szokoz = matched.find("</");
			if(szokoz == string::npos)
				return "nincs adat";

			return matched.substr(0, szokoz);
		}
		else
		{
			Log.Error("HgInfo", "Author: Hiba a Http lekerdezesben. %s thread", nev[id].c_str());
			return "nincs adat";
		}
	}

	return "nincs adat";
}

void HgInfo::Lekerdezes(uint32 id)
{
	boost::details::pool::guard<boost::mutex> g(h_mutex);
	string rev = revUrl(id);
	if(rev == "nincs adat")
		return;

	a_rev[id] = rev;
}

void HgInfo::Kiiras(uint32 id)
{
	boost::details::pool::guard<boost::mutex> g(h_mutex);
	string rev = revUrl(id);
	if(rev == "nincs adat")
		return;

	if(rev == a_rev[id])
		return;

	a_rev[id] = rev;

	string title = titleUrl(id);
	if(title == "nincs adat")
		return;

	string author = authorUrl(id);
	if(author == "nincs adat")
		return;

	QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT channel FROM hginfo WHERE nev = '%s'", nev[id].c_str());
	if(db)
	{
		vector<string> reschannel(1);
		split(db->Fetch()[0].GetString(), ",", reschannel);

		if(reschannel.size() < 2)
		{
			reschannel.clear();
			return;
		}

		int resChannel = reschannel.size();

		for(int y = 1; y < resChannel; y++)
		{
			if(oldal[id] == "google")
			{
				uint8 szokoz = title.find(':');
				string kiiras = sVezerlo.htmldecode(title.substr(szokoz+1));

				sIRCSession.SendChatMessage(PRIVMSG, reschannel[y].c_str(), "3%s Revision: 10%s by %s", nev[id].c_str(), rev.substr(0, 10).c_str(), author.c_str());
				sIRCSession.SendChatMessage(PRIVMSG, reschannel[y].c_str(), "3%s Info:%s", nev[id].c_str(), kiiras.c_str());
			}
			else if(oldal[id] == "bitbucket")
			{
				string kiiras = sVezerlo.htmldecode(title);

				sIRCSession.SendChatMessage(PRIVMSG, reschannel[y].c_str(), "3%s Revision: 10%s by %s", nev[id].c_str(), rev.substr(0, 10).c_str(), author.c_str());
				sIRCSession.SendChatMessage(PRIVMSG, reschannel[y].c_str(), "3%s Info: %s", nev[id].c_str(), kiiras.c_str());
			}

			Sleep(1000);
		}

		reschannel.clear();
	}
}

void HgInfo::Leallas()
{
	QueryResultPointer adatbazis = sVezerlo.GetSQLConn()->Query("SELECT id FROM hginfo");
	if(adatbazis)
	{
		do 
		{
			uint32 id = adatbazis->Fetch()[0].GetUInt32();
			m_running[id] = false;
		} while(adatbazis->NextRow());
	}

	Log.Notice("HgInfo", "HgInfo leallt.");
}
