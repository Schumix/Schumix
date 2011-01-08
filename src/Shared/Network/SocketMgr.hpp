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

#ifndef _SCHUMIX_SOCKET_MGR_HPP
#define _SCHUMIX_SOCKET_MGR_HPP

class Socket;

class SocketMgr
{
public:
	SocketMgr(SocketPointer session);
	~SocketMgr();

	void AddSocket(SocketPointer pSocket);
	void RemoveSocket(SocketPointer pSocket);

protected:
	void Update();
	static Thread_void RunUpdateProc(void* smg);

	typedef set<SocketPointer> SocketSet;
	Mutex m_mutex;
	SocketSet m_sockets;

private:
        SocketPointer m_session;
};

#endif
