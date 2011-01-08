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

#include "../StdAfx.h"

initialiseSingleton(Vezerlo);

Vezerlo::Vezerlo()
{
	m_crash = true;
	m_Indulas = true;
	m_running = true;

	// uptime
	UNIXTIME = time(NULL);
	m_StartTime = cast_uint32(UNIXTIME);
	//ThreadPool.Startup();

	// Irc szerver conf
	m_server[0] = Config.MainConfig.GetStringDefault("IRC", "Server", "");
	m_port[0] = Config.MainConfig.GetIntDefault("IRC", "Port", 6667);

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
		fprintf(f, "%u", pid);
		fclose(f);
	}

	_HookSignals();

	Log.Debug("Vezerlo", "SvnInfo indul...");
	m_SvnInfo = new SvnInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

	Log.Debug("Vezerlo", "GitInfo indul...");
	m_GitInfo = new GitInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

	Log.Debug("Vezerlo", "HgInfo indul...");
	m_HgInfo = new HgInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

	Log.Debug("Vezerlo", "Console indul...");
	m_Console = new Console();
	ThreadPool.ExecuteTask(m_Console);

	Log.Debug("Vezerlo", "RemoteAccess indul...");
	m_RemoteAccess = new RemoteAccess(6000, 1);
	ThreadPool.ExecuteTask(m_RemoteAccess);

	Log.Debug("Vezerlo", "IRCSession indul...");
	m_IRCSession = new IRCSession(m_server[0], m_port[0]);
	ThreadPool.ExecuteTask(m_IRCSession);

	while(Running())
	{
		if(!Running())
			break;

		Sleep(1000);
	}

	_UnhookSignals();

	// delete pid fájl
	remove(pidfajl.c_str());
	delete this;
}

Vezerlo::~Vezerlo()
{
#ifdef _DEBUG_MOD
	Log.Notice("Vezerlo", "~Vezerlo()");
#endif
	delete m_SvnInfo;
	delete m_GitInfo;
	delete m_HgInfo;
	delete m_Console;
	delete m_IRCSession;
}

void _OnSignal(int s)
{
	switch(s)
	{
#if PLATFORM == PLATFORM_LINUX
	case SIGHUP:
		break;
#endif
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
#if PLATFORM == PLATFORM_WINDOWS
	case SIGBREAK:
#endif
		if(IRCSession::getSingletonPtr() != 0)
		{
			if(sVezerlo.m_crash)
			{
				Log.Notice("Crash", "Uptime mentese elindult!");
				sVezerlo.Uptime();
				sVezerlo.m_crash = false;
				Log.Notice("Crash", "Mentes kesz.");
				sVezerlo.Leallas();
			}
		}

		remove(format("%s.pid", Elnevezes).c_str());
		break;
	}

	signal(s, _OnSignal);
}

#if PLATFORM == PLATFORM_LINUX
void segfault_handler(int c)
{
	Log.Warning("Crash", "Segfault handler elindult...");

	try
	{
		if(Vezerlo::getSingletonPtr() != 0)
		{
			Log.Notice("Crash", "Uptime mentese elindult!");
			sVezerlo.Uptime();

			if(IRCSession::getSingletonPtr() != 0)
			{
				sIRCSession.WriteLine("QUIT :Crash: segfault!");
				Sleep(1000);
			}

			sVezerlo.m_crash = false;
			Log.Notice("Crash", "Mentes kesz.");
			sVezerlo.Leallas();
		}
	}
	catch(...)
	{
		Log.Error("Crash", "Kivetel keletkezet mentes kozben ezert nincs mentes!");
	}

	Log.Notice("Crash", "Program leallhat...");
	abort();
}
#endif

void Vezerlo::_HookSignals()
{
	Log.Notice("Vezerlo","Hooking signals...");
	signal(SIGINT, _OnSignal);
	signal(SIGTERM, _OnSignal);
	signal(SIGABRT, _OnSignal);
#if PLATFORM == PLATFORM_WINDOWS
	signal(SIGBREAK, _OnSignal);
#else
	signal(SIGHUP, _OnSignal);
	signal(SIGUSR1, _OnSignal);

	// crash handler
	signal(SIGSEGV, segfault_handler);
	signal(SIGFPE, segfault_handler);
	signal(SIGILL, segfault_handler);
	signal(SIGBUS, segfault_handler);
#endif
}

void Vezerlo::_UnhookSignals()
{
	Log.Notice("Vezerlo","Unhooking signals...");
	signal(SIGINT, 0);
	signal(SIGTERM, 0);
	signal(SIGABRT, 0);
#if PLATFORM == PLATFORM_WINDOWS
	signal(SIGBREAK, 0);
#else
	signal(SIGHUP, 0);
#endif
}

string Vezerlo::GetUptimeString()
{
	UNIXTIME = time(NULL);
	time_t pTime = cast_default(time_t, UNIXTIME) - m_StartTime;
	tm* tmv = gmtime(&pTime);

	return format("%u nap, %u óra, %u perc, %u másodperc.", tmv->tm_yday, tmv->tm_hour, tmv->tm_min, tmv->tm_sec);
}

void Vezerlo::Uptime()
{
	string datum;
	int honap = Honap();
	int nap = Nap();

	if(honap < 10)
	{
		if(nap < 10)
			datum = format("%i. 0%i. 0%i. %i:%i", Ev(), honap, nap, Ora(), Perc());
		else
			datum = format("%i. 0%i. %i. %i:%i", Ev(), honap, nap, Ora(), Perc());
	}
	else
	{
		if(nap < 10)
			datum = format("%i. %i. 0%i. %i:%i", Ev(), honap, nap, Ora(), Perc());
		else
			datum = format("%i. %i. %i. %i:%i", Ev(), honap, nap, Ora(), Perc());
	}

#if PLATFORM == PLATFORM_WINDOWS
	m_SQLConn->Query("INSERT INTO `uptime`(datum, uptime, memory) VALUES ('%s', '%s', '%f MB')", datum.c_str(), GetUptimeString().c_str(), MemoryInfo(GetCurrentProcessId()/1024)/1024);
#else
	m_SQLConn->Query("INSERT INTO `uptime`(datum, uptime, memory) VALUES ('%s', '%s', '%f MB')", datum.c_str(), GetUptimeString().c_str(), MemoryInfo()/1000*0.3762);
#endif
}

void Vezerlo::IndulasiIdo()
{
	printf("\n");
	ThreadPool.ShowStats();
	UNIXTIME = time(NULL);
	time_t pTime = cast_default(time_t, UNIXTIME) - m_StartTime;
	tm* tmv = gmtime(&pTime);

	Log.Debug("Vezerlo", "A program %ums alatt indult el.", cast_uint32(tmv->tm_sec*1000));
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
		if(c[i] == 'á')
		{
			escaped.append("%C3%A1");
			continue;
		}
		else if(c[i] == 'Á')
		{
			escaped.append("%C3%81");
			continue;
		}
		else if(c[i] == 'ä')
		{
			escaped.append("%C3%A4");
			continue;
		}
		else if(c[i] == 'Ä')
		{
			escaped.append("%C3%84");
			continue;
		}
		else if(c[i] == 'é')
		{
			escaped.append("%C3%A9");
			continue;
		}
		else if(c[i] == 'É')
		{
			escaped.append("%C3%89");
			continue;
		}
		else if(c[i] == 'í')
		{
			escaped.append("%C3%AD");
			continue;
		}
		else if(c[i] == 'Í')
		{
			escaped.append("%C3%8D");
			continue;
		}
		else if(c[i] == 'ó')
		{
			escaped.append("%C3%B3");
			continue;
		}
		else if(c[i] == 'Ó')
		{
			escaped.append("%C3%93");
			continue;
		}
		else if(c[i] == 'ö')
		{
			escaped.append("%C3%B6");
			continue;
		}
		else if(c[i] == 'Ö')
		{
			escaped.append("%C3%96");
			continue;
		}
		else if(c[i] == 'õ')
		{
			escaped.append("%C5%91");
			continue;
		}
		else if(c[i] == 'Õ')
		{
			escaped.append("%C5%90");
			continue;
		}
		else if(c[i] == 'ü')
		{
			escaped.append("%C3%BC");
			continue;
		}
		else if(c[i] == 'Ü')
		{
			escaped.append("%C3%9C");
			continue;
		}
		else if(c[i] == 'û')
		{
			escaped.append("%C5%B1");
			continue;
		}
		else if(c[i] == 'Û')
		{
			escaped.append("%C5%B0");
			continue;
		}
		else if(c[i] == 'ú')
		{
			escaped.append("%C3%BA");
			continue;
		}
		else if(c[i] == 'Ú')
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

			if(res[i].substr(0, 5) == "#225;")	// Kisbetük
			{
				res[i].erase(0, 5);
				escaped.append("á");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#233;")
			{
				res[i].erase(0, 5);
				escaped.append("é");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#237;")
			{
				res[i].erase(0, 5);
				escaped.append("í");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#243;")
			{
				res[i].erase(0, 5);
				escaped.append("ó");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#246;")
			{
				res[i].erase(0, 5);
				escaped.append("ö");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#337;")
			{
				res[i].erase(0, 5);
				escaped.append("õ");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#250;")
			{
				res[i].erase(0, 5);
				escaped.append("ú");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#252;")
			{
				res[i].erase(0, 5);
				escaped.append("ü");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#369;")
			{
				res[i].erase(0, 5);
				escaped.append("û");
				escaped.append(res[i]);
				continue;
			}
			if(res[i].substr(0, 5) == "#193;")	// Nagybetük
			{
				res[i].erase(0, 5);
				escaped.append("Á");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#201;")
			{
				res[i].erase(0, 5);
				escaped.append("É");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#205;")
			{
				res[i].erase(0, 5);
				escaped.append("Í");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#211;")
			{
				res[i].erase(0, 5);
				escaped.append("Ó");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#214;")
			{
				res[i].erase(0, 5);
				escaped.append("Ö");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#336;")
			{
				res[i].erase(0, 5);
				escaped.append("Õ");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#218;")
			{
				res[i].erase(0, 5);
				escaped.append("Ú");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#220;")
			{
				res[i].erase(0, 5);
				escaped.append("Ü");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#368;")
			{
				res[i].erase(0, 5);
				escaped.append("Û");
				escaped.append(res[i]);
				continue;
			}
			else if(res[i].substr(0, 5) == "#180;")	// Egyéb karakterek
			{
				res[i].erase(0, 5);
				escaped.append("´");
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

string Vezerlo::Sha1(string Sha1)
{
	Sha1Hash sha1;
	unsigned char* eredmeny = new unsigned char[SHA_DIGEST_LENGTH+1];

	sha1.Initialize();
	sha1.UpdateData(Sha1);
	sha1.Finalize();
	memcpy(eredmeny, sha1.GetDigest(), SHA_DIGEST_LENGTH);

	stringstream ss;
	const size_t len = 20;

	for(size_t u = 0; u < len; ++u)
		ss << std::hex << std::setw(2) << std::setfill('0') << cast_int(eredmeny[u]);

	string hash;
	ss >> hash;

	return hash;
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
		m_Console->OnShutdown();
		delete m_Console;
		Log.Debug("Vezerlo", "Console reload...");
		m_Console = new Console();
		return "Console ujraindult.";
	}
	else if(nev == "ircsession")
	{
		/*sIRCSession.OnShutdown();
		if(IRCSession::getSingletonPtr() != 0)
			delete IRCSession::getSingletonPtr();

		Log.Debug("Vezerlo", "IRCSession reload...");
		m_IRCSession = new IRCSession(m_server[0], m_port[0]);
		return "IRCSession ujraindult.";*/
		return "javitas alatt";
	}
	else if(nev == "all")
	{
		m_SvnInfo->Leallas();
		delete m_SvnInfo;
		m_GitInfo->Leallas();
		delete m_GitInfo;
		m_HgInfo->Leallas();
		delete m_HgInfo;
		m_Console->OnShutdown();
		delete m_Console;
		m_IRCSession->OnShutdown();

		//if(IRCSession::getSingletonPtr() != 0)
			//delete IRCSession::getSingletonPtr(); javítás alatt

		Log.Debug("Vezerlo", "SvnInfo reload...");
		m_SvnInfo = new SvnInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

		Log.Debug("Vezerlo", "GitInfo reload...");
		m_GitInfo = new GitInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

		Log.Debug("Vezerlo", "HgInfo reload...");
		m_HgInfo = new HgInfo(_mysql[0], _mysql[1], _mysql[2], _mysql[3]);

		Log.Debug("Vezerlo", "Console reload...");
		m_Console = new Console();

		/*Log.Debug("Vezerlo", "IRCSession reload...");
		m_IRCSession = new IRCSession(m_server[0], m_port[0]);*/
		return "Minden ujraindult.";
	}

	return "Nincs ilyen resz.";
}

void Vezerlo::Leallas()
{
	printf("\n");
	Log.Notice("Vezerlo", "Leallasi folyamat elindult.");

	if(m_crash)
		Uptime();

	m_SvnInfo->Leallas();
	m_GitInfo->Leallas();
	m_HgInfo->Leallas();
	m_Console->OnShutdown();
	m_RemoteAccess->OnShutdown();
	m_IRCSession->OnShutdown();

	Log.Notice("ThreadPool", "Ending %u active threads...", ThreadPool.GetActiveThreadCount());
	ThreadPool.Shutdown();

	Log.Success("Vezerlo", "Leallas befejezodot.");
	m_running = false;
}
