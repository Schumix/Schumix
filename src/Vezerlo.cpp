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

initialiseSingleton(Vezerlo);

Vezerlo::Vezerlo()
{
	// Irc szerver conf
	m_server = Config.MainConfig.GetStringDefault("IRC", "Server", "");
	m_port = Config.MainConfig.GetIntDefault("IRC", "Port", 6667);

	// Mysql Config
	_mysql[0] = Config.MainConfig.GetStringDefault("MySQL", "Host", "");
	_mysql[1] = Config.MainConfig.GetStringDefault("MySQL", "User", "");
	_mysql[2] = Config.MainConfig.GetStringDefault("MySQL", "Password", "");
	_mysql[3] = Config.MainConfig.GetStringDefault("MySQL", "Database", "");

	m_SQLConn = MySQLConnectionPointer(new MySQLConnection(_mysql[0], _mysql[1], _mysql[2]));
	m_SQLConn->UseDatabase(_mysql[3]);

	if(!m_SQLConn->GetSqlError())
		Log.Notice("Vezerlo", "Mysql adatbazishoz sikeres a kapcsolodas.");
	else
	{
		Log.Error("Vezerlo", "Mysql adatbazishoz sikertelen a kapcsolodas.");
		Sleep(5000);
		return;
	}

	// uptime
	UNIXTIME = time(NULL);
	m_StartTime = cast_uint32(UNIXTIME);

	string pidfajl = string(Elnevezes) + ".pid";

	FILE* f = fopen(pidfajl.c_str(), "w");
	if(f)
	{
		uint32 pid;
#if PLATFORM == PLATFORM_WINDOWS
		pid = GetCurrentProcessId();
#else
		pid = getpid();
#endif
		fprintf(f, "%u", (unsigned int)pid);
		fclose(f);
	}

	Log.Debug("Vezerlo", "SvnInfo indul...");
	m_SvnInfo = new SvnInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

	Log.Debug("Vezerlo", "GitInfo indul...");
	m_GitInfo = new GitInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

	Log.Debug("Vezerlo", "HgInfo indul...");
	m_HgInfo = new HgInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

	Log.Debug("Vezerlo", "Console indul...");
	m_Console = new Console(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

	Log.Debug("Vezerlo", "IRCSession indul...");
	m_IRCSession = new IRCSession(m_server, m_port, _mysql[0], _mysql[1], _mysql[2], _mysql[3]);

	// delete pid f�jl
	remove(pidfajl.c_str());
	delete this;
}

Vezerlo::~Vezerlo()
{
#ifdef _DEBUG_MOD
	Log.Notice("Vezerlo", "~Vezerlo()");
#endif

	if(m_SvnInfo)
		delete m_SvnInfo;

	if(m_GitInfo)
		delete m_GitInfo;

	if(m_HgInfo)
		delete m_HgInfo;

	if(m_Console)
		delete m_Console;

	if(m_IRCSession)
		delete m_IRCSession;
}

void Vezerlo::split(string str, string delim, vector<string>& results)
{
	int at;
	while((at = str.find_first_of(delim)) != str.npos)
	{
		if(at > 0)
			results.push_back(str.substr(0, at));

		str = str.substr(at+1);
	}

	if(str.length() > 0)
		results.push_back(str);
}

int Vezerlo::Ev()
{
	time_t raw;
	struct tm *tinf = NULL;
	char* buffer = new char[80];

	time(&raw);
	tinf = localtime(&raw);

	strftime(buffer, 80, "%Y", tinf);
	string ev(buffer);
	delete[] buffer;

	stringstream ss;
	int iev;
	ss << ev;
	ss >> iev;

	return iev;
}

int Vezerlo::Honap()
{
	time_t raw;
	struct tm *tinf = NULL;
	char* buffer = new char[80];

	time(&raw);
	tinf = localtime(&raw);

	strftime(buffer, 80, "%m", tinf);
	string honap(buffer);
	delete[] buffer;

	stringstream ss;
	int ihonap;
	ss << honap;
	ss >> ihonap;

	return ihonap;
}

int Vezerlo::Nap()
{
	time_t raw;
	struct tm *tinf = NULL;
	char* buffer = new char[80];

	time(&raw);
	tinf = localtime(&raw);

	strftime(buffer, 80, "%d", tinf);
	string nap(buffer);
	delete[] buffer;

	stringstream ss;
	int inap;
	ss << nap;
	ss >> inap;

	return inap;
}

int Vezerlo::Ora()
{
	time_t raw;
	struct tm *tinf = NULL;
	char* buffer = new char[80];

	time(&raw);
	tinf = localtime(&raw);

	strftime(buffer, 80, "%H", tinf);
	string ido(buffer);
	delete[] buffer;

	stringstream ss;
	int iido;
	ss << ido;
	ss >> iido;

	return iido;
}

int Vezerlo::Perc()
{
	time_t raw;
	struct tm *tinf = NULL;
	char* buffer = new char[80];

	time(&raw);
	tinf = localtime(&raw);

	strftime(buffer, 80, "%M", tinf);
	string perc(buffer);
	delete[] buffer;

	stringstream ss;
	int iperc;
	ss << perc;
	ss >> iperc;

	return iperc;
}

string Vezerlo::GetUptimeString()
{
	char str[300];
	UNIXTIME = time(NULL);
	time_t pTime = cast_default(time_t, UNIXTIME) - m_StartTime;
	tm* tmv = gmtime(&pTime);

	snprintf(str, 300, "%u nap, %u �ra, %u perc, %u m�sodperc.", tmv->tm_yday, tmv->tm_hour, tmv->tm_min, tmv->tm_sec);
	return string(str);
}

void Vezerlo::Uptime()
{
	char datum[500];
	int honap = Honap();
	int nap = Nap();

	if(honap < 10)
	{
		if(nap < 10)
			snprintf(datum, 500, "%i. 0%i. 0%i. %i:%i", Ev(), honap, nap, Ora(), Perc());
		else
			snprintf(datum, 500, "%i. 0%i. %i. %i:%i", Ev(), honap, nap, Ora(), Perc());
	}
	else
	{
		if(nap < 10)
			snprintf(datum, 500, "%i. %i. 0%i. %i:%i", Ev(), honap, nap, Ora(), Perc());
		else
			snprintf(datum, 500, "%i. %i. %i. %i:%i", Ev(), honap, nap, Ora(), Perc());
	}

#if PLATFORM == PLATFORM_WINDOWS
	m_SQLConn->Query("INSERT INTO `uptime`(datum, uptime, memory) VALUES ('%s', '%s', '%f MB')", datum, GetUptimeString().c_str(), MemoryInfo(GetCurrentProcessId()/1024)/1024);
#else
	m_SQLConn->Query("INSERT INTO `uptime`(datum, uptime, memory) VALUES ('%s', '%s', '%f MB')", datum, GetUptimeString().c_str(), MemoryInfo()/1000*0.3762);
#endif
}

#if PLATFORM == PLATFORM_WINDOWS
float Vezerlo::MemoryInfo(DWORD processID)
{
	PROCESS_MEMORY_COUNTERS pmc;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	if(NULL == hProcess)
		return 0;

	float szam;
	if(GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
		szam = cast_float(pmc.WorkingSetSize);

	CloseHandle(hProcess);

	return szam;
}

#else

struct rusage usage;
float Vezerlo::MemoryInfo()
{
	int who = RUSAGE_SELF; 
	getrusage(who, &usage);

	return cast_float(usage.ru_maxrss);
}
#endif

string Vezerlo::urlencode(const string &c)
{
	string escaped = "";
	int max = c.length();

	for(int i = 0; i < max; i++)
	{
		if(c[i] == '�')
		{
			escaped.append("%C3%A1");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%81");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%A4");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%84");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%A9");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%89");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%AD");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%8D");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%B3");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%93");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%B6");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%96");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C5%91");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C5%90");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%BC");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%9C");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C5%B1");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C5%B0");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%BA");
			continue;
		}
		else if(c[i] == '�')
		{
			escaped.append("%C3%9A");
			continue;
		}
		else if(c[i] == ' ')
		{
			escaped.append("%20");
			continue;
		}

		if((48 <= c[i] && c[i] <= 57) || //0-9
             (65 <= c[i] && c[i] <= 90) || //abc...xyz
             (97 <= c[i] && c[i] <= 122) || //ABC...XYZ
             (c[i] == '~' || c[i] == '!' || c[i] == '*' || c[i] == '(' || c[i] == ')' || c[i] == '\''))
		{
			escaped.append(&c[i], 1);
		}
        else
		{
			escaped.append("%");
			escaped.append(char2hex(c[i])); //converts char 255 to string "ff"
		}
	}

    return escaped;
}

string Vezerlo::char2hex(char dec)
{
    char dig1 = (dec & 0xF0) >> 4;
    char dig2 = (dec & 0x0F);

    if(0 <= dig1 && dig1 <= 9)
		dig1 += 48;    //0,48inascii
    if(10 <= dig1 && dig1 <= 15)
		dig1 += 97-10; //a,97inascii
    if(0 <= dig2 && dig2 <= 9)
		dig2 += 48;
    if(10 <= dig2 && dig2 <= 15)
		dig2 += 97-10;

    string r;
    r.append(&dig1, 1);
    r.append(&dig2, 1);
    return r;
}

string Vezerlo::htmldecode(string c)
{
	string escaped = "";

	vector<string> res(1);
	split(c, "&", res);

	int max = res.size();

	if(max < 2)
	{
		res.clear();
		return c;
	}

	int hossz;
	string betu;
	bool Elsoresz = true;

	for(int i = 1; i < max; i++)
	{
		if(i < max)
		{
			hossz = res[i].length();

			if(res[i].substr(0, 5) == "#225;")	// Kisbet�k
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#233;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#237;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#243;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#246;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#337;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#250;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#252;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#369;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			if(res[i].substr(0, 5) == "#193;")	// Nagybet�k
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#201;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#205;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#211;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#214;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#336;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#218;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#220;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#368;")
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#180;")	// Egy�b karakterek
			{
				res[i].erase(0, 5);
				escaped.append("�");
				escaped.append(res[i]);
				continue;
			}
			else
			{
				if(Elsoresz)
				{
					escaped.append(res[i]);
					Elsoresz = false;
					continue;
				}
				else
				{
					escaped.append("&");
					escaped.append(res[i]);
				}
			}
		}
	}

	res.clear();
	return escaped;
}

string Vezerlo::Reload(string nev)
{
	transform(nev.begin(), nev.end(), nev.begin(), ::tolower);

	if(nev == "svninfo")
	{
		m_SvnInfo->Leallas();
		delete m_SvnInfo;
		Log.Debug("Vezerlo", "SvnInfo reload...");
		m_SvnInfo = new SvnInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);
		return "SvnInfo ujraindult.";
	}
	else if(nev == "gitinfo")
	{
		m_GitInfo->Leallas();
		delete m_GitInfo;
		Log.Debug("Vezerlo", "GitInfo reload...");
		m_GitInfo = new GitInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);
		return "GitInfo ujraindult.";
	}
	else if(nev == "hginfo")
	{
		m_HgInfo->Leallas();
		delete m_HgInfo;
		Log.Debug("Vezerlo", "HgInfo reload...");
		m_HgInfo = new HgInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);
		return "HgInfo ujraindult.";
	}
	else if(nev == "console")
	{
		m_Console->Leallas();
		delete m_Console;
		Log.Debug("Vezerlo", "Console reload...");
		m_Console = new Console(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);
		return "Console ujraindult.";
	}
	else if(nev == "ircsession")
	{
		sIRCSession.Leallas();
		delete IRCSession::getSingletonPtr();
		Log.Debug("Vezerlo", "IRCSession reload...");
		m_IRCSession = new IRCSession(m_server, m_port, _mysql[0], _mysql[1], _mysql[2], _mysql[3]);
		return "IRCSession ujraindult.";
	}
	else if(nev == "all")
	{
		m_SvnInfo->Leallas();
		delete m_SvnInfo;
		m_GitInfo->Leallas();
		delete m_GitInfo;
		m_HgInfo->Leallas();
		delete m_HgInfo;
		m_Console->Leallas();
		delete m_Console;
		sIRCSession.Leallas();
		delete IRCSession::getSingletonPtr();

		Log.Debug("Vezerlo", "SvnInfo reload...");
		m_SvnInfo = new SvnInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

		Log.Debug("Vezerlo", "GitInfo reload...");
		m_GitInfo = new GitInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

		Log.Debug("Vezerlo", "HgInfo reload...");
		m_HgInfo = new HgInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

		Log.Debug("Vezerlo", "Console reload...");
		m_Console = new Console(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

		Log.Debug("Vezerlo", "IRCSession reload...");
		m_IRCSession = new IRCSession(m_server, m_port, _mysql[0], _mysql[1], _mysql[2], _mysql[3]);
		return "Minden ujraindult.";
	}

	return "Nincs ilyen resz.";
}

void Vezerlo::Leallas()
{
	printf("\n");
	Log.Notice("Vezerlo", "Leallasi folyamat elindult.");

	Uptime();
	m_SvnInfo->Leallas();
	m_GitInfo->Leallas();
	m_HgInfo->Leallas();
	m_Console->Leallas();
	sIRCSession.Leallas();

	Log.Success("Vezerlo", "Leallas befejezodot.");
}
