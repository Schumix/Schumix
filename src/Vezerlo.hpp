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

#ifndef _SCHUMIX_VEZERLO_HPP
#define _SCHUMIX_VEZERLO_HPP

class Vezerlo : public Singleton<Vezerlo>
{
public:
	Vezerlo();
	~Vezerlo();

	int Ev();
	int Honap();
	int Nap();
	int Ora();
	int Perc();

	volatile bool m_crash;

	void Leallas();
	void Uptime();
	string Reload(string nev);
	string GetUptimeString();

#if PLATFORM == PLATFORM_WINDOWS
	float MemoryInfo(DWORD processID);
#else
	float MemoryInfo();
#endif

	string urlencode(const string &c);
	string htmldecode(string c);
	vector<string> split(const string str, const string delim, vector<string>& result);

protected:
	// Mysql hozzáférés
	string _mysql[4];

	time_t UNIXTIME;
	uint32 m_StartTime;

	string m_server;
	uint32 m_port;

	string char2hex(char dec);

private:
	// SvnInfo
	SvnInfo* m_SvnInfo;

	// GitInfo
	GitInfo* m_GitInfo;

	// HgInfo
	HgInfo* m_HgInfo;

	// Console
	Console* m_Console;

	// IRCSession
	IRCSession* m_IRCSession;

	// Mysql kapcsolat.
	MySQLConnectionPointer m_SQLConn;

	void _HookSignals();
	void _UnhookSignals();
};

#define sVezerlo Vezerlo::getSingleton()

#endif
