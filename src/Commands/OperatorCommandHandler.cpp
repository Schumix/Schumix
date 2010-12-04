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

void CommandMgr::HandleAdmin(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		if(sCommands.Admin(recvData.Nick, Operator))
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Parancsok: %snick | %sjoin | %sleft | %skick | %smode", sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Parancsok: %sszinek | %sfunkcio | %ssznap | %schannel | %shozzaferes | %sujjelszo | %ssvn | %sgit | %shg | %shluzenet | %sautofunkcio", sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel());
		}
		else if(sCommands.Admin(recvData.Nick, Administrator))
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Parancsok: %snick | %sjoin | %sleft | %skick | %smode", sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Parancsok: %sszinek | %sfunkcio | %skikapcs | %ssznap | %sszoba | %schannel | %shozzaferes | %sujjelszo | %ssvn | %sgit | %shg | %shluzenet | %sreload | %sautofunkcio", sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel());
		}

		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string iras = res[1];

	if(iras == Help)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Alparancsok használata:");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Admin lista: %sadmin lista", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hozzáadás: %sadmin add <admin neve>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Eltávolítás: %sadmin del <admin neve>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Rang: %sadmin rang <admin neve> <új rang pl operator: 0, administrator: 1>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Info: %sadmin info", sIRCSession.GetParancsElojel());
	}
	else if(iras == INFO)
	{
		int flag;
		string nev = recvData.Nick;
		transform(nev.begin(), nev.end(), nev.begin(), ::tolower);

		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT flag FROM adminok WHERE nev = '%s'", nev.c_str());
		if(db)
			flag = cast_int(db->Fetch()[0].GetUInt8());
		else
			flag = -1;

		if(flag == Operator)
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Jelenleg Operátor vagy.");
		else if(flag == Administrator)
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Jelenleg Adminisztrátor vagy.");
	}
	else if(iras == "lista")
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

			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "2Adminok: %s", adminok.substr(2).c_str());
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás lekérdezés!");
	}
	else if(iras == added)
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
			res.clear();
			return;
		}

		string nev = res[2];
		transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
		string pass = sIRCSession.RandomString(10, true, true, false);

		sVezerlo.GetSQLConn()->Query("INSERT INTO `adminok`(nev, jelszo) VALUES ('%s', '%s')", nev.c_str(), sVezerlo.Sha1(pass).c_str());
		sVezerlo.GetSQLConn()->Query("INSERT INTO `hluzenet`(nick, alapot) VALUES ('%s', 'ki')", nev.c_str());

		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Admin hozzáadva: %s", nev.c_str());
		sIRCSession.SendChatMessage(PRIVMSG, nev.c_str(), "Mostantól Schumix adminja vagy. A te mostani jelszavad: %s", pass.c_str());
		sIRCSession.SendChatMessage(PRIVMSG, nev.c_str(), "Ha megszeretnéd változtatni használd az %sujjelszo parancsot. Használata: %sujjelszo <régi> <új>", sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, nev.c_str(), "Admin nick élesítése: %shozzaferes <jelszó>", sIRCSession.GetParancsElojel());
	}
	else if(iras == delet)
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
			res.clear();
			return;
		}

		string nev = res[2];
		transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
		if(sCommands.Admin(recvData.Nick, Operator) && sCommands.Admin(nev, Administrator))
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nem vagy Adminisztrátor!");
			res.clear();
			return;
		}

		sVezerlo.GetSQLConn()->Query("DELETE FROM `adminok` WHERE nev = '%s'", nev.c_str());
		sVezerlo.GetSQLConn()->Query("DELETE FROM `hluzenet` WHERE nick = '%s'", nev.c_str());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Admin törölve: %s", nev.c_str());
	}
	else if(iras == "rang")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
			res.clear();
			return;
		}

		if(res.size() < 4)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs rang megadva!");
			res.clear();
			return;
		}

		string nev = res[2];
		transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
		if(sCommands.Admin(recvData.Nick, Operator) && sCommands.Admin(nev, Administrator))
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nem vagy Adminisztrátor!");
			res.clear();
			return;
		}

		stringstream ss;
		int rang;
		ss << res[3];
		ss >> rang;

		if(sCommands.Admin(recvData.Nick, Operator) && sCommands.Admin(nev, Operator) && rang == Administrator)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nem vagy Adminisztrátor!");
			res.clear();
			return;
		}

		if(rang == Administrator || rang == Operator)
		{
			sVezerlo.GetSQLConn()->Query("UPDATE adminok SET flag = '%i' WHERE nev = '%s'", rang, nev.c_str());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Rang sikeresen modósitva.");
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás rang!");
	}

	res.clear();
}

void CommandMgr::HandleHozzaferes(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;

	}

	QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nev, jelszo FROM adminok WHERE nev = '%s'", recvData.GetNick());
	if(db)
	{
		string Nev = db->Fetch()[0].GetString();
		string JelszoSql = db->Fetch()[1].GetString();

		if(JelszoSql == sVezerlo.Sha1(res[1]))
		{
			sVezerlo.GetSQLConn()->Query("UPDATE adminok SET vhost = '%s' WHERE nev = '%s'", recvData.GetHost(), Nev.c_str());
			sIRCSession.SendChatMessage(PRIVMSG, Nev.c_str(), "Hozzáférés engedélyezve");
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, Nev.c_str(), "Hozzáférés megtagadva");
	}

	res.clear();
}

void CommandMgr::HandleUjjelszo(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 3)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs az újjelszó megadva!");
		res.clear();
		return;
	}

	QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nev, jelszo FROM adminok WHERE nev = '%s'", recvData.GetNick());
	if(db)
	{
		string Nev = db->Fetch()[0].GetString();
		string JelszoSql = db->Fetch()[1].GetString();
		string ujjelszo = res[2];

		if(JelszoSql == sVezerlo.Sha1(res[1]))
		{
			sVezerlo.GetSQLConn()->Query("UPDATE adminok SET jelszo = '%s' WHERE nev = '%s'", sVezerlo.Sha1(ujjelszo).c_str(), Nev.c_str());
			sIRCSession.SendChatMessage(PRIVMSG, Nev.c_str(), "Jelszó sikeresen meg lett változtatva erre: %s", ujjelszo.c_str());
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, Nev.c_str(), "A mostani jelszó nem egyezik, modósitás megtagadva");
	}

	res.clear();
}

void CommandMgr::HandleFunkciok(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string info = res[1];

	if(info == Help)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Alparancsok használata:");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel kezelés: %sfunkcio <be vagy ki> <funkcio név>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel-en több funkció kezelése: %sfunkcio <be vagy ki> <funkcio név1> <funkcio név2> ... stb", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel kezelés máshonnét: %sfunkcio channel <channel név> <be vagy ki> <funkcio név>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel-en több funkció kezelés máshonnét: %sfunkcio channel <channel név> <be vagy ki> <funkcio név1> <funkcio név2> ... stb", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Együtes kezelés: %sfunkcio all <be vagy ki> <funkcio név>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Együtesen több funkció kezelése: %sfunkcio all <be vagy ki> <funkcio név1> <funkcio név2> ... stb", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "All update kezelése: %sfunkcio update all", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Más channel update kezelése: %sfunkcio update <channel neve>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ahol tartozkodsz channel update kezelése: %sfunkcio update", sIRCSession.GetParancsElojel());
	}
	else if(info == INFO)
	{
		string ChannelInfo = sIRCSession.ChannelFunkciokInfo(recvData.Channel);
		int szokoz = ChannelInfo.find("|");

		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "2Bekapcsolva: %s", ChannelInfo.substr(0, szokoz).c_str());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "2Kikapcsolva: %s", ChannelInfo.substr(szokoz+1).c_str());
	}
	else if(info == "all")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs megadva az 1. paraméter!");
			res.clear();
			return;
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

				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "2Bekapcsolva: %s", be.c_str());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "2Kikapcsolva: %s", ki.c_str());
			}
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás lekérdezés!");
		}
		else
		{
			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs a funkció név megadva!");
				res.clear();
				return;
			}

			string status = res[2];

			if(status == bekapcsol || status == kikapcsol)
			{
				if(res.size() >= 5)
				{
					string alomany;
					int resAdat = res.size();

					for(int i = 3; i < resAdat; i++)
					{
						alomany += ", " + res[i];
						sVezerlo.GetSQLConn()->Query("UPDATE schumix SET funkcio_status = '%s' WHERE funkcio_nev = '%s'", status.c_str(), res[i].c_str());
					}

					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s: %skapcsolva", alomany.substr(2).c_str(), status.c_str());
				}
				else
				{
					string nev = res[3];
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s: %skapcsolva", nev.c_str(), status.c_str());
					sVezerlo.GetSQLConn()->Query("UPDATE schumix SET funkcio_status = '%s' WHERE funkcio_nev = '%s'", status.c_str(), nev.c_str());
				}
			}
		}
	}
	else if(info == "channel")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs channel név megadva!");
			res.clear();
			return;
		}

		if(res.size() < 4)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs megadva a 2. paraméter!");
			res.clear();
			return;
		}

		string channel = res[2];
		string status = res[3];

		if(status == INFO)
		{
			string ChannelInfo = sIRCSession.ChannelFunkciokInfo(channel);
			int szokoz = ChannelInfo.find("|");

			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "2Bekapcsolva: %s", ChannelInfo.substr(0, szokoz).c_str());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "2Kikapcsolva: %s", ChannelInfo.substr(szokoz+1).c_str());
		}
		else if(status == bekapcsol || status == kikapcsol)
		{
			if(res.size() < 5)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs a funkció név megadva!");
				res.clear();
				return;
			}

			if(res.size() >= 6)
			{
				string alomany;
				int resAdat = res.size();

				for(int i = 4; i < resAdat; i++)
				{
					alomany += ", " + res[i];
					sVezerlo.GetSQLConn()->Query("UPDATE channel SET funkciok = '%s' WHERE szoba = '%s'", sIRCSession.ChannelFunkciok(res[i], status, channel).c_str(), channel.c_str());

					sIRCSession.ChannelFunkcioReload();
				}

				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s: %skapcsolva",  alomany.substr(2).c_str(), status.c_str());
			}
			else
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s: %skapcsolva", res[4].c_str(), status.c_str());
				sVezerlo.GetSQLConn()->Query("UPDATE channel SET funkciok = '%s' WHERE szoba = '%s'", sIRCSession.ChannelFunkciok(res[4], status, channel).c_str(), channel.c_str());
				sIRCSession.ChannelFunkcioReload();
			}
		}
	}
	else if(info == update)
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sikeresen frissitve %s channel funkciók.", recvData.GetChannel());
			sVezerlo.GetSQLConn()->Query("UPDATE channel SET funkciok = ',%s:be,%s:be,%s:be,%s:be,%s:be,%s:be,%s:be' WHERE szoba = '%s'", KOSZONES, LOG, REJOIN, HL, PARANCSOK, KICK, MODE, recvData.GetChannel());
			sIRCSession.ChannelFunkcioReload();

			res.clear();
			return;
		}

		if(res[2] == "all")
		{
			map<string, string>::iterator itr = sIRCSession.GetChannelLista().begin();
			for(; itr != sIRCSession.GetChannelLista().end(); itr++)
			{
				string szoba = itr->first;
				sVezerlo.GetSQLConn()->Query("UPDATE channel SET funkciok = ',%s:be,%s:be,%s:be,%s:be,%s:be,%s:be,%s:be' WHERE szoba = '%s'", KOSZONES, LOG, REJOIN, HL, PARANCSOK, KICK, MODE, szoba.c_str());
			}

			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sikeresen frissitve minden channelen a funkciók.");
			sIRCSession.ChannelFunkcioReload();
		}
		else
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sikeresen frissitve %s channel funkciók.", res[2].c_str());
			sVezerlo.GetSQLConn()->Query("UPDATE channel SET funkciok = ',%s:be,%s:be,%s:be,%s:be,%s:be,%s:be,%s:be' WHERE szoba = '%s'", KOSZONES, LOG, REJOIN, HL, PARANCSOK, KICK, MODE, res[2].c_str());
			sIRCSession.ChannelFunkcioReload();
		}
	}
	else
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs a funkció név megadva!");
			res.clear();
			return;
		}

		string status = res[1];

		if(status == bekapcsol || status == kikapcsol)
		{
			if(res.size() >= 4)
			{
				string alomany;
				int resAdat = res.size();

				for(int i = 2; i < resAdat; i++)
				{
					alomany += ", " + res[i];
					sVezerlo.GetSQLConn()->Query("UPDATE channel SET funkciok = '%s' WHERE szoba = '%s'", sIRCSession.ChannelFunkciok(res[i], status, recvData.Channel).c_str(), recvData.GetChannel());
					sIRCSession.ChannelFunkcioReload();
				}

				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s: %skapcsolva",  alomany.substr(2).c_str(), status.c_str());
			}
			else
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s: %skapcsolva", res[2].c_str(), status.c_str());
				sVezerlo.GetSQLConn()->Query("UPDATE channel SET funkciok = '%s' WHERE szoba = '%s'", sIRCSession.ChannelFunkciok(res[2], status, recvData.Channel).c_str(), recvData.GetChannel());
				sIRCSession.ChannelFunkcioReload();
			}
		}
	}

	res.clear();
}

void CommandMgr::HandleChannel(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string iras = res[1];

	if(iras == Help)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Alparancsok használata:");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hozzáadás: %schannel add <channel> <ha van pass akkor az>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Eltávolítás: %schannel del <channel>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Frissítés: %schannel update", sIRCSession.GetParancsElojel());
	}
	else if(iras == added)
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs a channel név megadva!");
			res.clear();
			return;
		}

		string szoba = res[2];

		if(res.size() >= 4)
		{
			string jelszo = res[3];
			sIRCSession.WriteLine("JOIN %s %s", szoba.c_str(), jelszo.c_str());
			sVezerlo.GetSQLConn()->Query("INSERT INTO `channel`(szoba, jelszo) VALUES ('%s', '%s')", szoba.c_str(), jelszo.c_str());
			sVezerlo.GetSQLConn()->Query("UPDATE channel SET aktivitas = 'aktiv' WHERE szoba = '%s'", szoba.c_str());

			sIRCSession.m_ChannelPrivmsg = recvData.Channel;
		}
		else
		{
			sIRCSession.WriteLine("JOIN %s", szoba.c_str());
			sVezerlo.GetSQLConn()->Query("INSERT INTO `channel`(szoba, jelszo) VALUES ('%s', '')", szoba.c_str());
			sVezerlo.GetSQLConn()->Query("UPDATE channel SET aktivitas = 'aktiv' WHERE szoba = '%s'", szoba.c_str());

			sIRCSession.m_ChannelPrivmsg = recvData.Channel;
		}

		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel hozzáadva: %s", szoba.c_str());

		sIRCSession.ChannelListaReload();
		sIRCSession.ChannelFunkcioReload();
	}
	else if(iras == delet)
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs a channel név megadva!");
			res.clear();
			return;
		}

		string szoba = res[2];
		sIRCSession.WriteLine("PART %s", szoba.c_str());
		sVezerlo.GetSQLConn()->Query("DELETE FROM `channel` WHERE szoba = '%s'", szoba.c_str());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel eltávolítva: %s", szoba.c_str());

		sIRCSession.ChannelListaReload();
		sIRCSession.ChannelFunkcioReload();
	}
	else if(iras == update)
	{
		sIRCSession.ChannelListaReload();
		sIRCSession.ChannelFunkcioReload();
	}
	else if(iras == INFO)
	{
		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT szoba, aktivitas, error FROM channel");
		if(db)
		{
			string Aktivszobak, DeAktivszobak;
			bool adatszoba = false;
			bool adatszoba1 = false;

			do 
			{
				string szoba = db->Fetch()[0].GetString();
				string aktivitas = db->Fetch()[1].GetString();
				string error = db->Fetch()[2].GetString();

				if(aktivitas == "aktiv")
				{
					Aktivszobak += ", " + szoba;
					adatszoba = true;
				}
				else if(aktivitas == "nem aktiv")
				{
					DeAktivszobak += ", " + szoba + ":" + error;
					adatszoba1 = true;
				}
			} while(db->NextRow());

			if(adatszoba)
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Aktiv: %s", Aktivszobak.substr(2).c_str());
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Aktiv: Nincs adat.");
			if(adatszoba1)
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Deaktiv: %s", DeAktivszobak.substr(2).c_str());
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Deaktiv: Nincs adat.");
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás lekérdezés!");
	}

	res.clear();
}

void CommandMgr::HandleSznap(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nev, honap, honap1, nap FROM sznap WHERE nev = '%s'", res[1].c_str());
	if(db)
	{
		string nev = db->Fetch()[0].GetString();
		string honap = db->Fetch()[1].GetString();
		uint32 honap1 = db->Fetch()[2].GetUInt32();
		uint32 nap = db->Fetch()[3].GetUInt32();

		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s születés napja: %s %u", nev.c_str(), honap.c_str(), nap);
	}
	else
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs ilyen ember.");

	res.clear();
}

void CommandMgr::HandleNick(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string nick = res[1];
	sIRCSession.m_NickTarolo = nick;
	sIRCSession.WriteLine("NICK %s", nick.c_str());

	res.clear();
}

void CommandMgr::HandleJoin(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() == 2)
	{
		sIRCSession.WriteLine("JOIN %s", res[1].c_str());
		sIRCSession.m_ChannelPrivmsg = recvData.Channel;
	}
	else if(res.size() >= 3)
	{
		sIRCSession.WriteLine("JOIN %s %s", res[1].c_str(), res[2].c_str());
		sIRCSession.m_ChannelPrivmsg = recvData.Channel;
	}

	res.clear();
}

void CommandMgr::HandleLeft(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	sIRCSession.WriteLine("PART %s", res[1].c_str());
	res.clear();
}

void CommandMgr::HandleKick(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string kick = res[1];
	string Nick = sIRCSession.m_NickTarolo;
	transform(Nick.begin(), Nick.end(), Nick.begin(), ::tolower);
	transform(kick.begin(), kick.end(), kick.begin(), ::tolower);
	int szam = res.size();

	if(szam == 2)
	{
		if(kick != Nick)
			sIRCSession.WriteLine("KICK %s %s", recvData.GetChannel(), kick.c_str());

	}
	else if(szam >= 3)
	{
		string oka;
		int resAdat = res.size();

		for(int i = 2; i < resAdat; i++)
			oka += " " + res[i];

		if(kick != Nick)
			sIRCSession.WriteLine("KICK %s %s :%s", recvData.GetChannel(), kick.c_str(), oka.substr(1).c_str());
	}

	res.clear();
}

void CommandMgr::HandleMode(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 3)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
		res.clear();
		return;
	}

	string rang = res[1];
	string nev = res[2];
	string nevek;
	int resAdat = res.size();

	for(int i = 2; i < resAdat; i++)
		nevek += " " + res[i];

	sIRCSession.WriteLine("MODE %s %s %s", recvData.GetChannel(), rang.c_str(), nevek.substr(1).c_str());

	res.clear();
}

void CommandMgr::HandleHLFunkcio(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	if(res[1] == Help)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Alparancsok használata:");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nick lista frissitése: %shluzenet update", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hl üzenet hozzáadása: %shluzenet <üzenet>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hl üzenet küldés állitása: %shluzenet funkcio <állapot>", sIRCSession.GetParancsElojel());
	}
	if(res[1] == INFO)
	{
		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nick, alapot FROM hluzenet");
		if(db)
		{
			string Nevek;

			do
			{
				string nev = db->Fetch()[0].GetString();
				string alapot = db->Fetch()[1].GetString();
				Nevek += ", " + nev + ":" + alapot;
			} while(db->NextRow());

			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Létezõ nickek: %s", Nevek.substr(2).c_str());
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás lekérdezés!");
	}
	if(res[1] == update)
	{
	// Hibás a kod egyenlõre nem lehet használni
	/*
		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT MAX(id) FROM hluzenet");
		if(db)
		{
			uint32 id = db->Fetch()[0].GetUInt32();
			sVezerlo.GetSQLConn()->kiiras = false;

			for(int x = 0; x < id; x++)
				sVezerlo.GetSQLConn()->Query("DELETE FROM `hluzenet` WHERE id = '%u'", id);

			sVezerlo.GetSQLConn()->kiiras = true;
		}

		QueryResultPointer db1 = sVezerlo.GetSQLConn()->Query("SELECT nev FROM adminok");
		if(db1)
		{
			do
			{
				string nev = db1->Fetch()[0].GetString();
				transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
				sVezerlo.GetSQLConn()->Query("INSERT INTO `hluzenet`(nick, alapot) VALUES ('%s', 'ki')", nev.c_str());
			} while(db1->NextRow());
		}
	*/
	}
	if(res[1] == "funkcio")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs a funkció név megadva!");
			res.clear();
			return;
		}

		string nev = recvData.Nick;
		transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
		sVezerlo.GetSQLConn()->Query("UPDATE `hluzenet` SET `alapot` = '%s' WHERE nick = '%s'", res[2].c_str(), nev.c_str());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s: %skapcsolva", nev.c_str(), res[2].c_str());
	}
	else
	{
		string alomany;
		int resAdat = res.size();

		for(int i = 1; i < resAdat; i++)
			alomany += " " + res[i];

		string nev = recvData.Nick;
		transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
		sVezerlo.GetSQLConn()->Query("UPDATE `hluzenet` SET `info` = '%s', `alapot` = 'be' WHERE nick = '%s'", alomany.substr(1).c_str(), nev.c_str());
		sVezerlo.GetSQLConn()->Query("UPDATE `schumix` SET `funkcio_status` = 'be' WHERE funkcio_nev = '%s'", HL);

		res.clear();
	}
}

void CommandMgr::HandleSvn(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string info = res[1];

	if(info == Help)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Alparancsok használata:");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Választható emuk: %ssvn lista", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hozzáadás: %ssvn add <emu> <channel>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Eltávolítás: %ssvn del <emu> <channel>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "New thread: %ssvn new <emu>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Stop thread: %ssvn stop <emu>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Reload all thread: %ssvn reload all", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Reload thread: %ssvn reload <emu>", sIRCSession.GetParancsElojel());
	}
	else if(info == INFO)
	{
		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nev, channel FROM svninfo");
		if(db)
		{
			do
			{
				string nev = db->Fetch()[0].GetString();
				string channel = db->Fetch()[1].GetString();
				vector<string> reschannel(1);
				split(channel, ",", reschannel);

				if(reschannel.size() < 2)
				{
					reschannel.clear();
					continue;
				}

				string adat;
				int resChannel = reschannel.size();

				for(int y = 1; y < resChannel; y++)
					adat += " " + reschannel[y];

				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3%s Channel:2%s", nev.c_str(), adat.c_str());
				reschannel.clear();
			} while(db->NextRow());
		}
	}
	else if(info == "lista")
	{
		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nev FROM svninfo");
		if(db)
		{
			string lista;

			do
			{
				string nev = db->Fetch()[0].GetString();
				lista += " " + nev;
			} while(db->NextRow());

			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "2Lista: 3%s", lista.substr(1).c_str());
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás lekérdezés!");
	}
	else if(info == "new")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
			res.clear();
			return;
		}

		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT id FROM svninfo WHERE nev = '%s'", res[2].c_str());
		if(db)
		{
			sSvnInfo.NewThread(db->Fetch()[0].GetUInt32());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s thread hozzáadva", res[2].c_str());
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sikertelen hozzáadás!");
	}
	else if(info == "stop")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
			res.clear();
			return;
		}

		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT id FROM svninfo WHERE nev = '%s'", res[2].c_str());
		if(db)
		{
			sSvnInfo.StopThread(db->Fetch()[0].GetUInt32());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s thread leállitva", res[2].c_str());
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sikertelen leállitás!");
	}
	else if(info == "reload")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név vagy all megadva!");
			res.clear();
			return;
		}

		if(res[2] == "all")
		{
			sSvnInfo.ReloadAllThread();
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "SvnInfo újrainditás kész.");
		}
		else
		{
			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT id FROM svninfo WHERE nev = '%s'", res[2].c_str());
			if(db)
			{
				sSvnInfo.ReloadThread(db->Fetch()[0].GetUInt32());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s thread újrainditva.", res[2].c_str());
			}
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sikertelen újrainditás!");
		}
	}
	else
	{
		if(info == added)
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
				res.clear();
				return;
			}

			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs a channel név megadva!");
				res.clear();
				return;
			}

			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT channel FROM svninfo WHERE nev = '%s'", res[2].c_str());
			if(db)
			{
				string channel = db->Fetch()[0].GetString();
				vector<string> reschannel(1);
				split(channel, ",", reschannel);

				if(reschannel.size() < 2)
				{
					reschannel.clear();
					return;
				}

				string adat;
				int resChannel = reschannel.size();

				for(int y = 1; y < resChannel; y++)
					adat += "," + reschannel[y];

				adat += "," + res[3];

				reschannel.clear();
				sVezerlo.GetSQLConn()->Query("UPDATE svninfo SET channel = '%s' WHERE nev = '%s'", adat.c_str(), res[2].c_str());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel sikeresen hozzáadva.");
			}
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel hozzáadása sikertelen!");
		}
		else if(info == delet)
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
				res.clear();
				return;
			}

			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs a channel név megadva!");
				res.clear();
				return;
			}

			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT channel FROM svninfo WHERE nev = '%s'", res[2].c_str());
			if(db)
			{
				string channel = db->Fetch()[0].GetString();
				vector<string> reschannel(1);
				split(channel, ",", reschannel);

				if(reschannel.size() < 2)
				{
					reschannel.clear();
					return;
				}

				string adat;
				int resChannel = reschannel.size();

				for(int y = 1; y < resChannel; y++)
				{
					if(reschannel[y] == res[3])
						continue;

					adat += "," + reschannel[y];
				}

				reschannel.clear();
				sVezerlo.GetSQLConn()->Query("UPDATE svninfo SET channel = '%s' WHERE nev = '%s'", adat.c_str(), res[2].c_str());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel sikeresen törölve.");
			}
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel törlése sikertelen!");
		}
	}

	res.clear();
}

void CommandMgr::HandleGit(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string info = res[1];

	if(info == Help)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Alparancsok használata:");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Választható emuk: %sgit lista", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hozzáadás: %sgit add <nev> <tipus> <channel>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Eltávolítás: %sgit del <nev> <tipus> <channel>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "New thread: %sgit new <nev> <tipus>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Stop thread: %sgit stop <nev> <tipus>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Reload all thread: %sgit reload all", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Reload thread: %sgit reload <nev> <tipus>", sIRCSession.GetParancsElojel());
	}
	else if(info == INFO)
	{
		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nev, tipus, channel FROM gitinfo");
		if(db)
		{
			do
			{
				string nev = db->Fetch()[0].GetString();
				string tipus = db->Fetch()[1].GetString();
				string channel = db->Fetch()[2].GetString();
				vector<string> reschannel(1);
				split(channel, ",", reschannel);

				if(reschannel.size() < 2)
				{
					reschannel.clear();
					continue;

				}

				string adat;
				int resChannel = reschannel.size();

				for(int y = 1; y < resChannel; y++)
					adat += " " + reschannel[y];

				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3%s 7%s Channel:2%s", nev.c_str(), tipus.c_str(), adat.c_str());
				reschannel.clear();
			} while(db->NextRow());
		}
	}
	else if(info == "lista")
	{
		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nev, tipus FROM gitinfo");
		if(db)
		{
			string lista;

			do
			{
				string nev = db->Fetch()[0].GetString();
				string tipus = db->Fetch()[1].GetString();

				lista += " " + nev + " " + tipus + ";";
			} while(db->NextRow());

			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "2Lista: 3%s", lista.substr(1).c_str());
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás lekérdezés!");
	}
	else if(info == "new")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
			res.clear();
			return;
		}

		if(res.size() < 4)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs tipus név megadva!");
			res.clear();
			return;
		}

		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT id FROM gitinfo WHERE nev = '%s' AND tipus = '%s'", res[2].c_str(), res[3].c_str());
		if(db)
		{
			sGitInfo.NewThread(db->Fetch()[0].GetUInt32());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s %s thread hozzáadva", res[2].c_str(), res[3].c_str());
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sikertelen hozzáadás!");
	}
	else if(info == "stop")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
			res.clear();
			return;
		}

		if(res.size() < 4)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs tipus név megadva!");
			res.clear();
			return;
		}

		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT id FROM gitinfo WHERE nev = '%s' AND tipus = '%s'", res[2].c_str(), res[3].c_str());
		if(db)
		{
			sGitInfo.StopThread(db->Fetch()[0].GetUInt32());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s %s thread leállitva", res[2].c_str(), res[3].c_str());
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sikertelen leállitás!");
	}
	else if(info == "reload")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név vagy all megadva!");
			res.clear();
			return;
		}

		if(res[2] == "all")
		{
			sGitInfo.ReloadAllThread();
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "GitInfo újrainditás kész.");
		}
		else
		{
			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs tipus név megadva!");
				res.clear();
				return;
			}

			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT id FROM gitinfo WHERE nev = '%s' AND tipus = '%s'", res[2].c_str(), res[3].c_str());
			if(db)
			{
				sGitInfo.ReloadThread(db->Fetch()[0].GetUInt32());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s %s thread újrainditva.", res[2].c_str(), res[3].c_str());
			}
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sikertelen újrainditás!");
		}
	}
	else
	{
		if(info == added)
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
				res.clear();
				return;
			}

			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs tipus név megadva!");
				res.clear();
				return;
			}

			if(res.size() < 5)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs a channel név megadva!");
				res.clear();
				return;
			}


			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT id, channel FROM gitinfo WHERE nev = '%s' AND tipus = '%s'", res[2].c_str(), res[3].c_str());
			if(db)
			{
				string channel = db->Fetch()[1].GetString();
				vector<string> reschannel(1);
				split(channel, ",", reschannel);

				if(reschannel.size() < 2)
				{
					reschannel.clear();
					return;
				}

				string adat;
				int resChannel = reschannel.size();

				for(int y = 1; y < resChannel; y++)
					adat += "," + reschannel[y];

				adat += "," + res[4];

				reschannel.clear();
				sVezerlo.GetSQLConn()->Query("UPDATE gitinfo SET channel = '%s' WHERE id = '%u'", adat.c_str(), db->Fetch()[0].GetUInt32());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel sikeresen hozzáadva.");
			}
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel hozzáadása sikertelen!");
		}
		else if(info == delet)
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
				res.clear();
				return;
			}

			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs tipus név megadva!");
				res.clear();
				return;
			}


			if(res.size() < 5)

			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs a channel név megadva!");
				res.clear();
				return;
			}

			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT id, channel FROM gitinfo WHERE nev = '%s' AND tipus = '%s'", res[2].c_str(), res[3].c_str());
			if(db)
			{
				string channel = db->Fetch()[1].GetString();
				vector<string> reschannel(1);
				split(channel, ",", reschannel);

				if(reschannel.size() < 2)
				{
					reschannel.clear();
					return;
				}

				string adat;
				int resChannel = reschannel.size();

				for(int y = 1; y < resChannel; y++)
				{
					if(reschannel[y] == res[4])
						continue;

					adat += "," + reschannel[y];
				}

				reschannel.clear();
				sVezerlo.GetSQLConn()->Query("UPDATE gitinfo SET channel = '%s' WHERE id = '%u'", adat.c_str(), db->Fetch()[0].GetUInt32());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel sikeresen törölve.");
			}
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel törlése sikertelen!");
		}
	}

	res.clear();
}

void CommandMgr::HandleHg(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string info = res[1];

	if(info == Help)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Alparancsok használata:");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Választható emuk: %shg lista", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hozzáadás: %shg add <emu> <channel>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Eltávolítás: %shg del <emu> <channel>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "New thread: %shg new <emu>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Stop thread: %shg stop <emu>", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Reload all thread: %shg reload all", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Reload thread: %shg reload <emu>", sIRCSession.GetParancsElojel());
	}
	else if(info == INFO)
	{
		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nev, channel FROM hginfo");
		if(db)
		{
			do
			{
				string nev = db->Fetch()[0].GetString();
				string channel = db->Fetch()[1].GetString();
				vector<string> reschannel(1);
				split(channel, ",", reschannel);

				if(reschannel.size() < 2)
				{
					reschannel.clear();
					continue;
				}

				string adat;
				int resChannel = reschannel.size();

				for(int y = 1; y < resChannel; y++)
					adat += " " + reschannel[y];

				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3%s Channel:2%s", nev.c_str(), adat.c_str());
				reschannel.clear();
			} while(db->NextRow());
		}
	}
	else if(info == "lista")
	{
		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nev FROM hginfo");
		if(db)
		{
			string lista;

			do
			{
				string nev = db->Fetch()[0].GetString();
				lista += " " + nev;
			} while(db->NextRow());

			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "2Lista: 3%s", lista.substr(1).c_str());
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás lekérdezés!");
	}
	else if(info == "new")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
			res.clear();
			return;
		}

		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT id FROM hginfo WHERE nev = '%s'", res[2].c_str());
		if(db)
		{
			sHgInfo.NewThread(db->Fetch()[0].GetUInt32());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s thread hozzáadva", res[2].c_str());
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sikertelen hozzáadás!");
	}
	else if(info == "stop")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
			res.clear();
			return;
		}

		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT id FROM hginfo WHERE nev = '%s'", res[2].c_str());
		if(db)
		{
			sHgInfo.StopThread(db->Fetch()[0].GetUInt32());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s thread leállitva", res[2].c_str());
		}
		else
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sikertelen leállitás!");
	}
	else if(info == "reload")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név vagy all megadva!");
			res.clear();
			return;
		}

		if(res[2] == "all")
		{
			sHgInfo.ReloadAllThread();
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "HgInfo újrainditás kész.");
		}
		else
		{
			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT id FROM hginfo WHERE nev = '%s'", res[2].c_str());
			if(db)
			{
				sHgInfo.ReloadThread(db->Fetch()[0].GetUInt32());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s thread újrainditva.", res[2].c_str());
			}
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sikertelen újrainditás!");
		}
	}
	else
	{
		if(info == added)
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
				res.clear();
				return;
			}

			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs a channel név megadva!");
				res.clear();
				return;
			}

			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT channel FROM hginfo WHERE nev = '%s'", res[2].c_str());
			if(db)
			{
				string channel = db->Fetch()[0].GetString();
				vector<string> reschannel(1);
				split(channel, ",", reschannel);

				if(reschannel.size() < 2)
				{
					reschannel.clear();
					return;
				}

				string adat;
				int resChannel = reschannel.size();

				for(int y = 1; y < resChannel; y++)
					adat += "," + reschannel[y];

				adat += "," + res[3];

				reschannel.clear();
				sVezerlo.GetSQLConn()->Query("UPDATE hginfo SET channel = '%s' WHERE nev = '%s'", adat.c_str(), res[2].c_str());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel sikeresen hozzáadva.");
			}
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel hozzáadása sikertelen!");
		}
		else if(info == delet)
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
				res.clear();
				return;
			}

			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs a channel név megadva!");
				res.clear();
				return;
			}

			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT channel FROM hginfo WHERE nev = '%s'", res[2].c_str());
			if(db)
			{
				string channel = db->Fetch()[0].GetString();
				vector<string> reschannel(1);
				split(channel, ",", reschannel);

				if(reschannel.size() < 2)
				{
					reschannel.clear();
					return;
				}

				string adat;
				int resChannel = reschannel.size();

				for(int y = 1; y < resChannel; y++)
				{
					if(reschannel[y] == res[3])
						continue;

					adat += "," + reschannel[y];
				}

				reschannel.clear();
				sVezerlo.GetSQLConn()->Query("UPDATE hginfo SET channel = '%s' WHERE nev = '%s'", adat.c_str(), res[2].c_str());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel sikeresen törölve.");
			}
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel törlése sikertelen!");
		}
	}

	res.clear();
}

void CommandMgr::HandleAutoFunkcio(CommandMessage& recvData)
{
	if(!sCommands.Admin(recvData.Nick, recvData.Host, Operator))
		return;

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Parancsok: kick | mode");
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	if(res[1] == Help)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Alparancsok használata:");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Auto kick help: %sautofunkcio kick help", sIRCSession.GetParancsElojel());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Auto mode help: %sautofunkcio mode help", sIRCSession.GetParancsElojel());
	}
	/*if(res[1] == INFO)
	{

	}*/
	if(res[1] == "kick")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs megadva az 1. paraméter!");
			res.clear();
			return;
		}

		if(res[2] == added)
		{
			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
				res.clear();
				return;
			}

			if(res.size() < 5)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs ok megadva!");
				res.clear();
				return;
			}

			string alomany;
			int resAdat = res.size();

			for(int i = 4; i < resAdat; i++)
				alomany += " " + res[i];

			transform(res[3].begin(), res[3].end(), res[3].begin(), ::tolower);
			sVezerlo.GetSQLConn()->Query("INSERT INTO `kicklista`(nick, channel, oka) VALUES ('%s', '%s', '%s')", res[3].c_str(), recvData.GetChannel(), alomany.substr(1).c_str());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kick listához a név hozzáadva: %s", res[3].c_str());
		}
		else if(res[2] == delet)
		{
			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
				res.clear();
				return;
			}

			transform(res[3].begin(), res[3].end(), res[3].begin(), ::tolower);
			sVezerlo.GetSQLConn()->Query("DELETE FROM `kicklista` WHERE nick = '%s'", res[3].c_str());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kick listából a név eltávólitva: %s", res[3].c_str());
		}
		else if(res[2] == Help)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Alparancsok használata:");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ahol vagy channel kick-elt hozzáadása: %sautofunkcio kick add <nev> <oka>", sIRCSession.GetParancsElojel());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ahol vagy channel kick-elt eltávólítása: %sautofunkcio kick del <nev>", sIRCSession.GetParancsElojel());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Másik channel channel kick-elt hozzáadása: %sautofunkcio kick channel add <nev> <channel> <oka>", sIRCSession.GetParancsElojel());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Másik channel channel kick-elt eltávólítása: %sautofunkcio kick channel del <nev>", sIRCSession.GetParancsElojel());
		}
		else if(res[2] == INFO)
		{
			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nick, channel FROM kicklista WHERE channel = '%s'", recvData.GetChannel());
			if(db)
			{
				string Nevek;

				do
				{
					string nev = db->Fetch()[0].GetString();
					Nevek += ", " + nev + ":" + recvData.Channel;
				} while(db->NextRow());

				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kick listán lévök: %s", Nevek.substr(2).c_str());
			}
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás lekérdezés!");
		}
		else if(res[2] == "channel")
		{
			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs megadva az 1. paraméter!");
				res.clear();
				return;
			}

			if(res[3] == added)
			{
				if(res.size() < 5)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
					res.clear();
					return;
				}

				if(res.size() < 6)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs channel megadva!");
					res.clear();
					return;
				}

				if(res.size() < 7)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs ok megadva!");
					res.clear();
					return;
				}

				string alomany;
				int resAdat = res.size();

				for(int i = 6; i < resAdat; i++)
					alomany += " " + res[i];

				transform(res[4].begin(), res[4].end(), res[4].begin(), ::tolower);
				sVezerlo.GetSQLConn()->Query("INSERT INTO `kicklista`(nick, channel, oka) VALUES ('%s', '%s', '%s')", res[4].c_str(), res[5].c_str(), alomany.substr(1).c_str());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kick listához a név hozzáadva: %s", res[4].c_str());
			}
			else if(res[3] == delet)
			{
				if(res.size() < 5)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
					res.clear();
					return;
				}

				transform(res[4].begin(), res[4].end(), res[4].begin(), ::tolower);
				sVezerlo.GetSQLConn()->Query("DELETE FROM `kicklista` WHERE nick = '%s'", res[4].c_str());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kick listából a név eltávólitva: %s", res[4].c_str());
			}
			else if(res[3] == INFO)
			{
				QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nick, channel FROM kicklista");
				if(db)
				{
					string Nevek;

					do
					{
						string nev = db->Fetch()[0].GetString();
						string szoba = db->Fetch()[1].GetString();
						Nevek += ", " + nev + ":" + szoba;
					} while(db->NextRow());

					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kick listán lévök: %s", Nevek.substr(2).c_str());
				}
				else
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás lekérdezés!");
			}
		}
	}
	if(res[1] == "mode")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs megadva az 1. paraméter!");
			res.clear();
			return;
		}

		if(res[2] == added)
		{
			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
				res.clear();
				return;
			}

			if(res.size() < 5)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs rang megadva!");
				res.clear();
				return;
			}

			transform(res[3].begin(), res[3].end(), res[3].begin(), ::tolower);
			sVezerlo.GetSQLConn()->Query("INSERT INTO `modelista`(nick, channel, rang) VALUES ('%s', '%s', '%s')", res[3].c_str(), recvData.GetChannel(), res[4].c_str());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Mode listához a név hozzáadva: %s", res[3].c_str());
		}
		else if(res[2] == delet)
		{
			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
				res.clear();
				return;
			}

			transform(res[3].begin(), res[3].end(), res[3].begin(), ::tolower);
			sVezerlo.GetSQLConn()->Query("DELETE FROM `modelista` WHERE nick = '%s'", res[3].c_str());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Mode listából a név eltávólitva: %s", res[3].c_str());
		}
		else if(res[2] == Help)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Alparancsok használata:");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ahol vagy channel rang hozzáadása: %sautofunkcio mode add <nev> <rang>", sIRCSession.GetParancsElojel());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ahol vagy channel rang eltávólítása: %sautofunkcio mode del <nev>", sIRCSession.GetParancsElojel());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Másik channel rang hozzáadása: %sautofunkcio mode channel add <nev> <channel> <rang>", sIRCSession.GetParancsElojel());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Másik channel rang eltávólítása: %sautofunkcio mode channel del <nev>", sIRCSession.GetParancsElojel());
		}
		else if(res[2] == INFO)
		{
			QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nick FROM modelista WHERE channel = '%s'", recvData.GetChannel());
			if(db)
			{
				string Nevek;

				do
				{
					string nev = db->Fetch()[0].GetString();
					Nevek += ", " + nev + ":" + recvData.Channel;
				} while(db->NextRow());

				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Mode listán lévök: %s", Nevek.substr(2).c_str());
			}
			else
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás lekérdezés!");
		}
		else if(res[2] == "channel")
		{
			if(res.size() < 4)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs megadva az 1. paraméter!");
				res.clear();
				return;
			}

			if(res[3] == added)
			{
				if(res.size() < 5)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
					res.clear();
					return;
				}

				if(res.size() < 6)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs channel megadva!");
					res.clear();
					return;
				}

				if(res.size() < 7)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs rang megadva!");
					res.clear();
					return;
				}

				transform(res[4].begin(), res[4].end(), res[4].begin(), ::tolower);
				sVezerlo.GetSQLConn()->Query("INSERT INTO `modelista`(nick, channel, rang) VALUES ('%s', '%s', '%s')", res[4].c_str(), res[5].c_str(), res[6].c_str());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Mode listához a név hozzáadva: %s", res[4].c_str());
			}
			else if(res[3] == delet)
			{
				if(res.size() < 5)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs név megadva!");
					res.clear();
					return;
				}

				transform(res[4].begin(), res[4].end(), res[4].begin(), ::tolower);
				sVezerlo.GetSQLConn()->Query("DELETE FROM `modelista` WHERE nick = '%s'", res[3].c_str());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Mode listából a név eltávólitva: %s", res[4].c_str());
			}
			else if(res[3] == INFO)
			{
				QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT nick, channel FROM modelista");
				if(db)
				{
					string Nevek;

					do
					{
						string nev = db->Fetch()[0].GetString();
						string szoba = db->Fetch()[1].GetString();
						Nevek += ", " + nev + ":" + szoba;
					} while(db->NextRow());

					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Mode listán lévök: %s", Nevek.substr(2).c_str());
				}
				else
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás lekérdezés!");
			}
		}
	}
}
