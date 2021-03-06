/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2008 Valroft <http://www.aspiredev.org/>
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

#include "../../StdAfx.h"

//initialiseSingleton(Socket);

Socket::Socket()
{
	m_fd = NULL;
	m_sendCount = NULL;
	m_sendPerPeriod = Config.MainConfig.GetIntDefault("IRC", "LinesPerSecond", 3);
	m_lastSendTime = time(NULL);
}

Socket::~Socket()
{
#ifdef _DEBUG_MOD
	Log.Notice("Socket", "~Socket()");
#endif
}

void Socket::UpdateQueue()
{
	time_t nt = time(NULL);
	uint32 c = 0;
	if(m_lastSendTime == nt)
		return;

	m_lastSendTime = nt;
	m_buffer_mutex.Acquire();

	while(m_sendQueue.size())
	{
		c++;
		m_outBuf.append(m_sendQueue.front().c_str(), m_sendQueue.front().length());
		m_sendQueue.pop_front();
		if(c >= m_sendPerPeriod)
			break;
	}

	m_buffer_mutex.Release();
	m_sendCount = c;
}

void Socket::SendLine(string line)
{
	if(m_sendCount >= m_sendPerPeriod)
	{
		m_sendQueue.push_back(line);
		return;
	}

	m_buffer_mutex.Acquire();
	m_outBuf.append(line.c_str(), line.length());
	m_buffer_mutex.Release();
	m_sendCount++;
}

void Socket::SendForcedLine(string line)
{
	m_buffer_mutex.Acquire();
	m_outBuf.append(line.c_str(), line.length());
	m_buffer_mutex.Release();
}

bool Socket::HasLine()
{
	bool ret = false;

	m_buffer_mutex.Acquire();
	if(m_inBuf.find("\n") != string::npos)
		ret = true;

	m_buffer_mutex.Release();
	return ret;
}

string Socket::GetLine()
{
	// can probably be optimized /lazy

	string ret;
	char c;

	m_buffer_mutex.Acquire();

	for(;;)
	{
		c = m_inBuf[0];
		m_inBuf.erase(0, 1);

		if(c == '\n')
			break;

		ret.append(&c, 1);
	}

	m_buffer_mutex.Release();
	return ret;
}

bool Socket::Connect(string host, uint32 port)
{
	if(m_fd) // We already have an existing socket
		return false;

	// Populate the socket handle
	m_fd = SocketOps::CreateTCPFileDescriptor();

	struct hostent * ci = gethostbyname(host.c_str());
	if(ci == NULL)
		return false;

	m_client.sin_family = ci->h_addrtype;
	m_client.sin_port = ntohs((u_short)port);
	memcpy(&m_client.sin_addr.s_addr, ci->h_addr_list[0], ci->h_length);

	SocketOps::Blocking(m_fd);

	if(connect(m_fd, (const sockaddr*)&m_client, sizeof(m_client)) == -1)
	{
		m_fd = NULL;
		return false;
	}

	SocketOps::Blocking(m_fd);

	pSocketMgr = new SocketMgr(cast_default(SocketPointer, shared_from_this()));
	pSocketMgr->AddSocket(cast_default(SocketPointer, shared_from_this()));
	ThreadPool.ExecuteTask(pSocketMgr);

	return true;
}

bool Socket::SocketServer(uint32 port, int connections)
{
	m_fd = SocketOps::CreateTCPFileDescriptor();
	SocketOps::Nonblocking(m_fd);

	m_client.sin_family = AF_INET;
	m_client.sin_port = ntohs((u_short)port);
	m_client.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = ::bind(m_fd, (const sockaddr*)&m_client, sizeof(m_client));
	if(ret != 0)
	{
		Log.Error("Socket", "Bind unsuccessful on port %u.", port);
		return false;
	}

	ret = listen(m_fd, connections);
	if(ret != 0) 
	{
		Log.Error("Socket", "Unable to listen on port %u.", port);
		return false;
	}

	pSocketMgr = new SocketMgr(cast_default(SocketPointer, shared_from_this()));
	pSocketMgr->AddSocket(cast_default(SocketPointer, shared_from_this()));
	ThreadPool.ExecuteTask(pSocketMgr);

	return true;
}

string Socket::GetRemoteIP()
{
	char* ip = cast_default(char*, inet_ntoa(m_client.sin_addr));
	if(ip != NULL)
		return string(ip);
	else
		return string("noip");

	return string("");
}

void Socket::Disconnect()
{
	SocketOps::CloseSocket(m_fd);
	m_fd = NULL;

	pSocketMgr->RemoveSocket(cast_default(SocketPointer, shared_from_this()));
	Sleep(1500);
}

void Socket::Leallas()
{
	pSocketMgr->OnShutdown();
	Log.Notice("Socket", "Socket leallt.");
}
