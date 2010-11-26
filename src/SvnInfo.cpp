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

initialiseSingleton(SvnInfo);
boost::mutex s_mutex;

SvnInfo::SvnInfo(string host, string user, string password, string database)
{
	_mysql[0] = host;
	_mysql[1] = user;
	_mysql[2] = password;
	_mysql[3] = database;

	m_SQLConn[0] = MySQLConnectionPointer(new MySQLConnection(_mysql[0], _mysql[1], _mysql[2]));
	m_SQLConn[0]->UseDatabase(_mysql[3]);
	m_SQLConn[0]->kiiras = false;

	if(!m_SQLConn[0]->GetSqlError())
		Log.Notice("SvnInfo", "Mysql adatbazishoz sikeres a kapcsolodas.");
	else
		Log.Error("SvnInfo", "Mysql adatbazishoz sikertelen a kapcsolodas.");

	uint32 Threadszam = NULL;
	m_Lido = cast_uint16(Config.MainConfig.GetIntDefault("LekerdezesiIdo", "Svninfo", 15));

	QueryResultPointer adatbazis = m_SQLConn[0]->Query("SELECT * FROM svninfo");
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
	Log.Debug("SvnInfo", "%u Thread indult el.", Threadszam);

	QueryResultPointer db = m_SQLConn[0]->Query("SELECT funkcio_status FROM schumix WHERE funkcio_nev = 'svn'");
	if(db)
		status = db->Fetch()[0].GetString();

	if(status == bekapcsol)
		Log.Success("SvnInfo", "A funkcio elindult.");
	else
		Log.Warning("SvnInfo", "Az %s funkcio nem uzemel!", SVN);

	printf("\n");
}

SvnInfo::~SvnInfo()
{
#ifdef _DEBUG_MOD
	Log.Notice("SvnInfo", "~SvnInfo()");
#endif
}

void SvnInfo::Feltoltes(uint32 id)
{
	int szam = 1;

	QueryResultPointer adatbazis = m_SQLConn[0]->Query("SELECT * FROM svninfo WHERE id = '%u'", id);
	if(adatbazis)
	{
		nev[id] = adatbazis->Fetch()[szam++].GetString();
		url[id] = adatbazis->Fetch()[szam++].GetString();
		oldal[id] = adatbazis->Fetch()[szam++].GetString();

		if(oldal[id] == "arcemu")
		{
			regex[id] = "<item>\\s*<title>(.*)</title>";
			regex2[id] = "<dc:creator>(.+)</dc:creator>";
		}
		else if(oldal[id] == "assembla")
		{
			regex[id] = "<item>\\s*<title>(.*)</title>";
			regex2[id] = "<author>(.+)</author>";
		}
		else if(oldal[id] == "toma3757")
		{
			regex[id] = "</dc:creator><title>(.*)</title>";
			regex2[id] = "<dc:creator>(.+)</dc:creator>";
		}

		engedely[id] = adatbazis->Fetch()[szam++].GetUInt8();
		account[id] = adatbazis->Fetch()[szam++].GetString();
		password[id] = adatbazis->Fetch()[szam++].GetString();
	}
}

void SvnInfo::NewThread(uint32 id)
{
	if(id >= MaxSvnID)
		return;

	Log.Notice("SvnInfo", "New thread: %s", nev[id].c_str());
	Feltoltes(id);

	m_running[id] = true;
	MultiThread Mt(this, id);
	boost::thread t(Mt);

	Log.Success("SvnInfo", "Thread indult: %s", nev[id].c_str());
}

void SvnInfo::StopThread(uint32 id)
{
	if(id >= MaxSvnID)
		return;

	m_running[id] = false;
	MultiThread Mt(this, id);
	boost::thread t(Mt);

	Log.Success("SvnInfo", "Thread leallt: %s", nev[id].c_str());
}

void SvnInfo::ReloadAllThread()
{
	uint32 Threadszam = NULL;

	QueryResultPointer adatbazis = m_SQLConn[0]->Query("SELECT id FROM svninfo");
	if(adatbazis)
	{
		do 
		{
			uint32 id = adatbazis->Fetch()[0].GetUInt32();
			m_running[id] = false;
			Threadszam++;
		} while(adatbazis->NextRow());
	}

	Log.Notice("SvnInfo", "Reload %u threads.", Threadszam);
	Threadszam = NULL;
	Sleep(2000);

	QueryResultPointer adatbazis1 = m_SQLConn[0]->Query("SELECT id FROM svninfo");
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

	Log.Success("SvnInfo", "%u Thread indult ujra.", Threadszam);
}

void SvnInfo::ReloadThread(uint32 id)
{
	if(id >= MaxSvnID)
		return;

	Log.Notice("SvnInfo", "Reload thread: %s", nev[id].c_str());
	m_running[id] = false;
	Sleep(2000);

	Feltoltes(id);

	m_running[id] = true;
	MultiThread Mt(this, id);
	boost::thread t(Mt);

	Log.Success("SvnInfo", "Thread ujraindult: %s", nev[id].c_str());
}

void SvnInfo::Thread(uint32 id)
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

		QueryResultPointer db = m_SQLConn[id]->Query("SELECT funkcio_status FROM schumix WHERE funkcio_nev = 'svn'");
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
	Log.Warning("SvnInfo", "Thread leallt: %s", nev[id].c_str());
#endif
	ThreadExit(0);
}

int SvnInfo::writer(char* data, size_t size, size_t nmemb, string *buffer)
{
	int result = NULL;

	if(buffer != NULL)
	{
		buffer->append(data, size* nmemb);
		result = size* nmemb;
	}
	return result;
}

int SvnInfo::getrfa(uint32 id, string title)
{
	if(oldal[id] == "toma3757")
	{
		vector<string> res(1);
		sVezerlo.split(title, " ", res);

		if(res.size() < 3)
		{
			res.clear();
			return 0;
		}

		if(res[1] != "Rev")
		{
			res.clear();
			return 0;
		}

		stringstream ss;
		int szam;
		ss << res[2];
		ss >> szam;
		res.clear();

		return szam;
	}
	else if(oldal[id] == "arcemu" || oldal[id] == "assembla")
	{
		vector<string> res(1);
		sVezerlo.split(title, "[", res);

		if(res.size() < 3)
		{
			res.clear();
			return 0;
		}

		if(res[1] != "Changeset ")
		{
			res.clear();
			return 0;
		}

		int szokoz = res[2].find(']');
		string rev = res[2].substr(0, szokoz);
		res.clear();

		stringstream ss;
		int szam;
		ss << rev;
		ss >> szam;

		return szam;
	}

	return 0;
}

string SvnInfo::titleUrl(uint32 id)
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
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, SvnInfo::writer);
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl[id]);
		}
		else
		{
			curl_easy_setopt(m_Curl[id], CURLOPT_URL, url[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, SvnInfo::writer);
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
			Log.Error("SvnInfo", "Title: Hiba a Http lekerdezesben. %s thread", nev[id].c_str());
			return "nincs adat";
		}
	}

	return "nincs adat";
}

string SvnInfo::authorUrl(uint32 id)
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
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, SvnInfo::writer);
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEDATA, &bufferdata);
			result = curl_easy_perform(m_Curl[id]);
		}
		else
		{
			curl_easy_setopt(m_Curl[id], CURLOPT_URL, url[id].c_str());
			curl_easy_setopt(m_Curl[id], CURLOPT_WRITEFUNCTION, SvnInfo::writer);
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
			Log.Error("SvnInfo", "Author: Hiba a Http lekerdezesben. %s thread", nev[id].c_str());
			return "nincs adat";
		}
	}

	return "nincs adat";
}

void SvnInfo::Lekerdezes(uint32 id)
{
	boost::details::pool::guard<boost::mutex> g(s_mutex);
	string title = titleUrl(id);
	if(title == "nincs adat")
		return;

	int rev = getrfa(id, title);
	if(rev == NULL)
		return;

	a_rev[id] = cast_uint32(rev);
}

void SvnInfo::Kiiras(uint32 id)
{
	boost::details::pool::guard<boost::mutex> g(s_mutex);
	string title = titleUrl(id);
	if(title == "nincs adat")
		return;

	int rev = getrfa(id, title);
	if(rev == NULL)
		return;

	if(rev == a_rev[id])
		return;

	a_rev[id] = cast_uint32(rev);

	string author = authorUrl(id);
	if(author == "nincs adat")
		return;

	QueryResultPointer db = m_SQLConn[id]->Query("SELECT channel FROM svninfo WHERE nev = '%s'", nev[id].c_str());
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
			if(oldal[id] == "arcemu" || oldal[id] == "assembla")
			{
				uint8 szokoz = title.find(':');
				string kiiras = sVezerlo.htmldecode(title.substr(szokoz+1));

				sIRCSession.SendChatMessage(PRIVMSG, reschannel[y].c_str(), "3%s Revision: 10%i by %s", nev[id].c_str(), rev, author.c_str());
				sIRCSession.SendChatMessage(PRIVMSG, reschannel[y].c_str(), "3%s Info:%s", nev[id].c_str(), kiiras.c_str());
			}
			else
			{
				vector<string> res(1);
				sVezerlo.split(title, " ", res);

				if(res.size() < 3)
				{
					res.clear();
					continue;
				}

				string alomany;
				int resAdat = res.size();

				for(int x = 4; x < resAdat; x++)
					alomany += " " + res[x];

				string kiiras = sVezerlo.htmldecode(alomany.substr(1));

				sIRCSession.SendChatMessage(PRIVMSG, reschannel[y].c_str(), "3%s Revision: 10%i by %s", nev[id].c_str(), rev, author.c_str());
				sIRCSession.SendChatMessage(PRIVMSG, reschannel[y].c_str(), "3%s Info: %s", nev[id].c_str(), kiiras.c_str());

				res.clear();
			}

			Sleep(1000);
		}

		reschannel.clear();
	}
}

void SvnInfo::Leallas()
{
	QueryResultPointer adatbazis = m_SQLConn[0]->Query("SELECT id FROM svninfo");
	if(adatbazis)
	{
		do 
		{
			uint32 id = adatbazis->Fetch()[0].GetUInt32();
			m_running[id] = false;
		} while(adatbazis->NextRow());
	}

	Log.Notice("SvnInfo", "SvnInfo leallt.");
}
