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

void CommandMgr::HandleHelp(CommandMessage& recvData)
{
	CNick(recvData);

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ha a aparancs mögé irod a megadot paranc snevét vagy a nevet és egy alparancsát információt add a használatáról.");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Fő parancsom: %sxbot", sIRCSession.GetParancsElojel());
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	// Publikus parancsok segítségei
	if(res[1] == "xbot")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Felhasználok számára használható parancslista.");
	}
	else if(res[1] == "info")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kis leírás a botról.");
	}
	else if(res[1] == "whois")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "A parancs segítségével megtudhatjuk hogy egy nick milyen channelon van fent.");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %swhois <nick>", sIRCSession.GetParancsElojel());
	}
	else if(res[1] == "jegyzet")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Különböző adatokat jegyezhetünk fel a segítségével.");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Jegyzet beküldése: %sjegyzet <amit feljegyeznél>", sIRCSession.GetParancsElojel());
			res.clear();
			return;
		}

		if(res[2] == "kod")
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Jegyzet kiolvasásához szükséges kód.", sIRCSession.GetParancsElojel());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sjegyzet kod <kod amit kaptál>", sIRCSession.GetParancsElojel());
		}
	}
	else if(res[1] == "roll")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Csöp szorakozás a wowból, már ha valaki felismeri :P");
	}
	else if(res[1] == "datum")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Az aktuális dátumot irja ki és a hozzá tartozó névnapot.");
	}
	else if(res[1] == "ido")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Az aktuális időt irja ki.");
	}
	else if(res[1] == "keres")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ha szökséged lenne valamire a google-ből nem kell hozzá weboldal csak ez a parancs.");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %skeres <ide jön a kereset szöveg>", sIRCSession.GetParancsElojel());
	}
	else if(res[1] == "fordit")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ha rögtön kéne fordítani másik nyelvre vagy ről valamit megteheted ezzel a parancsal.");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sfordit <miről|mire> <szöveg>", sIRCSession.GetParancsElojel());
	}
	else if(res[1] == "xrev")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Amik a kódba vannak integrálva projectek annak lekérdezhetőek egyes verziói.");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sxrev <emulátor neve> <rev>", sIRCSession.GetParancsElojel());
	}
	else if(res[1] == "irc")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Néhány parancs használata az IRC-n.");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sirc <parancs neve>", sIRCSession.GetParancsElojel());
	}
	else if(res[1] == "calc")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Több funkciós számologép.");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %scalc <egyenlet>", sIRCSession.GetParancsElojel());
	}
	else if(res[1] == "uzenet")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Figyelmeztető üzenet küldése hogy keresik ezzen a channelen vagy egy tetszöleges üzenet küldése.");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %süzenet <ide jön a személy> <ha nem felhivás küldenél hanem saját üzenetet>", sIRCSession.GetParancsElojel());
	}
	else if(res[1] == "sha1")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sha1 kódolássá átalakitó parancs.");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %ssha1 <átalakitandó szöveg>", sIRCSession.GetParancsElojel());
	}
	else if(res[1] == "md5")
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Md5 kódolássá átalakitó parancs.");
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %smd5 <átalakitandó szöveg>", sIRCSession.GetParancsElojel());
	}

	// Operátor parancsok segítségei
	if(sCommands.Admin(recvData.Nick, recvData.Host, Operator))
	{
		if(res[1] == "admin")
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja az operátorok vagy adminisztrátorok által használható parancsokat.");
				res.clear();
				return;
			}

			if(res[2] == "add")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Új admin hozzáadása.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sadmin add <admin neve>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "del")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Admin eltávolítása.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sadmin del <admin neve>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "rang")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Admin rangjának megváltoztatása.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sadmin rang <admin neve> <új rang pl operator: 0, administrator: 1>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "info")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja az összes admin nevét aki az adatbázisban szerepel.", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "hozzaferes")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Az admin parancsok használatához szükséges jelszó ellenörző és vhost aktiváló.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sadmin hozzaferes <jelszó>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "ujjelszo")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Az admin jelszavának cseréje ha új kéne a régi helyet.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sadmin ujjelszo <régi jelszó> <új jelszó>", sIRCSession.GetParancsElojel());
			}
		}
		else if(res[1] == "channel")
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Összes channel kiirása ami az adatbázisban van és a hozzájuk tartozó informáciok.");
				res.clear();
				return;
			}

			if(res[2] == "add")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Új channel hozzáadása.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %schannel add <channel> <ha van pass akkor az>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "del")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nem használatos channel eltávolítása.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %schannel del <channel>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "update")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channelekhez tartozó összes információ frissitése, alapértelmezésre állítása.", sIRCSession.GetParancsElojel());
			}
		}
		else if(res[1] == "funkcio")
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Funkciók vezérlésére szolgáló parancs.");
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata ahol tartozkodsz:");
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel funkció kezelése: %sfunkcio <be vagy ki> <funkcio név>", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel funkciók kezelése: %sfunkcio <be vagy ki> <funkcio név1> <funkcio név2> ... stb", sIRCSession.GetParancsElojel());
				res.clear();
				return;
			}

			if(res[2] == "channel")
			{
				if(res.size() < 4)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Megadot channelen állithatók ezzel a parancsal a funkciók.", sIRCSession.GetParancsElojel());
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata:");
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel funkció kezelése: %sfunkcio <be vagy ki> <funkcio név>", sIRCSession.GetParancsElojel());
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Channel funkciók kezelése: %sfunkcio <be vagy ki> <funkcio név1> <funkcio név2> ... stb", sIRCSession.GetParancsElojel());
					res.clear();
					return;
				}

				if(res[3] == "info")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja a funkciók állapotát.", sIRCSession.GetParancsElojel());
				}
			}
			else if(res[2] == "all")
			{
				if(res.size() < 4)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Globális funkciók kezelése.", sIRCSession.GetParancsElojel());
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Együtes kezelés: %sfunkcio all <be vagy ki> <funkcio név>", sIRCSession.GetParancsElojel());
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Együtes funkciók kezelése: %sfunkcio all <be vagy ki> <funkcio név1> <funkcio név2> ... stb", sIRCSession.GetParancsElojel());
					res.clear();
					return;
				}

				if(res[3] == "info")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja a funkciók állapotát.", sIRCSession.GetParancsElojel());
				}
			}
			else if(res[2] == "update")
			{
				if(res.size() < 4)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Frissiti a funkciókat vagy alapértelmezésre állitja.", sIRCSession.GetParancsElojel());
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata:");
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Más channel: %sfunkcio update <channel neve>", sIRCSession.GetParancsElojel());
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ahol tartozkodsz channel: %sfunkcio update", sIRCSession.GetParancsElojel());
					res.clear();
					return;
				}

				if(res[3] == "all")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Frissiti az összes funkciót vagy alapértelmezésre állitja.", sIRCSession.GetParancsElojel());
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sfunkcio update all", sIRCSession.GetParancsElojel());
				}
			}
			else if(res[2] == "info")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja a funkciók állapotát.", sIRCSession.GetParancsElojel());
			}
		}
		else if(res[1] == "autofunkcio")
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Autómatikusan müködő kódrészek kezelése.");
				res.clear();
				return;
			}

			if(res[2] == "kick")
			{
				if(res.size() < 4)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Autómatikusan kirúgásra kerülő nick-ek kezelése.");
					res.clear();
					return;
				}

				if(res[3] == "add")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kirúgandó nevének hozzáadása ahol tartozkodsz.", sIRCSession.GetParancsElojel());
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sautofunkcio kick add <nev> <oka>", sIRCSession.GetParancsElojel());
				}
				else if(res[3] == "del")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kirúgandó nevének eltávolítása ahol tartozkodsz.", sIRCSession.GetParancsElojel());
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sautofunkcio kick del <nev>", sIRCSession.GetParancsElojel());
				}
				else if(res[3] == "info")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja a kirúgandok állapotát.", sIRCSession.GetParancsElojel());
				}
				else if(res[3] == "channel")
				{
					if(res.size() < 5)
					{
						sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Autómatikusan kirúgásra kerülő nick-ek kezelése megadot channelen.");
						res.clear();
						return;
					}

					if(res[4] == "add")
					{
						sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kirúgandó nevének hozzáadása megadott channelen.", sIRCSession.GetParancsElojel());
						sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sautofunkcio kick channel add <nev> <channel> <oka>", sIRCSession.GetParancsElojel());
					}
					else if(res[4] == "del")
					{
						sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kirúgandó nevének eltávolítása megadott channelen.", sIRCSession.GetParancsElojel());
						sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sautofunkcio kick channel del <nev>", sIRCSession.GetParancsElojel());
					}
					else if(res[4] == "info")
					{
						sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja a kirúgandok állapotát.", sIRCSession.GetParancsElojel());
					}
				}
			}
			else if(res[2] == "mode")
			{
				if(res.size() < 4)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Autómatikusan rangot kapó nick-ek kezelése.");
					res.clear();
					return;
				}

				if(res[3] == "add")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Rangot kapó nevének hozzáadása ahol tartozkodsz.", sIRCSession.GetParancsElojel());
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sautofunkcio mode add <nev> <rang>", sIRCSession.GetParancsElojel());
				}
				else if(res[3] == "del")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Rangot kapó nevének eltávolítása ahol tartozkodsz.", sIRCSession.GetParancsElojel());
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sautofunkcio mode del <nev>", sIRCSession.GetParancsElojel());
				}
				else if(res[3] == "info")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja a rangot kapók állapotát.", sIRCSession.GetParancsElojel());
				}
				else if(res[3] == "channel")
				{
					if(res.size() < 5)
					{
						sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Autómatikusan rangot kapó nick-ek kezelése megadot channelen.");
						res.clear();
						return;
					}

					if(res[4] == "add")
					{
						sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Rangot kapó nevének hozzáadása megadott channelen.", sIRCSession.GetParancsElojel());
						sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sautofunkcio mode channel add <nev> <channel> <rang>", sIRCSession.GetParancsElojel());
					}
					else if(res[4] == "del")
					{
						sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Rangot kapó nevének eltávolítása megadott channelen.", sIRCSession.GetParancsElojel());
						sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sautofunkcio mode channel del <nev>", sIRCSession.GetParancsElojel());
					}
					else if(res[4] == "info")
					{
						sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja a rangot kapók állapotát.", sIRCSession.GetParancsElojel());
					}
				}
			}
			else if(res[2] == "hlfunkcio")
			{
				if(res.size() < 4)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Autómatikusan hl-t kapó nick-ek kezelése.");
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sautofunkcio hluzenet <üzenet>", sIRCSession.GetParancsElojel());
					res.clear();
					return;
				}

				if(res[3] == "funkcio")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ezzel a parancsal állitható a hl állapota.", sIRCSession.GetParancsElojel());
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %shluzenet funkcio <állapot>", sIRCSession.GetParancsElojel());
				}
				else if(res[3] == "update")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nem üzemel!", sIRCSession.GetParancsElojel());
				}
				else if(res[3] == "info")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja a hl-ek állapotát.", sIRCSession.GetParancsElojel());
				}
			}
		}
		else if(res[1] == "szinek")
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Adot skálájú szinek kiirása amit lehet használni IRC-n.");
		}
		else if(res[1] == "sznap")
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja a megadott név születésnapjának dátumát.");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %ssznap <név>", sIRCSession.GetParancsElojel());
		}
		else if(res[1] == "nick")
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Bot nick nevének cseréje.");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %snick <név>", sIRCSession.GetParancsElojel());
		}
		else if(res[1] == "join")
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kapcsolodás megadot channelra.");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata:");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Jelszó nélküli channel: %sjoin <channel>", sIRCSession.GetParancsElojel());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Jelszóval ellátott channel: %sjoin <channel> <jelszó>", sIRCSession.GetParancsElojel());
		}
		else if(res[1] == "left")
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Lelépés megadot channelra.");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sleft <channel>", sIRCSession.GetParancsElojel());
		}
		else if(res[1] == "kick")
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kirúgja a nick-et a megadott channelről.");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata:");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Csak kirugás: %skick <channel> <név>", sIRCSession.GetParancsElojel());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kirugás okkal: %skick <channel> <név> <oka>", sIRCSession.GetParancsElojel());
		}
		else if(res[1] == "mode")
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Megváltoztatja a nick rangját megadott channelen.");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %smode <rang> <név vagy nevek>", sIRCSession.GetParancsElojel());
		}
		else if(res[1] == "svn")
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Svn rss-ek kezelése.");
				res.clear();
				return;
			}

			if(res[2] == "add")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Új channel hozzáadása az rss-hez.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %ssvn add <rss neve> <channel>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "del")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nem használatos channel eltávolítása az rss-ből.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %ssvn del <rss neve> <channel>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "info")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja az rss-ek állapotát.", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "lista")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Választható rss-ek listája.", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "new")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Új rss betöltése.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %ssvn new <rss neve>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "stop")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Rss leállítása.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %ssvn stop <rss neve>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "reload")
			{
				if(res.size() < 4)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Megadott rss újratöltése.");
					res.clear();
					return;
				}

				else if(res[3] == "all")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Minden rss újratöltése.");
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %ssvn reload <rss neve>", sIRCSession.GetParancsElojel());
				}
			}
		}
		else if(res[1] == "git")
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Git rss-ek kezelése.");
				res.clear();
				return;
			}

			if(res[2] == "add")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Új channel hozzáadása az rss-hez.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sgit add <rss neve> <tipus> <channel>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "del")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nem használatos channel eltávolítása az rss-ből.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sgit del <rss neve> <channel>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "info")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja az rss-ek állapotát.", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "lista")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Választható rss-ek listája.", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "new")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Új rss betöltése.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sgit new <rss neve> <tipus>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "stop")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Rss leállítása.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sgit stop <rss neve> <tipus>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "reload")
			{
				if(res.size() < 4)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Megadott rss újratöltése.");
					res.clear();
					return;
				}

				else if(res[3] == "all")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Minden rss újratöltése.");
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sgit reload <rss neve> <tipus>", sIRCSession.GetParancsElojel());
				}
			}
		}
		else if(res[1] == "hg")
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hg rss-ek kezelése.");
				res.clear();
				return;
			}

			if(res[2] == "add")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Új channel hozzáadása az rss-hez.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %shg add <rss neve> <channel>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "del")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nem használatos channel eltávolítása az rss-ből.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %shg del <rss neve> <channel>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "info")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Kiirja az rss-ek állapotát.", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "lista")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Választható rss-ek listája.", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "new")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Új rss betöltése.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %shg new <rss neve>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "stop")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Rss leállítása.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %shg stop <rss neve>", sIRCSession.GetParancsElojel());
			}
			else if(res[2] == "reload")
			{
				if(res.size() < 4)
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Megadott rss újratöltése.");
					res.clear();
					return;
				}

				else if(res[3] == "all")
				{
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Minden rss újratöltése.");
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %shg reload <rss neve>", sIRCSession.GetParancsElojel());
				}
			}
		}
	}

	// Adminisztrátor parancsok
	if(sCommands.Admin(recvData.Nick, recvData.Host, Administrator))
	{
		if(res[1] == "teszt")
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Teszt célokra használt parancs.");
		}
		else if(res[1] == "reload")
		{
			if(res.size() < 3)
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Bot egyes részeinek újratöltése.");
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %sreload <rész neve>", sIRCSession.GetParancsElojel());
				res.clear();
				return;
			}

			if(res[2] == "info")
			{
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Jelenleg újrainditható részekről infó.", sIRCSession.GetParancsElojel());
				sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Használata: %shg add <rss neve> <channel>", sIRCSession.GetParancsElojel());
			}
		}
		else if(res[1] == "kikapcs")
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Bot leállítására használható parancs.");
		}
	}

	res.clear();
}
