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

#ifndef _SCHUMIX_HG_INFO_HPP
#define _SCHUMIX_HG_INFO_HPP

class MySQLConnection;
class IRCSession;

class HgInfo : public Singleton<HgInfo>
{
public:
	HgInfo(string host, string user, string password, string database);
	~HgInfo();

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

	string nev[MaxHgID];
	string url[MaxHgID];
	string oldal[MaxHgID];
	string regex[MaxHgID];
	string regex2[MaxHgID];
	string regex3[MaxHgID];
	uint8 engedely[MaxHgID];
	string account[MaxHgID];
	string password[MaxHgID];

	string _mysql[4];
	string a_rev[MaxHgID];
	volatile bool m_running[MaxHgID];
	inline void lekerdezesi_ido() { Sleep(1000); }

	uint16 m_Lido;

private:
	// Url kezeles
	CURL* m_Curl[MaxHgID];
	// Mysql kapcsolat.
	MySQLConnectionPointer m_SQLConn[MaxHgID];
	static int writer(char* data, size_t size, size_t nmemb, string *buffer);
};

#define sHgInfo HgInfo::getSingleton()

struct HMultiThread
{
	uint32 _id;

	HMultiThread(uint32 id)
	{
		_id = id;
	}

	~HMultiThread()
	{
#ifdef _DEBUG_MOD
		Log.Notice("HgInfo", "MultiThread::~MultiThread()");
#endif
	}

	void operator()()
	{
		sHgInfo.Thread(_id);
	}
};

#endif
