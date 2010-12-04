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

#include "../../StdAfx.h"

SocketMgr::SocketMgr()
{
	Log.Debug("SocketMgr", "SocketMgr indul...");
	Thread t(&RunUpdateProc, this);
}

SocketMgr::~SocketMgr()
{
#ifdef _DEBUG_MOD
	Log.Notice("SocketMgr", "~SocketMgr()");
#endif
}

Thread_void SocketMgr::RunUpdateProc(void* smg)
{
	cast_default(SocketMgr*, smg)->Update();
	return NULL;
}

void SocketMgr::AddSocket(SocketPointer pSocket)
{
	m_mutex.Acquire();
	m_sockets.insert(pSocket);
	m_mutex.Release();
}

void SocketMgr::RemoveSocket(SocketPointer pSocket)
{
	m_mutex.Acquire();
	m_sockets.erase(pSocket);
	m_mutex.Release();
}

void SocketMgr::Update()
{
	fd_set read_set;
	fd_set write_set;
	fd_set exception_set;
	set<SocketPointer>::iterator itr;
	SocketPointer s;
	int res;
	int max_fd = NULL;
	char buffer[65000];

	timeval tv;
	tv.tv_sec = NULL;
	tv.tv_usec = 10000; // 10ms
	
	Log.Notice("SocketMgr", "SocketMgr elindult.");

	while(sSocket.Running())
	{
		if(!sSocket.Running())
			break;

		FD_ZERO(&read_set);
		FD_ZERO(&write_set);
		FD_ZERO(&exception_set);

		m_mutex.Acquire();
		for(itr = m_sockets.begin(); itr != m_sockets.end(); ++itr)
		{
#if PLATFORM == PLATFORM_WINDOWS
			if(cast_int((*itr)->m_fd) >= max_fd)
				max_fd = cast_int((*itr)->m_fd) + 1;
#else
			if((*itr)->m_fd >= max_fd)
				max_fd = (*itr)->m_fd + 1;
#endif

			if((*itr)->m_outBuf.size() > NULL)
				FD_SET((*itr)->m_fd, &write_set);
			else
				FD_SET((*itr)->m_fd, &read_set);
		}
		m_mutex.Release();
		
		res = select(max_fd, &read_set, &write_set, &exception_set, &tv);

		m_mutex.Acquire();
		for(itr = m_sockets.begin(); itr != m_sockets.end();)
		{
			s = *itr;
			++itr;

			if(FD_ISSET(s->m_fd, &exception_set))
			{
				s->m_fd = NULL;
				continue;
			}

			if(FD_ISSET(s->m_fd, &read_set))
			{
				res = recv(s->m_fd, buffer, 65000, 0);
				if(res <= NULL)
				{
					s->m_fd = NULL;
					continue;
				}

				s->m_inBuf.append(buffer, res);
			}

			if(FD_ISSET(s->m_fd, &write_set) && s->m_outBuf.size())
			{
				res = send(s->m_fd, s->m_outBuf.c_str(), s->m_outBuf.size(), 0);
				if(res <= NULL)
				{
					s->m_fd = NULL;
					continue;
				}

				s->m_outBuf.erase(0, res);
			}
		}
		m_mutex.Release();

		Sleep(100);
	}

	Log.Warning("SocketMgr", "SocketMgr leallt.");
	ThreadExit(0);
}
