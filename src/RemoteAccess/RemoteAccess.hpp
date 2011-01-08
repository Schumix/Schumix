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

#ifndef _SCHUMIX_REMOTE_ACCESS_H
#define _SCHUMIX_REMOTE_ACCESS_H

struct RAMessage
{
	string Opcode;		// Opcode elnevezése vagy száma
	string Args;		// Szöveg ami bejön
	string Minden;		// Minden

	// .c_str() hozzáadva hogy const char* legyen
	inline const char* GetOpcode() { return Opcode.c_str(); }
	inline const char* GetArgs() { return Args.c_str(); }
	inline const char* GetMinden() { return Minden.c_str(); }
};

class RemoteAccess;
typedef void(RemoteAccess::*RACallback)(RAMessage& recvData);

class RemoteAccess : public Singleton<RemoteAccess>, public ThreadContext
{
public:
	RemoteAccess(uint32 port, int connections);
	~RemoteAccess();

	/*
	 * Kifelé induló fv-k melyek a szerver felé küldik az adatott. Bele van épitve a "\n".
	 */
	void WriteLine(const char* format, ...);
	void WriteLineForce(const char* format, ...);

	bool Run();
	// Class leállása
	void OnShutdown();

protected:
	static Thread_void RunUpdateProc(void* smg);
	void BejovoInfo(string SInfo);

	void InitHandler();
	void RegisterHandler(string code, RACallback method);

	void HandleTeszt(RAMessage& recvData);

	uint32 m_Port;
	int m_Connections;
	uint32 m_LastPing;

private:
	Mutex m_mutex;
	// Socket kapcsolat.
	SocketPointer m_Socket;
	map<string, RACallback> MessageHandlerMap;
};

#define sRemoteAccess RemoteAccess::getSingleton()

#endif
