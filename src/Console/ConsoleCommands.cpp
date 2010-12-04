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

bool Console::ConsoleCommands(char* adat)
{
	string info = string(adat);
	int szokoz = info.find("\n");
	string iras = info.substr(0, szokoz);

	vector<string> res(1);
	split(iras, " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return false;
	}

	string cmd = res[1];

	if(cmd == "help")
	{
		Log.Notice("Console", "Prarancsok: connect, disconnect, reconnect, funkcio, kikapcs");
		Log.Notice("Console", "szoba, konzollog, admin, reload");

		res.clear();
		return true;
	}

	if(cmd == "connect")
	{
		if(!sIRCSession.GetSocket()->Connect(sIRCSession.GetHost(), sIRCSession.GetPort()))
		{
			Log.Error("Console", "Kapcsolodas ide: %s sikertelen.", sIRCSession.GetHost().c_str());
			sIRCSession.SocketDisconnect();
		}
		else
		{
			Log.Success("Console", "Kapcsolat letrehozva ide: %s", sIRCSession.GetHost().c_str());
			sIRCSession.m_ConnState = CONN_CONNECTED;

			if(sIRCSession.m_ConnState == CONN_CONNECTED)
			{
				Sleep(1000);
				sIRCSession.WriteLine("NICK %s", sIRCSession.GetNickTarolo().c_str());
				sIRCSession.WriteLine("USER %s 8 * :%s", sIRCSession.GetUserName().c_str(), sIRCSession.GetUserName().c_str());
				Log.Notice("Console", "Nick %s", sIRCSession.GetNickTarolo().c_str());
				printf("\n");
				Log.Notice("Console", "Komunikacio az irc szerverrel megindult.");
			}
		}

		res.clear();
		return true;
	}

	if(cmd == "disconnect")
	{
		sIRCSession.WriteLine("QUIT :Console: disconnect.");
		Sleep(1000);
		sIRCSession.SocketDisconnect();
		Log.Notice("Console", "Kapcsolat bontva");

		res.clear();
		return true;
	}

	if(cmd == "reconnect")
	{
		sIRCSession.WriteLine("QUIT :Console: reconnect.");
		Sleep(1000);
		sIRCSession.SocketDisconnect();
		Log.Notice("Console", "Kapcsolat bontva");

		if(!sIRCSession.GetSocket()->Connect(sIRCSession.GetHost(), sIRCSession.GetPort()))
		{
			Log.Error("Console", "Ujrakapcsolodas sikertelen ide: %s", sIRCSession.GetHost().c_str());
			sIRCSession.SocketDisconnect();
		}
		else
		{
			Log.Success("Console", "Ujrakapcsolodva ide: %s", sIRCSession.GetHost().c_str());
			sIRCSession.m_ConnState = CONN_CONNECTED;

			if(sIRCSession.m_ConnState == CONN_CONNECTED)
			{
				Sleep(1000);
				sIRCSession.WriteLine("NICK %s", sIRCSession.GetNickTarolo().c_str());
				sIRCSession.WriteLine("USER %s 8 * :%s", sIRCSession.GetUserName().c_str(), sIRCSession.GetUserName().c_str());
				Log.Notice("Console", "Nick %s", sIRCSession.GetNickTarolo().c_str());
				printf("\n");
				Log.Notice("Console", "Komunikacio az irc szerverrel megindult.");
			}
		}

		res.clear();
		return true;
	}

	if(cmd == "funkcio")
	{
		if(res.size() < 3)
		{
			Log.Notice("Console", "Nincs megadva az 1. parameter!");
			res.clear();
			return false;
		}

		if(res[2] == INFO)
		{
			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT funkcio_nev, funkcio_status FROM schumix");
			if(db)
			{
				string be, ki;

				do 
				{
					string nev = db->Fetch()[0].GetString();
					string status = db->Fetch()[1].GetString();

					if(status == bekapcsol)
						be += nev + " ";
					else
						ki += nev + " ";
				} while(db->NextRow());

				Log.Notice("Console", "Bekapcsolva: %s", be.c_str());
				Log.Notice("Console", "Kikapcsolva: %s", ki.c_str());
			}
			else
				Log.Notice("Console", "Hibas lekerdezes!");
		}
		else
		{
			if(res.size() < 4)
			{
				Log.Notice("Console", "Nincs a funkcio nev megadva!");
				res.clear();
				return false;
			}

			string status = res[2];

			if(status == bekapcsol || status == kikapcsol)
			{
				string nev = res[3];
				Log.Notice("Console", "%s: %skapcsolva", nev.c_str(), status.c_str());
				sVezerlo.GetSQLConn()->Query("UPDATE schumix SET funkcio_status = '%s' WHERE funkcio_nev = '%s'", status.c_str(), nev.c_str());
			}
		}

		res.clear();
		return true;
	}

	if(cmd == "szoba")
	{
		if(res.size() < 3)
		{
			Log.Notice("Console", "Nincs parameter!");
			res.clear();
			return false;
		}

		sVezerlo.GetSQLConn()->Query("UPDATE schumix SET irc_cim = '%s' WHERE entry = '1'", res[2].c_str());

		res.clear();
		return true;
	}

	if(cmd == "konzollog")
	{
		if(res.size() < 3)
		{
			Log.Notice("Console", "Nincs parameter!");
			res.clear();
			return false;
		}

		if(res[2] == bekapcsol)
		{
			ConsoleLog = true;
			Log.Notice("Console", "Console logolas bekapcsolva.");
		}
		else if(res[2] == kikapcsol)
		{
			ConsoleLog = false;
			Log.Notice("Console", "Console logolas kikapcsolva.");
		}

		res.clear();
		return true;
	}

	if(cmd == "admin")
	{
		if(res.size() < 3)
		{
			Log.Notice("Console", "Parancsok: help | lista | add | del");
			res.clear();
			return false;
		}

		if(res[2] == Help)
		{
			Log.Notice("Console", "Alparancsok hasznalata:");
			Log.Notice("Console", "Admin lista: admin lista");
			Log.Notice("Console", "Hozzaadas: admin add <admin neve>");
			Log.Notice("Console", "Eltavolitas: admin del <admin neve>");
			Log.Notice("Console", "Rang: admin rang <admin neve> <uj rang pl operator: 0, administrator: 1>");
			Log.Notice("Console", "Info: admin info <admin neve>");
		}
		else if(res[2] == INFO)
		{
			if(res.size() < 4)
			{
				Log.Notice("Console", "Nincs nev megadva!");
				res.clear();
				return false;
			}

			int flag;
			transform(res[3].begin(), res[3].end(), res[3].begin(), ::tolower);

			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT flag FROM adminok WHERE nev = '%s'", res[3].c_str());
			if(db)
				flag = cast_int(db->Fetch()[0].GetUInt8());
			else
				flag = -1;

			if(flag == Operator)
				Log.Notice("Console", "Jelenleg Operator.");
			else if(flag == Administrator)
				Log.Notice("Console", "Jelenleg Adminisztrator.");
		}
		else if(res[2] == "lista")
		{
			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nev FROM adminok");
			if(db)
			{
				string adminok;

				do
				{
					string nev = db->Fetch()[0].GetString();
					adminok += ", " + nev;
				} while(db->NextRow());

				Log.Notice("Console", "Adminok: %s", adminok.substr(2).c_str());
			}
		}
		else if(res[2] == added)
		{
			if(res.size() < 4)
			{
				Log.Notice("Console", "Nincs nev megadva!");
				res.clear();
				return false;
			}

			string nev = res[3];
			transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
			string pass = sIRCSession.RandomString(10, true, true, false);

			sVezerlo.GetSQLConn()->Query("INSERT INTO `adminok`(nev, jelszo) VALUES ('%s', '%s')", nev.c_str(), sVezerlo.Sha1(pass).c_str());
			sVezerlo.GetSQLConn()->Query("INSERT INTO `hluzenet`(nick, alapot) VALUES ('%s', 'ki')", nev.c_str());

			Log.Notice("Console", "Admin hozzaadva: %s", nev.c_str());
			Log.Notice("Console", "Mostani jelszo: %s", pass.c_str());
		}
		else if(res[2] == delet)
		{
			if(res.size() < 4)
			{
				Log.Notice("Console", "Nincs nev megadva!");
				res.clear();
				return false;
			}

			string nev = res[3];
			transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
			sVezerlo.GetSQLConn()->Query("DELETE FROM `adminok` WHERE nev = '%s'", nev.c_str());
			sVezerlo.GetSQLConn()->Query("DELETE FROM `hluzenet` WHERE nick = '%s'", nev.c_str());
			Log.Notice("Console", "Admin törölve: %s", nev.c_str());
		}
		else if(res[2] == "rang")
		{
			if(res.size() < 4)
			{
				Log.Notice("Console", "Nincs nev megadva!");
				res.clear();
				return false;
			}

			if(res.size() < 5)
			{
				Log.Notice("Console", "Nincs rang megadva!");
				res.clear();
				return false;
			}

			string nev = res[3];
			transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
			stringstream ss;
			int rang;
			ss << res[4];
			ss >> rang;

			if(rang == Administrator || rang == Operator)
			{
				sVezerlo.GetSQLConn()->Query("UPDATE adminok SET flag = '%i' WHERE nev = '%s'", rang, nev.c_str());
				Log.Notice("Console", "Rang sikeresen modositva.");
			}
			else
				Log.Notice("Console", "Hibas rang!");
		}

		res.clear();
		return true;
	}

	if(cmd == "reload")
	{
		if(res.size() < 3)
		{
			Log.Notice("Console", "Nincs parameter!");
			res.clear();
			return false;
		}

		if(res[2] == Help)
		{
			Log.Notice("Console", "Alparancsok hasznalata:");
			Log.Notice("Console", "Reload: reload <nev>");
		}
		else if(res[2] == INFO)
			Log.Notice("Console", "Most ujraindithatoak: IRCSession, SvnInfo, GitInfo, HgInfo, Console");
		else
			Log.Notice("Console", "%s", sVezerlo.Reload(res[2]).c_str());

		res.clear();
		return true;
	}

	if(cmd == "kikapcs")
	{
		Log.Notice("Console", "Viszlat :(");
		sIRCSession.WriteLine("QUIT :Console: leallitas.");
		Sleep(1000);
		sVezerlo.Leallas();
		res.clear();

		return true;
	}

	res.clear();
	return false;
}
