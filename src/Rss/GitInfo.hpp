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

#ifndef _SCHUMIX_GIT_INFO_HPP
#define _SCHUMIX_GIT_INFO_HPP

class MySQLConnection;
class IRCSession;

class GitInfo : public Singleton<GitInfo>
{
public:
	GitInfo(string host, string user, string password, string database);
	~GitInfo();

	void NewThread(uint32 id);
	void StopThread(uint32 id);
	void ReloadAllThread();
	void ReloadThread(uint32 id);
	void Leallas();

	void Thread(uint32 id);
	bool Running(uint32 id) { return m_running[id]; }

protected:
	string titleUrl(uint32 id);
	string revUrl(uint32 id);
	string authorUrl(uint32 id);

	void Kiiras(uint32 id);
	void Lekerdezes(uint32 id);
	void Feltoltes(uint32 id);

	string nev[MaxGitID];
	string url[MaxGitID];
	string tipus[MaxGitID];
	string oldal[MaxGitID];
	string regex[MaxGitID];
	string regex2[MaxGitID];
	string regex3[MaxGitID];
	uint8 engedely[MaxGitID];
	string account[MaxGitID];
	string password[MaxGitID];

	string _mysql[4];
	string a_rev[MaxGitID];
	volatile bool m_running[MaxGitID];
	inline void lekerdezesi_ido() { Sleep(1000); }

	uint16 m_Lido;

private:
	// Url kezeles
	CURL* m_Curl[MaxGitID];
	// Mysql kapcsolat.
	MySQLConnectionPointer m_SQLConn[MaxGitID];
	static int writer(char* data, size_t size, size_t nmemb, string *buffer);
};

#define sGitInfo GitInfo::getSingleton()

struct GMultiThread
{
	uint32 _id;

	GMultiThread(uint32 id)
	{
		_id = id;
	}

	~GMultiThread()
	{
#ifdef _DEBUG_MOD
		Log.Notice("GitInfo", "MultiThread::~MultiThread()");
#endif
	}

	void operator()()
	{
		sGitInfo.Thread(_id);
	}
};

#endif
