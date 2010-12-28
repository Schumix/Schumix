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

void IRCSession::HandlePrivmsg(IRCMessage& recvData)
{
	if(sConsole.GetConsoleLog())
	{
		Log.Color(TYELLOW);
		printf("[%s] <%s> %s%s", recvData.GetChannel(), recvData.GetNick(), recvData.GetArgs(), NEWLINE);
		Log.Color(TNORMAL);
	}

	// Log
	Logfajl(recvData);

	if(FSelect(PARANCSOK) == bekapcsol)
	{
		if(FSelectChannel(PARANCSOK, recvData.Channel) != bekapcsol && cast_int(recvData.Channel.find("#")) != string::npos)
			return;

		if(AutoKick(recvData, "privmsg"))
			return;

		if(FSelect(MODE) == bekapcsol && FSelectChannel(MODE, recvData.Channel) == bekapcsol)
		{
			AutoMode = true;
			ModeChannel = recvData.Channel;
			WriteLine("NICKSERV STATUS %s", recvData.GetNick());
		}
#ifdef _DEBUG_MOD
		else
			Log.Warning("Funkcio", "A %s funkcio nem uzemel!", MODE);
#endif

		// Hl üzenet
		HLUzenet(recvData);

		// Schumix
		Schumix(recvData);

		// Parancs előjel
		if(recvData.Args.substr(0, 1) != m_ParancsElojel)
			return;

		sCommandMgr.BejovoInfo(recvData);
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", PARANCSOK);
#endif
}
