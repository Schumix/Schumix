/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2008 Valroft <http://www.aspiredev.org/>
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

#ifndef _SIMPLE_SOCKET_H
#define _SIMPLE_SOCKET_H

#if PLATFORM == PLATFORM_LINUX
#include <sys/ioctl.h>
#include <sys/socket.h>
#define SOCKET int
#define SD_BOTH SHUT_RDWR
#endif

class IRCSession;
class SocketMgr;

class SimpleSocket : public boost::enable_shared_from_this<SimpleSocket>, public Singleton<SimpleSocket>
{
	friend class SocketMgr;
public:
	SimpleSocket();
	~SimpleSocket();

	bool Connect(string host, uint32 port);
	void Disconnect();
	bool IsConnected() { return m_fd != NULL; }

	void SendLine(string line);
	void SendForcedLine(string line);

	bool HasLine();
	string GetLine();
	void UpdateQueue();
	bool Running() { return m_running; }

protected:
	bool m_full;
	string buffer;
	SOCKET m_fd;
	sockaddr_in m_client;

	Mutex m_buffer_mutex;
	string m_outBuf;
	string m_inBuf;

	deque<string> m_sendQueue;
	uint32 m_sendPerPeriod;
	uint32 m_sendCount;
	time_t m_lastSendTime;

	volatile bool m_running;

private:
	SocketMgr* pSocketMgr;
};

#define sSimpleSocket SimpleSocket::getSingleton()

#endif
