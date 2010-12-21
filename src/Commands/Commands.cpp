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

initialiseSingleton(Commands);

Commands::Commands()
{
	Log.Notice("Commands", "Commands elindult.");

	Log.Debug("Commands", "CommandMgr indul...");
	m_CommandMgr = CommandMgrPointer(new CommandMgr());
}

Commands::~Commands()
{
#ifdef _DEBUG_MOD
	Log.Notice("Commands", "~Commands()");
#endif
}

bool Commands::Admin(string nick)
{
	transform(nick.begin(), nick.end(), nick.begin(), ::tolower);

	QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT * FROM adminok WHERE nev = '%s'", nick.c_str());
	if(db)
		return true;

	return false;
}

bool Commands::Admin(string nick, AdminFlag Flag)
{
	transform(nick.begin(), nick.end(), nick.begin(), ::tolower);

	QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT flag FROM adminok WHERE nev = '%s'", nick.c_str());
	if(db)
	{
		int flag = cast_int(db->Fetch()[0].GetUInt8());

		if(Flag != flag)
			return false;

		return true;
	}

	return false;
}

bool Commands::Admin(string nick, string Vhost, AdminFlag Flag)
{
	transform(nick.begin(), nick.end(), nick.begin(), ::tolower);

	QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT vhost, flag FROM adminok WHERE nev = '%s'", nick.c_str());
	if(db)
	{
		string vhost = db->Fetch()[0].GetString();

		if(Vhost != vhost)
			return false;

		int flag = cast_int(db->Fetch()[1].GetUInt8());

		if(flag == 1 && Flag == NULL)
			return true;

		if(Flag != flag)
			return false;

		return true;
	}

	return false;
}
