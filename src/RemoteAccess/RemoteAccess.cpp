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

initialiseSingleton(RemoteAccess);

RemoteAccess::RemoteAccess(uint32 port, int connections)
{
	Log.Notice("RemoteAccess", "RemoteAccess elindult.");

	m_Port = port;
	m_Connections = connections;
	m_running = true;

	//m_LastPing = getMSTime();
	InitHandler();

	Log.Debug("RemoteAccess", "Update Thread indul...");
	Thread t(&RunUpdateProc, this);
}

RemoteAccess::~RemoteAccess()
{
#ifdef _DEBUG_MOD
	Log.Notice("RemoteAccess", "~RemoteAccess()");
#endif
}

void RemoteAccess::InitHandler()
{
	RegisterHandler("teszt",    cast_default(RACallback, &RemoteAccess::HandleTeszt));

	Log.Notice("RemoteAccess", "Osszes RemoteAccess handler regisztralva.");
}

void RemoteAccess::RegisterHandler(string code, RACallback method)
{
	MessageHandlerMap.insert(make_pair(code, method));
}

void RemoteAccess::BejovoInfo(string SInfo)
{
	RAMessage ra;
	ra.Minden = SInfo;
	printf("%s\n", SInfo.c_str());

	int szokoz = ra.Minden.find(' ');
	if(szokoz == string::npos)
		return;

	ra.Opcode = ra.Minden.substr(0, szokoz);
	ra.Args = ra.Minden.substr(szokoz+1);

	map<string, RACallback>::iterator itr = MessageHandlerMap.find(ra.Opcode);
	if(itr == MessageHandlerMap.end())
		return;

	RACallback cb = itr->second;
	(this->*cb)(ra);
}

void RemoteAccess::Update()
{
	Log.Success("RemoteAccess", "Update Thread elindult.");
	m_Socket = SocketPointer(new Socket());

	if(!m_Socket->SocketServer(m_Port, m_Connections))
	{
		Log.Error("RemoteAccess", "Sikertelen a Socket Szerver inditasa.");
		Sleep(5000);
		return;
	}
	else
		Log.Success("RemoteAccess", "Sikeres a Socket Szerver inditasa.");

	while(Running())
	{
		if(!Running())
			break;

		if(m_Socket->HasLine())
		{
			printf("0\n");
			string info = m_Socket->GetLine();
			BejovoInfo(info);
			printf("1\n");
		}

		/*if(getMSTime() - m_LastPing >= 15000)
		{
			WriteLine("PING :%s", m_Host.c_str());
			m_LastPing = getMSTime();
		}*/

		m_Socket->UpdateQueue();

		Sleep(100);
	}
}

Thread_void RemoteAccess::RunUpdateProc(void* smg)
{
	cast_default(RemoteAccess*, smg)->Update();
	return NULL;
}

void RemoteAccess::WriteLine(const char* format, ...)
{
	m_mutex.Acquire();
	char* obuf = new char[65536];
	va_list ap;

	va_start(ap, format);
	vsnprintf(obuf, 65536, format, ap);
	va_end(ap);

	if(*obuf == '\0')
	{
		delete[] obuf;
		return;
	}

	string send = string(obuf) + NEWLINE;
	delete[] obuf;

	m_Socket->SendLine(send);
	m_mutex.Release();
}

void RemoteAccess::WriteLineForce(const char* format, ...)
{
	m_mutex.Acquire();
	char* obuf = new char[65536];
	va_list ap;

	va_start(ap, format);
	vsnprintf(obuf, 65536, format, ap);
	va_end(ap);

	if(*obuf == '\0')
	{
		delete[] obuf;
		return;
	}

	string send = string(obuf) + NEWLINE;
	delete[] obuf;

	m_Socket->SendForcedLine(send);
	m_mutex.Release();
}

void RemoteAccess::Leallas()
{
	m_running = false;
	Log.Notice("RemoteAccess", "RemoteAccess leallt.");
}
