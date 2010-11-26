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

initialiseSingleton(GitInfo);
boost::mutex g_mutex;

GitInfo::GitInfo(string host, string user, string password, string database)
{
	_mysql[0] = host;
	_mysql[1] = user;
	_mysql[2] = password;
	_mysql[3] = database;

	m_SQLConn[0] = MySQLConnectionPointer(new MySQLConnection(_mysql[0], _mysql[1], _mysql[2]));
	m_SQLConn[0]->UseDatabase(_mysql[3]);
	m_SQLConn[0]->kiiras = false;

	if(!m_SQLConn[0]->GetSqlError())
		Log.Notice("GitInfo", "Mysql adatbazishoz sikeres a kapcsolodas.");
	else
		Log.Error("GitInfo", "Mysql adatbazishoz sikertelen a kapcsolodas.");

	uint32 Threadszam = NULL;
	m_Lido = cast_uint16(Config.MainConfig.GetIntDefault("LekerdezesiIdo", "Gitinfo", 15));

	QueryResultPointer adatbazis = m_SQLConn[0]->Query("SELECT id FROM gitinfo");
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
	Log.Debug("GitInfo", "%u Thread indult el.", Threadszam);

	QueryResultPointer db = m_SQLConn[0]->Query("SELECT funkcio_status FROM schumix WHERE funkcio_nev = 'git'");
	if(db)
		status = db->Fetch()[0].GetString();

	if(status == bekapcsol)
		Log.Success("GitInfo", "A funkcio elindult.");
	else
		Log.Warning("GitInfo", "Az %s funkcio nem uzemel!", GIT);

	printf("\n");
}

GitInfo::~GitInfo()
{
#ifdef _DEBUG_MOD
	Log.Notice("GitInfo", "~GitInfo()");
#endif
}

void GitInfo::Feltoltes(uint32 id)
{
	int szam = 1;

	QueryResultPointer adatbazis = m_SQLConn[0]->Query("SELECT * FROM gitinfo WHERE id = '%u'", id);
	if(adatbazis)
	{
		nev[id] = adatbazis->Fetch()[szam++].GetString();
		tipus[id] = adatbazis->Fetch()[szam++].GetString();
		url[id] = adatbazis->Fetch()[szam++].GetString();
		oldal[id] = adatbazis->Fetch()[szam++].GetString();

		if(oldal[id] == "github")
		{
			regex[id] = "<link\\stype=.text/html.\\srel=\\S+.\\shref=.(\\S+)/>\\s*<title>(.*)</title>";
			regex2[id] = "<author>\\s*<name>(.+)</name>";
			regex3[id] = "<entry>\\s*<id>\\S+\\Commit/(.+)</id>";
		}

		engedely[id] = adatbazis->Fetch()[szam++].GetUInt8();
		account[id] = adatbazis->Fetch()[szam++].GetString();
		password[id] = adatbazis->Fetch()[szam++].GetString();
	}
}

void GitInfo::NewThread(uint32 id)
{
	if(id >= MaxGitID)
		return;

	Log.Notice("GitInfo", "New thread: %s %s", nev[id].c_str(), tipus[id].c_str());
	Feltoltes(id);

	m_running[id] = true;
	MultiThread Mt(this, id);
	boost::thread t(Mt);

	Log.Success("GitInfo", "Thread indult: %s %s", nev[id].c_str(), tipus[id].c_str());
}

void GitInfo::StopThread(uint32 id)
{
	if(id >= MaxGitID)
		return;

	m_running[id] = false;
	MultiThread Mt(this, id);
	boost::thread t(Mt);

	Log.Success("GitInfo", "Thread leallt: %s %s", nev[id].c_str(), tipus[id].c_str());
}

void GitInfo::ReloadAllThread()
{
	uint32 Threadszam = NULL;

	QueryResultPointer adatbazis = m_SQLConn[0]->Query("SELECT id FROM gitinfo");
	if(adatbazis)
	{
		do 
		{
			uint32 id = adatbazis->Fetch()[0].GetUInt32();
			m_running[id] = false;
			Threadszam++;
		} while(adatbazis->NextRow());
	}

	Log.Notice("GitInfo", "Reload %u threads.", Threadszam);
	Threadszam = NULL;
	Sleep(2000);

	QueryResultPointer adatbazis1 = m_SQLConn[0]->Query("SELECT id FROM gitinfo");
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

	Log.Success("GitInfo", "%u Thread indult ujra.", Threadszam);
}

void GitInfo::ReloadThread(uint32 id)
{
	if(id >= MaxGitID)
		return;

	Log.Notice("GitInfo", "Reload thread: %s %s", nev[id].c_str(), tipus[id].c_str());
	m_running[id] = false;
	Sleep(2000);

	Feltoltes(id);

	m_running[id] = true;
	MultiThread Mt(this, id);
	boost::thread t(Mt);

	Log.Success("GitInfo", "Thread ujraindult: %s %s", nev[id].c_str(), tipus[id].c_str());
}

void GitInfo::Thread(uint32 id)
{
	m_SQLConn[id] = MySQLConnectionPointer(new MySQLConnection(_mysql[0], _mysql[1], _mysql[2]));
	m_SQLConn[id]->UseDatabase(_mysql[3]);

	string status;
	bool kilepes = false;
	Lekerdezes(id);

	while(Running(id))
	{
		if(!Running(id))
			break;

		QueryResultPointer db = m_SQLConn[id]->Query("SELECT funkcio_status FROM schumix WHERE funkcio_nev = 'git'");
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
	Log.Warning("GitInfo", "Thread leallt: %s %s", nev[id].c_str(), tipus[id].c_str());
#endif
	ThreadExit(0);
}

int GitInfo::writer(char* data, size_t size, size_t nmemb, string *buffer)
{
	int result = NULL;

	if(buffer != NULL)
	{
		buffer->append(data, size* nmemb);
		result = size* nmemb;
	}
	return result;
}

string GitInfo::titleUrl(uint32 id)
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
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, GitInfo::writer);
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl[id]);
		}
		else
		{
			curl_easy_setopt(m_Curl[id], CURLOPT_URL, url[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, GitInfo::writer);
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl[id]);
		}

		curl_easy_cleanup(m_Curl[id]);

		if(result == CURLE_OK)
		{
			boost::regex re(regex[id]);
			boost::cmatch matches;

			boost::regex_search(bufferdata.c_str(), matches, re);
			string matched(matches[2].first, matches[2].second);
			if(matched == "")
				return "nincs adat";

			int szokoz = matched.find("</title>");
			if(szokoz == string::npos)
				return "nincs adat";

			return matched.substr(0, szokoz);
		}
		else
		{
			Log.Error("GitInfo", "Title: Hiba a Http lekerdezesben. %s %s thread", nev[id].c_str(), tipus[id].c_str());
			return "nincs adat";
		}
	}

	return "nincs adat";
}

string GitInfo::revUrl(uint32 id)
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
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, GitInfo::writer);
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl[id]);
		}
		else
		{
			curl_easy_setopt(m_Curl[id], CURLOPT_URL, url[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, GitInfo::writer);
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

			int szokoz = matched.find("</id>");
			if(szokoz == string::npos)
				return "nincs adat";

			return matched.substr(0, szokoz);
		}
		else
		{
			Log.Error("GitInfo", "Rev: Hiba a Http lekerdezesben. %s %s thread", nev[id].c_str(), tipus[id].c_str());
			return "nincs adat";
		}
	}

	return "nincs adat";
}

string GitInfo::authorUrl(uint32 id)
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
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, GitInfo::writer);
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl[id]);
		}
		else
		{
			curl_easy_setopt(m_Curl[id], CURLOPT_URL, url[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, GitInfo::writer);
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

			int szokoz = matched.find("</name>");
			if(szokoz == string::npos)
				return "nincs adat";

			return matched.substr(0, szokoz);
		}
		else
		{
			Log.Error("GitInfo", "Author: Hiba a Http lekerdezesben. %s %s thread", nev[id].c_str(), tipus[id].c_str());
			return "nincs adat";
		}
	}

	return "nincs adat";
}

void GitInfo::Lekerdezes(uint32 id)
{
	boost::details::pool::guard<boost::mutex> g(g_mutex);
	string rev = revUrl(id);
	if(rev == "nincs adat")
		return;

	a_rev[id] = rev;
}

void GitInfo::Kiiras(uint32 id)
{
	boost::details::pool::guard<boost::mutex> g(g_mutex);
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

	QueryResultPointer db = m_SQLConn[id]->Query("SELECT channel FROM gitinfo WHERE id = '%u'", id);
	if(db)
	{
		vector<string> reschannel(1);
		sVezerlo.split(db->Fetch()[0].GetString(), ",", reschannel);

		if(reschannel.size() < 2)
		{
			reschannel.clear();
			return;
		}

		int resChannel = reschannel.size();

		for(int y = 1; y < resChannel; y++)
		{
			string kiiras = sVezerlo.htmldecode(title);

			sIRCSession.SendChatMessage(PRIVMSG, reschannel[y].c_str(), "3%s 7%s Revision: 10%s by %s", nev[id].c_str(), tipus[id].c_str(), rev.substr(0, 10).c_str(), author.c_str());
			sIRCSession.SendChatMessage(PRIVMSG, reschannel[y].c_str(), "3%s Info: %s", nev[id].c_str(), kiiras.c_str());

			Sleep(1000);
		}

		reschannel.clear();
	}
}

void GitInfo::Leallas()
{
	QueryResultPointer adatbazis = m_SQLConn[0]->Query("SELECT id FROM gitinfo");
	if(adatbazis)
	{
		do 
		{
			uint32 id = adatbazis->Fetch()[0].GetUInt32();
			m_running[id] = false;
		} while(adatbazis->NextRow());
	}

	Log.Notice("GitInfo", "GitInfo leallt.");
}
