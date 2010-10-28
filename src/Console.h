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

#ifndef _CONSOL_H
#define _CONSOL_H

class MySQLConnection;
class IRCSession;

class Console : public Singleton<Console>
{
public:
	Console(string host, string user, string password, string database);
	~Console();

	inline string GetConsolLog() { return ConsolLog; }
	// Class leállása
	void Leallas();

protected:
	// Konzol irás
	void ReadConsoleRoutine();
	//Konzol parancsok
	bool ConsoleCommands(char* adat);
	static Thread_void RunUpdateProc(void* smg);

	bool Running() { return m_running; }
	volatile bool m_running;

	// Konzol irás állapota
	string ConsolLog;

private:
	// Mysql kapcsolat.
	MySQLConnectionPointer m_SQLConn;
};

#define sConsole Console::getSingleton()

#endif
