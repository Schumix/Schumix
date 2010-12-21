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

#include "../StdAfx.h"

initialiseSingleton(Console);

Console::Console()
{
	m_running = true;
	Thread t(&RunUpdateProc, this);
	ConsoleLog = Config.MainConfig.GetBoolDefault("Log", "Irclog", false);

	Log.Notice("Console", "Console elindult.");
	printf("\n");
}

Console::~Console()
{
#ifdef _DEBUG_MOD
	Log.Notice("Console", "~Console()");
#endif
}

void Console::ReadConsoleRoutine()
{
	Sleep(1000);

	while(Running())
	{
		if(!Running())
			break;

		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT irc_cim FROM schumix WHERE entry = '1'");
		if(db)
		{
			char* txt = new char[2048];
			fgets(txt, 2048, stdin);

			if(ConsoleCommands(txt))
			{
				delete[] txt;
				continue;
			}

			string irc_cim = db->Fetch()[0].GetString();
			sIRCSession.SendChatMessage(PRIVMSG, irc_cim.c_str(), "%s", txt);

			delete[] txt;
			Sleep(200);
		}
	}

#ifdef _DEBUG_MOD
	Log.Error("Console", "Console thread leallt.");
#endif
	ThreadExit(0);
}

void Console::Leallas()
{
	m_running = false;
	Log.Notice("Console", "Console leallt.");
}

Thread_void Console::RunUpdateProc(void* smg)
{
	cast_default(Console*, smg)->ReadConsoleRoutine();
	return NULL;
}
