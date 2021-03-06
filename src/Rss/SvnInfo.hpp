/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2010-2011 Megax <http://www.megaxx.info/>
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

#ifndef _SCHUMIX_SVN_INFO_HPP
#define _SCHUMIX_SVN_INFO_HPP

class MySQLConnection;
class IRCSession;

class SvnInfo : public Singleton<SvnInfo>
{
public:
	SvnInfo(string host, string user, string password, string database);
	~SvnInfo();

	void NewThread(uint32 id);
	void StopThread(uint32 id);
	void ReloadAllThread();
	void ReloadThread(uint32 id);
	void Leallas();

	void Thread(uint32 id);
	bool Running(uint32 id) { return m_running[id]; }

protected:
	int getrfa(uint32 id, string title);
	string titleUrl(uint32 id);
	string authorUrl(uint32 id);

	void Kiiras(uint32 id);
	void Lekerdezes(uint32 id);
	void Feltoltes(uint32 id);

	string nev[MaxSvnID];
	string url[MaxSvnID];
	string oldal[MaxSvnID];
	string regex[MaxSvnID];
	string regex2[MaxSvnID];
	uint8 engedely[MaxSvnID];
	string account[MaxSvnID];
	string password[MaxSvnID];

	string _mysql[4];
	uint32 a_rev[MaxSvnID];
	volatile bool m_running[MaxSvnID];
	inline void lekerdezesi_ido() { Sleep(1000); }

	uint16 m_Lido;

private:
	// Url kezeles
	CURL* m_Curl[MaxSvnID];
	// Mysql kapcsolat.
	MySQLConnectionPointer m_SQLConn[MaxSvnID];
	static int writer(char* data, size_t size, size_t nmemb, string *buffer);
};

#define sSvnInfo SvnInfo::getSingleton()

struct SMultiThread
{
	uint32 _id;

	SMultiThread(uint32 id)
	{
		_id = id;
	}

	~SMultiThread()
	{
#ifdef _DEBUG_MOD
		Log.Notice("SvnInfo", "MultiThread::~MultiThread()");
#endif
	}

	void operator()()
	{
		sSvnInfo.Thread(_id);
	}
};

#endif
