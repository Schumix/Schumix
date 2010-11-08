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

#include "StdAfx.h"

void IRCSession::Admin(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		if(Admin(recvData.source_nick, Operator))
		{
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Parancsok: %snick | %sjoin | %sleft | %skick | %smode", m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str());
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Parancsok: %sszinek | %sfunkcio | %ssznap | %schannel | %shozzaferes | %sujjelszo | %ssvn | %sgit | %shg | %shluzenet", m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str());
		}
		else if(Admin(recvData.source_nick, Administrator))
		{
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Parancsok: %snick | %sjoin | %sleft | %skick | %smode", m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str());
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Parancsok: %sszinek | %sfunkcio | %skikapcs | %ssznap | %sszoba | %schannel | %shozzaferes | %sujjelszo | %ssvn | %sgit | %shg | %shluzenet | %sreload", m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str());
		}

		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string iras = res[1];

	if(iras == Help)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Alparancsok használata:");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Admin lista: %sadmin lista", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hozzáadás: %sadmin add <admin neve>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Eltávolítás: %sadmin del <admin neve>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Rang: %sadmin rang <admin neve> <új rang pl operator: 0, administrator: 1>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Info: %sadmin info", m_ParancsElojel.c_str());
	}
	else if(iras == INFO)
	{
		int flag;
		string nev = recvData.source_nick;
		transform(nev.begin(), nev.end(), nev.begin(), ::tolower);

		QueryResultPointer db = m_SQLConn->Query("SELECT flag FROM adminok WHERE nev = '%s'", nev.c_str());
		if(db)
			flag = cast_int(db->Fetch()[0].GetUInt8());

		if(flag == Operator)
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Jelenleg Operátor vagy.");
		else if(flag == Administrator)
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Jelenleg Adminisztrátor vagy.");
	}
	else if(iras == "lista")
	{
		QueryResultPointer db = m_SQLConn->Query("SELECT nev FROM adminok");
		if(db)
		{
			string adminok;

			do
			{
				string nev = db->Fetch()[0].GetString();
				adminok += ", " + nev;
			} while(db->NextRow());

			SendChatMessage(PRIVMSG, recvData.target.c_str(), "2Adminok: %s", adminok.substr(2).c_str());
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hibás lekérdezés!");
	}
	else if(iras == added)
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		string nev = res[2];
		transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
		string pass = randomString(10, true, true, false);

		Sha1Hash hash;
		unsigned char* j_hash = new unsigned char[SHA_DIGEST_LENGTH+1];

		hash.Initialize();
		hash.UpdateData(pass);
		hash.Finalize();
		memcpy(j_hash, hash.GetDigest(), SHA_DIGEST_LENGTH);

		stringstream ss;
		const size_t len = 20;

		for(size_t u = 0; u < len; ++u)
			ss << std::hex << std::setw(2) << std::setfill('0') << cast_int(j_hash[u]);

		string jelszo_hash;
		ss >> jelszo_hash;

		m_SQLConn->Query("INSERT INTO `adminok`(nev, jelszo) VALUES ('%s', '%s')", nev.c_str(), jelszo_hash.c_str());
		m_SQLConn->Query("INSERT INTO `hluzenet`(nick, alapot) VALUES ('%s', 'ki')", nev.c_str());

		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Admin hozzáadva: %s", nev.c_str());
		SendChatMessage(PRIVMSG, nev.c_str(), "Mostantól Schumix adminja vagy. A te mostani jelszavad: %s", pass.c_str());
		SendChatMessage(PRIVMSG, nev.c_str(), "Ha megszeretnéd változtatni használd az %sujjelszo parancsot. Használata: %sujjelszo <régi> <új>", m_ParancsElojel.c_str(), m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, nev.c_str(), "Admin nick élesítése: %shozzaferes <jelszó>", m_ParancsElojel.c_str());
	}
	else if(iras == delet)
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		string nev = res[2];
		transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
		if(Admin(recvData.source_nick, Operator) && Admin(nev, Administrator))
		{
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nem vagy Adminisztrátor!", nev.c_str());
			res.clear();
			return;
		}

		m_SQLConn->Query("DELETE FROM `adminok` WHERE nev = '%s'", nev.c_str());
		m_SQLConn->Query("DELETE FROM `hluzenet` WHERE nick = '%s'", nev.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Admin törölve: %s", nev.c_str());
	}
	else if(iras == "rang")
	{
		if(res.size() < 4)
		{
			res.clear();
			return;
		}

		string nev = res[2];
		transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
		if(Admin(recvData.source_nick, Operator) && Admin(nev, Administrator))
		{
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nem vagy Adminisztrátor!", nev.c_str());
			res.clear();
			return;
		}

		stringstream ss;
		int rang;
		ss << res[3];
		ss >> rang;

		if(rang == Administrator || rang == Operator)
		{
			m_SQLConn->Query("UPDATE adminok SET flag = '%i' WHERE nev = '%s'", rang, nev.c_str());
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Rang sikeresen modósitva.");
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hibás rang!");
	}

	res.clear();
}

void IRCSession::Hozzaferes(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	QueryResultPointer db = m_SQLConn->Query("SELECT nev, jelszo FROM adminok WHERE nev = '%s'", recvData.source_nick.c_str());
	if(db)
	{
		string Nev = db->Fetch()[0].GetString();
		string JelszoSql = db->Fetch()[1].GetString();

		Sha1Hash hash;
		unsigned char* j_hash = new unsigned char[SHA_DIGEST_LENGTH+1];

		hash.Initialize();
		hash.UpdateData(res[1]);
		hash.Finalize();
		memcpy(j_hash, hash.GetDigest(), SHA_DIGEST_LENGTH);

		stringstream ss;
		const size_t len = 20;

		for(size_t u = 0; u < len; ++u)
			ss << std::hex << std::setw(2) << std::setfill('0') << cast_int(j_hash[u]);

		string jelszo_hash;
		ss >> jelszo_hash;

		if(JelszoSql == jelszo_hash)
		{
			m_SQLConn->Query("UPDATE adminok SET ip = '%s' WHERE nev = '%s'", recvData.source_host.c_str(), Nev.c_str());
			SendChatMessage(PRIVMSG, Nev.c_str(), "Hozzáférés engedélyezve");
		}
		else
			SendChatMessage(PRIVMSG, Nev.c_str(), "Hozzáférés megtagadva");
	}

	res.clear();
}

void IRCSession::Ujjelszo(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 3)
	{
		res.clear();
		return;
	}

	QueryResultPointer db = m_SQLConn->Query("SELECT nev, jelszo FROM adminok WHERE nev = '%s'", recvData.source_nick.c_str());
	if(db)
	{
		string Nev = db->Fetch()[0].GetString();
		string JelszoSql = db->Fetch()[1].GetString();
		string ujjelszo = res[2];

		Sha1Hash hash;
		unsigned char* j_hash = new unsigned char[SHA_DIGEST_LENGTH+1];

		hash.Initialize();
		hash.UpdateData(res[1]);
		hash.Finalize();
		memcpy(j_hash, hash.GetDigest(), SHA_DIGEST_LENGTH);

		stringstream ss;
		const size_t j_len = 20;

		for(size_t u = 0; u < j_len; ++u)
			ss << std::hex << std::setw(2) << std::setfill('0') << cast_int(j_hash[u]);

		string jelszo_hash;
		ss >> jelszo_hash;

		if(JelszoSql == jelszo_hash)
		{
			Sha1Hash sha1;
			unsigned char* u_hash = new unsigned char[SHA_DIGEST_LENGTH+1];

			sha1.Initialize();
			sha1.UpdateData(ujjelszo);
			sha1.Finalize();
			memcpy(u_hash, sha1.GetDigest(), SHA_DIGEST_LENGTH);

			stringstream gg;
			const size_t u_len = 20;

			for(size_t u = 0; u < u_len; ++u)
				gg << std::hex << std::setw(2) << std::setfill('0') << cast_int(u_hash[u]);

			string sql_hash;
			gg >> sql_hash;

			QueryResultPointer db = m_SQLConn->Query("UPDATE adminok SET jelszo = '%s' WHERE nev = '%s'", sql_hash.c_str(), Nev.c_str());
			SendChatMessage(PRIVMSG, Nev.c_str(), "Jelszó sikereset meg lett változtatva erre: %s", ujjelszo.c_str());
		}
		else
			SendChatMessage(PRIVMSG, Nev.c_str(), "A mostani jelszó nem egyezik, modósitás megtagadva");
	}

	res.clear();
}

void IRCSession::Funkciok(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string info = res[1];

	if(info == Help)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Alparancsok használata:");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel kezelés: %sfunkcio <be vagy ki> <funkcio név>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel kezelés máshonnét: %sfunkcio channel <channel név> <be vagy ki> <funkcio név>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Együtes kezelés: %sfunkcio all <be vagy ki> <funkcio név>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Update kezelése: %sfunkcio update", m_ParancsElojel.c_str());
	}
	else if(info == INFO)
	{
		string ChannelInfo = ChannelFunkciokInfo(recvData.target);
		int szokoz = ChannelInfo.find("|");

		SendChatMessage(PRIVMSG, recvData.target.c_str(), "2Bekapcsolva: %s", ChannelInfo.substr(0, szokoz).c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "2Kikapcsolva: %s", ChannelInfo.substr(szokoz+1).c_str());
	}
	else if(info == "all")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		if(res[2] == INFO)
		{
			QueryResultPointer db = m_SQLConn->Query("SELECT funkcio_nev, funkcio_status FROM schumix");
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

				SendChatMessage(PRIVMSG, recvData.target.c_str(), "2Bekapcsolva: %s", be.c_str());
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "2Kikapcsolva: %s", ki.c_str());
			}
			else
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hibás lekérdezés!");
		}

		if(res.size() < 4)
		{
			res.clear();
			return;
		}

		string status = res[2];

		if(status == bekapcsol || status == kikapcsol)
		{
			string nev = res[3];
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s: %skapcsolva", nev.c_str(), status.c_str());
			m_SQLConn->Query("UPDATE schumix SET funkcio_status = '%s' WHERE funkcio_nev = '%s'", status.c_str(), nev.c_str());
		}
	}
	else if(info == "channel")
	{
		if(res.size() < 4)
		{
			res.clear();
			return;
		}

		string channel = res[2];
		string status = res[3];

		if(status == INFO)
		{
			string ChannelInfo = ChannelFunkciokInfo(channel);
			int szokoz = ChannelInfo.find("|");

			SendChatMessage(PRIVMSG, recvData.target.c_str(), "2Bekapcsolva: %s", ChannelInfo.substr(0, szokoz).c_str());
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "2Kikapcsolva: %s", ChannelInfo.substr(szokoz+1).c_str());
		}
		else if(status == bekapcsol || status == kikapcsol)
		{
			if(res.size() < 5)
			{
				res.clear();
				return;
			}

			SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s: %skapcsolva", res[4].c_str(), status.c_str());
			m_SQLConn->Query("UPDATE channel SET funkciok = '%s' WHERE szoba = '%s'", ChannelFunkciok(res[4], status, channel).c_str(), channel.c_str());
			ChannelFunkcioReload();
		}
	}
	else if(info == update)
	{
		map<string, string>::iterator itr = m_ChannelLista.begin();
		for(; itr != m_ChannelLista.end(); itr++)
		{
			string szoba = itr->first;
			m_SQLConn->Query("UPDATE channel SET funkciok = ',%s:be,%s:be,%s:be,%s:be,%s:be' WHERE szoba = '%s'", KOSZONES, LOG, REJOIN, HL, PARANCSOK, szoba.c_str());
		}

		ChannelFunkcioReload();
	}
	else
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		string status = res[1];

		if(status == bekapcsol || status == kikapcsol)
		{
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s: %skapcsolva", res[2].c_str(), status.c_str());
			m_SQLConn->Query("UPDATE channel SET funkciok = '%s' WHERE szoba = '%s'", ChannelFunkciok(res[2], status, recvData.target).c_str(), recvData.target.c_str());
			ChannelFunkcioReload();
		}
	}

	res.clear();
}

void IRCSession::Channel(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string iras = res[1];

	if(iras == Help)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Alparancsok használata:");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hozzáadás: %schannel add <channel> <ha van pass akkor az>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Eltávolítás: %schannel del <channel>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Frissítés: %schannel update", m_ParancsElojel.c_str());
	}
	else if(iras == added)
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		string szoba = res[2];

		if(res.size() >= 4)
		{
			string jelszo = res[3];
			WriteLine("JOIN %s %s", szoba.c_str(), jelszo.c_str());
			m_SQLConn->Query("INSERT INTO `channel`(szoba, jelszo) VALUES ('%s', '%s')", szoba.c_str(), jelszo.c_str());
			m_SQLConn->Query("UPDATE channel SET aktivitas = 'aktiv' WHERE szoba = '%s'", szoba.c_str());

			m_ChannelPrivmsg = recvData.target;
		}
		else
		{
			WriteLine("JOIN %s", szoba.c_str());
			m_SQLConn->Query("INSERT INTO `channel`(szoba, jelszo) VALUES ('%s', '')", szoba.c_str());
			m_SQLConn->Query("UPDATE channel SET aktivitas = 'aktiv' WHERE szoba = '%s'", szoba.c_str());

			m_ChannelPrivmsg = recvData.target;
		}

		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel hozzáadva: %s", szoba.c_str());

		ChannelListaReload();
		ChannelFunkcioReload();
	}
	else if(iras == delet)
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		string szoba = res[2];
		WriteLine("PART %s", szoba.c_str());
		m_SQLConn->Query("DELETE FROM `channel` WHERE szoba = '%s'", szoba.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel eltávolítva: %s", szoba.c_str());

		ChannelListaReload();
		ChannelFunkcioReload();
	}
	else if(iras == update)
	{
		ChannelListaReload();
		ChannelFunkcioReload();
	}
	else if(iras == INFO)
	{
		QueryResultPointer db = m_SQLConn->Query("SELECT szoba, aktivitas, error FROM channel");
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
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Aktiv: %s", Aktivszobak.substr(2).c_str());
			else
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Aktiv: Nincs adat.");
			if(adatszoba1)
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Deaktiv: %s", DeAktivszobak.substr(2).c_str());
			else
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Deaktiv: Nincs adat.");
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hibás lekérdezés!");
	}

	res.clear();
}

void IRCSession::Sznap(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	QueryResultPointer db = m_SQLConn->Query("SELECT nev, honap, honap1, nap FROM sznap WHERE nev = '%s'", res[1].c_str());
	if(db)
	{
		string nev = db->Fetch()[0].GetString();
		string honap = db->Fetch()[1].GetString();
		uint32 honap1 = db->Fetch()[2].GetUInt32();
		uint32 nap = db->Fetch()[3].GetUInt32();

		SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s születés napja: %s %u", nev.c_str(), honap.c_str(), nap);
	}
	else
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs ilyen ember.");

	res.clear();
}

void IRCSession::Nick(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string nick = res[1];
	m_NickTarolo = nick;
	WriteLine("NICK %s", nick.c_str());

	res.clear();
}

void IRCSession::Join(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() == 2)
	{
		WriteLine("JOIN %s", res[1].c_str());
		m_ChannelPrivmsg = recvData.target;
	}
	else if(res.size() >= 3)
	{
		WriteLine("JOIN %s %s", res[1].c_str(), res[2].c_str());
		m_ChannelPrivmsg = recvData.target;
	}

	res.clear();
}

void IRCSession::Left(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	WriteLine("PART %s", res[1].c_str());
	res.clear();
}

void IRCSession::Kick(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string kick = res[1];
	string Nick = m_NickTarolo;
	transform(Nick.begin(), Nick.end(), Nick.begin(), ::tolower);
	transform(kick.begin(), kick.end(), kick.begin(), ::tolower);
	int szam = res.size();

	if(szam == 2)
	{
		if(kick != Nick)
			WriteLine("KICK %s %s", recvData.target.c_str(), kick.c_str());
	}
	else if(szam >= 3)
	{
		string oka;
		int resAdat = res.size();

		for(int i = 2; i < resAdat; i++)
			oka += " " + res[i];

		if(kick != Nick)
			WriteLine("KICK %s %s :%s", recvData.target.c_str(), kick.c_str(), oka.substr(1).c_str());
	}

	res.clear();
}

void IRCSession::Mode(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 3)
	{
		res.clear();
		return;
	}

	string rang = res[1];
	string nev = res[2];
	string nevek;
	int resAdat = res.size();

	for(int i = 2; i < resAdat; i++)
		nevek += " " + res[i];

	WriteLine("MODE %s %s %s", recvData.target.c_str(), rang.c_str(), nevek.substr(1).c_str());

	res.clear();
}

void IRCSession::HLFunkcio(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	if(res[1] == Help)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Alparancsok használata:");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nick lista frissitése: %shluzenet update", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hl üzenet hozzáadása: %shluzenet <üzenet>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hl üzenet küldés állitása: %shluzenet funkcio <állapot>", m_ParancsElojel.c_str());
	}
	if(res[1] == INFO)
	{
		QueryResultPointer db = m_SQLConn->Query("SELECT nick, alapot FROM hluzenet");
		if(db)
		{
			string Nevek;

			do
			{
				string nev = db->Fetch()[0].GetString();
				string alapot = db->Fetch()[1].GetString();
				Nevek += ", " + nev + ":" + alapot;
			} while(db->NextRow());

			SendChatMessage(PRIVMSG, recvData.target.c_str(), "3Létezõ nickek: %s", Nevek.substr(2).c_str());
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hibás lekérdezés!");
	}
	if(res[1] == update)
	{
	// Hibás a kod egyenlõre nem lehet használni
	/*
		QueryResultPointer db = m_SQLConn->Query("SELECT MAX(id) FROM hluzenet");
		if(db)
		{
			uint32 id = db->Fetch()[0].GetUInt32();
			m_SQLConn->kiiras = false;

			for(int x = 0; x < id; x++)
				m_SQLConn->Query("DELETE FROM `hluzenet` WHERE id = '%u'", id);

			m_SQLConn->kiiras = true;
		}

		QueryResultPointer db1 = m_SQLConn->Query("SELECT nev FROM adminok");
		if(db1)
		{
			do
			{
				string nev = db1->Fetch()[0].GetString();
				transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
				m_SQLConn->Query("INSERT INTO `hluzenet`(nick, alapot) VALUES ('%s', 'ki')", nev.c_str());
			} while(db1->NextRow());
		}
	*/
	}
	if(res[1] == "funkcio")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		string nev = recvData.source_nick;
		transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
		m_SQLConn->Query("UPDATE `hluzenet` SET `alapot` = '%s' WHERE nick = '%s'", res[2].c_str(), nev.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s: %skapcsolva", nev.c_str(), res[2].c_str());
	}
	else
	{
		string alomany;
		int resAdat = res.size();

		for(int i = 1; i < resAdat; i++)
			alomany += " " + res[i];

		string nev = recvData.source_nick;
		transform(nev.begin(), nev.end(), nev.begin(), ::tolower);
		m_SQLConn->Query("UPDATE `hluzenet` SET `info` = '%s', `alapot` = 'be' WHERE nick = '%s'", alomany.substr(1).c_str(), nev.c_str());
		m_SQLConn->Query("UPDATE `schumix` SET `funkcio_status` = 'be' WHERE funkcio_nev = '%s'", HL);

		res.clear();
	}
}

void IRCSession::Svn(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string info = res[1];

	if(info == Help)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Alparancsok használata:");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Választható emuk: %ssvn lista", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hozzáadás: %ssvn add <emu> <channel>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Eltávolítás: %ssvn del <emu> <channel>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "New thread: %ssvn new <emu>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Stop thread: %ssvn stop <emu>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Reload all thread: %ssvn reload all", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Reload thread: %ssvn reload <emu>", m_ParancsElojel.c_str());
	}
	else if(info == INFO)
	{
		QueryResultPointer db = m_SQLConn->Query("SELECT nev, channel FROM svninfo");
		if(db)
		{
			do
			{
				string nev = db->Fetch()[0].GetString();
				string channel = db->Fetch()[1].GetString();
				vector<string> reschannel(1);
				sVezerlo.split(channel, ",", reschannel);

				if(reschannel.size() < 2)
				{
					reschannel.clear();
					continue;
				}

				string adat;
				int resChannel = reschannel.size();

				for(int y = 1; y < resChannel; y++)
					adat += " " + reschannel[y];

				SendChatMessage(PRIVMSG, recvData.target.c_str(), "3%s Channel:2%s", nev.c_str(), adat.c_str());
				reschannel.clear();
			} while(db->NextRow());
		}
	}
	else if(info == "lista")
	{
		QueryResultPointer db = m_SQLConn->Query("SELECT nev FROM svninfo");
		if(db)
		{
			string lista;

			do
			{
				string nev = db->Fetch()[0].GetString();
				lista += " " + nev;
			} while(db->NextRow());

			SendChatMessage(PRIVMSG, recvData.target.c_str(), "2Lista: 3%s", lista.substr(1).c_str());
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hibás lekérdezés!");
	}
	else if(info == "new")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		QueryResultPointer db = m_SQLConn->Query("SELECT id FROM svninfo WHERE nev = '%s'", res[2].c_str());
		if(db)
		{
			sSvnInfo.NewThread(db->Fetch()[0].GetUInt32());
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s thread hozzáadva", res[2].c_str());
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Sikertelen hozzáadás!");
	}
	else if(info == "stop")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		QueryResultPointer db = m_SQLConn->Query("SELECT id FROM svninfo WHERE nev = '%s'", res[2].c_str());
		if(db)
		{
			sSvnInfo.StopThread(db->Fetch()[0].GetUInt32());
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s thread leállitva", res[2].c_str());
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Sikertelen leállitás!");
	}
	else if(info == "reload")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		if(res[2] == "all")
		{
			sSvnInfo.ReloadAllThread();
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "SvnInfo újrainditás kész.");
		}
		else
		{
			QueryResultPointer db = m_SQLConn->Query("SELECT id FROM svninfo WHERE nev = '%s'", res[2].c_str());
			if(db)
			{
				sSvnInfo.ReloadThread(db->Fetch()[0].GetUInt32());
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s thread újrainditva.", res[2].c_str());
			}
			else
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Sikertelen újrainditás!");
		}
	}
	else
	{
		if(info == added)
		{
			if(res.size() < 4)
			{
				res.clear();
				return;
			}

			QueryResultPointer db = m_SQLConn->Query("SELECT channel FROM svninfo WHERE nev = '%s'", res[2].c_str());
			if(db)
			{
				string channel = db->Fetch()[0].GetString();
				vector<string> reschannel(1);
				sVezerlo.split(channel, ",", reschannel);

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
				m_SQLConn->Query("UPDATE svninfo SET channel = '%s' WHERE nev = '%s'", adat.c_str(), res[2].c_str());
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel sikeresen hozzáadva.");
			}
			else
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel hozzáadása sikertelen!");
		}
		else if(info == delet)
		{
			if(res.size() < 4)
			{
				res.clear();
				return;
			}

			QueryResultPointer db = m_SQLConn->Query("SELECT channel FROM svninfo WHERE nev = '%s'", res[2].c_str());
			if(db)
			{
				string channel = db->Fetch()[0].GetString();
				vector<string> reschannel(1);
				sVezerlo.split(channel, ",", reschannel);

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
				m_SQLConn->Query("UPDATE svninfo SET channel = '%s' WHERE nev = '%s'", adat.c_str(), res[2].c_str());
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel sikeresen törölve.");
			}
			else
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel törlése sikertelen!");
		}
	}

	res.clear();
}

void IRCSession::Git(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string info = res[1];

	if(info == Help)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Alparancsok használata:");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Választható emuk: %sgit lista", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hozzáadás: %sgit add <nev> <tipus> <channel>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Eltávolítás: %sgit del <nev> <tipus> <channel>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "New thread: %sgit new <nev> <tipus>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Stop thread: %sgit stop <nev> <tipus>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Reload all thread: %sgit reload all", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Reload thread: %sgit reload <nev> <tipus>", m_ParancsElojel.c_str());
	}
	else if(info == INFO)
	{
		QueryResultPointer db = m_SQLConn->Query("SELECT nev, tipus, channel FROM gitinfo");
		if(db)
		{
			do
			{
				string nev = db->Fetch()[0].GetString();
				string tipus = db->Fetch()[1].GetString();
				string channel = db->Fetch()[2].GetString();
				vector<string> reschannel(1);
				sVezerlo.split(channel, ",", reschannel);

				if(reschannel.size() < 2)
				{
					reschannel.clear();
					continue;
				}

				string adat;
				int resChannel = reschannel.size();

				for(int y = 1; y < resChannel; y++)
					adat += " " + reschannel[y];

				SendChatMessage(PRIVMSG, recvData.target.c_str(), "3%s 7%s Channel:2%s", nev.c_str(), tipus.c_str(), adat.c_str());
				reschannel.clear();
			} while(db->NextRow());
		}
	}
	else if(info == "lista")
	{
		QueryResultPointer db = m_SQLConn->Query("SELECT nev, tipus FROM gitinfo");
		if(db)
		{
			string lista;

			do
			{
				string nev = db->Fetch()[0].GetString();
				string tipus = db->Fetch()[1].GetString();

				lista += " " + nev + " " + tipus + ";";
			} while(db->NextRow());

			SendChatMessage(PRIVMSG, recvData.target.c_str(), "2Lista: 3%s", lista.substr(1).c_str());
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hibás lekérdezés!");
	}
	else if(info == "new")
	{
		if(res.size() < 4)
		{
			res.clear();
			return;
		}

		QueryResultPointer db = m_SQLConn->Query("SELECT id FROM gitinfo WHERE nev = '%s' AND tipus = '%s'", res[2].c_str(), res[3].c_str());
		if(db)
		{
			sGitInfo.NewThread(db->Fetch()[0].GetUInt32());
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s %s thread hozzáadva", res[2].c_str(), res[3].c_str());
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Sikertelen hozzáadás!");
	}
	else if(info == "stop")
	{
		if(res.size() < 4)
		{
			res.clear();
			return;
		}

		QueryResultPointer db = m_SQLConn->Query("SELECT id FROM gitinfo WHERE nev = '%s' AND tipus = '%s'", res[2].c_str(), res[3].c_str());
		if(db)
		{
			sGitInfo.StopThread(db->Fetch()[0].GetUInt32());
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s %s thread leállitva", res[2].c_str(), res[3].c_str());
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Sikertelen leállitás!");
	}
	else if(info == "reload")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		if(res[2] == "all")
		{
			sGitInfo.ReloadAllThread();
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "GitInfo újrainditás kész.");
		}
		else
		{
			if(res.size() < 4)
			{
				res.clear();
				return;
			}

			QueryResultPointer db = m_SQLConn->Query("SELECT id FROM gitinfo WHERE nev = '%s' AND tipus = '%s'", res[2].c_str(), res[3].c_str());
			if(db)
			{
				sGitInfo.ReloadThread(db->Fetch()[0].GetUInt32());
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s %s thread újrainditva.", res[2].c_str(), res[3].c_str());
			}
			else
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Sikertelen újrainditás!");
		}
	}
	else
	{
		if(info == added)
		{
			if(res.size() < 5)
			{
				res.clear();
				return;
			}

			QueryResultPointer db = m_SQLConn->Query("SELECT id, channel FROM gitinfo WHERE nev = '%s' AND tipus = '%s'", res[2].c_str(), res[3].c_str());
			if(db)
			{
				string channel = db->Fetch()[1].GetString();
				vector<string> reschannel(1);
				sVezerlo.split(channel, ",", reschannel);

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
				m_SQLConn->Query("UPDATE gitinfo SET channel = '%s' WHERE id = '%u'", adat.c_str(), db->Fetch()[0].GetUInt32());
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel sikeresen hozzáadva.");
			}
			else
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel hozzáadása sikertelen!");
		}
		else if(info == delet)
		{
			if(res.size() < 5)
			{
				res.clear();
				return;
			}

			QueryResultPointer db = m_SQLConn->Query("SELECT id, channel FROM gitinfo WHERE nev = '%s' AND tipus = '%s'", res[2].c_str(), res[3].c_str());
			if(db)
			{
				string channel = db->Fetch()[1].GetString();
				vector<string> reschannel(1);
				sVezerlo.split(channel, ",", reschannel);

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
				m_SQLConn->Query("UPDATE gitinfo SET channel = '%s' WHERE id = '%u'", adat.c_str(), db->Fetch()[0].GetUInt32());
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel sikeresen törölve.");
			}
			else
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel törlése sikertelen!");
		}
	}

	res.clear();
}

void IRCSession::Hg(IRCMessage& recvData)
{
	if(!Admin(recvData.source_nick, recvData.source_host, Operator))
		return;

	if(recvData.args.length() <= firstSpace+1)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Nincs paraméter!");
		return;
	}

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string info = res[1];

	if(info == Help)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Alparancsok használata:");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Választható emuk: %shg lista", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hozzáadás: %shg add <emu> <channel>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Eltávolítás: %shg del <emu> <channel>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "New thread: %shg new <emu>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Stop thread: %shg stop <emu>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Reload all thread: %shg reload all", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Reload thread: %shg reload <emu>", m_ParancsElojel.c_str());
	}
	else if(info == INFO)
	{
		QueryResultPointer db = m_SQLConn->Query("SELECT nev, channel FROM hginfo");
		if(db)
		{
			do
			{
				string nev = db->Fetch()[0].GetString();
				string channel = db->Fetch()[1].GetString();
				vector<string> reschannel(1);
				sVezerlo.split(channel, ",", reschannel);

				if(reschannel.size() < 2)
				{
					reschannel.clear();
					continue;
				}

				string adat;
				int resChannel = reschannel.size();

				for(int y = 1; y < resChannel; y++)
					adat += " " + reschannel[y];

				SendChatMessage(PRIVMSG, recvData.target.c_str(), "3%s Channel:2%s", nev.c_str(), adat.c_str());
				reschannel.clear();
			} while(db->NextRow());
		}
	}
	else if(info == "lista")
	{
		QueryResultPointer db = m_SQLConn->Query("SELECT nev FROM hginfo");
		if(db)
		{
			string lista;

			do
			{
				string nev = db->Fetch()[0].GetString();
				lista += " " + nev;
			} while(db->NextRow());

			SendChatMessage(PRIVMSG, recvData.target.c_str(), "2Lista: 3%s", lista.substr(1).c_str());
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hibás lekérdezés!");
	}
	else if(info == "new")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		QueryResultPointer db = m_SQLConn->Query("SELECT id FROM hginfo WHERE nev = '%s'", res[2].c_str());
		if(db)
		{
			sHgInfo.NewThread(db->Fetch()[0].GetUInt32());
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s thread hozzáadva", res[2].c_str());
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Sikertelen hozzáadás!");
	}
	else if(info == "stop")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		QueryResultPointer db = m_SQLConn->Query("SELECT id FROM hginfo WHERE nev = '%s'", res[2].c_str());
		if(db)
		{
			sHgInfo.StopThread(db->Fetch()[0].GetUInt32());
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s thread leállitva", res[2].c_str());
		}
		else
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "Sikertelen leállitás!");
	}
	else if(info == "reload")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		if(res[2] == "all")
		{
			sHgInfo.ReloadAllThread();
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "HgInfo újrainditás kész.");
		}
		else
		{
			QueryResultPointer db = m_SQLConn->Query("SELECT id FROM hginfo WHERE nev = '%s'", res[2].c_str());
			if(db)
			{
				sHgInfo.ReloadThread(db->Fetch()[0].GetUInt32());
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s thread újrainditva.", res[2].c_str());
			}
			else
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Sikertelen újrainditás!");
		}
	}
	else
	{
		if(info == added)
		{
			if(res.size() < 4)
			{
				res.clear();
				return;
			}

			QueryResultPointer db = m_SQLConn->Query("SELECT channel FROM hginfo WHERE nev = '%s'", res[2].c_str());
			if(db)
			{
				string channel = db->Fetch()[0].GetString();
				vector<string> reschannel(1);
				sVezerlo.split(channel, ",", reschannel);

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
				m_SQLConn->Query("UPDATE hginfo SET channel = '%s' WHERE nev = '%s'", adat.c_str(), res[2].c_str());
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel sikeresen hozzáadva.");
			}
			else
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel hozzáadása sikertelen!");
		}
		else if(info == delet)
		{
			if(res.size() < 4)
			{
				res.clear();
				return;
			}

			QueryResultPointer db = m_SQLConn->Query("SELECT channel FROM hginfo WHERE nev = '%s'", res[2].c_str());
			if(db)
			{
				string channel = db->Fetch()[0].GetString();
				vector<string> reschannel(1);
				sVezerlo.split(channel, ",", reschannel);

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
				m_SQLConn->Query("UPDATE hginfo SET channel = '%s' WHERE nev = '%s'", adat.c_str(), res[2].c_str());
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel sikeresen törölve.");
			}
			else
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Channel törlése sikertelen!");
		}
	}

	res.clear();
}
