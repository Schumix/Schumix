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

void CommandMgr::HandleXbot(CommandMessage& recvData)
{
	CNick(recvData);

	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Verzió: 10%s", revision);
	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Parancsok: %sinfo | %shelp | %sxrev | %sido | %sdatum | %sirc | %sroll | %scalc | %skeres | %sfordit | %ssha1 | %smd5 | %suzenet | %swhois | %sjegyzet | %sprime", sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel());
	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Programmed by: 3Csaba");
}

void CommandMgr::HandleInfo(CommandMessage& recvData)
{
	CNick(recvData);
	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Programozóm: Csaba, Twl és Scymex. Üzemeltetõ: %s.", sIRCSession.GetUzemelteto());
}

void CommandMgr::HandleRoll(CommandMessage& recvData)
{
	CNick(recvData);
	uint32 adat = rand()%101;
	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Százalékos aránya %u!", adat);
}

static uint8 Honapok[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

static const char* Nevnap[12][31] = {
	{ "ÚJÉV","Ábel","Genovéva","Titusz","Simon","Boldizsár","Attila","Gyöngyvér","Marcell","Melánia","Ágota","Ernõ","Veronika","Bódog","Lóránt","Gusztáv","Antal","Piroska","Sára","Sebestyén","Ágnes","Vince","Zelma","Timót","Pál","Vanda","Angelika","Károly,","Adél","Martina","Marcella" },
	{ "Ignác","Karolina","Balázs","Ráhel","Ágota","Dóra","Tódor","Aranka","Abigél","Elvira","Bertold","Lívia","Ella, Linda","Bálint","Kolos","Julianna","Donát","Bernadett","Zsuzsanna","Álmos","Eleonóra","Gerzson","Alfréd","Mátyás","Géza","Edina","Ákos, Bátor","Elemér","","","" },
	{ "Albin","Lujza","Kornélia","Kázmér","Adorján","Leonóra","Tamás","Zoltán","Franciska","Ildikó","Szilárd","Gergely","Krisztián, Ajtony","Matild","Kristóf","Henrietta","Gertrúd","Sándor","József","Klaudia","Benedek","Beáta","Emõke","Gábor","Irén","Emánuel","Hajnalka","Gedeon","Auguszta","Zalán","Árpád" },
	{ "Hugó","Áron","Buda, Richárd","Izidor","Vince","Vilmos, Bíborka","Herman","Dénes","Erhard","Zsolt","Zsolt, Leó","Gyula","Ida","Tibor","Tas, Anasztázia","Csongor","Rudolf","Andrea","Emma","Konrád, Tivadar","Konrád","Csilla","Béla","György","Márk","Ervin","Zita","Valéria","Péter","Katalin, Kitti","" },
	{ "Fülöp","Zsigmond","Tímea","Mónika","Györgyi","Ivett","Gizella","Mihály","Gergely","Ármin","Ferenc","Pongrác","Szervác","Bonifác","Zsófia","Botond, Mózes","Paszkál","Erik","Ivó, Milán","Bernát, Felícia","Konstantin","Júlia, Rita","Dezsõ","Eszter","Orbán","Fülöp","Hella","Emil, Csanád","Magdolna","Zsanett, Janka","Angéla" },
	{ "Tünde","Anita, Kármen","Klotild","Bulcsú","Fatime","Norbert","Róbert","Medárd","Félix","Margit","Barnabás","Villõ","Antal, Anett","Vazul","Jolán","Jusztin","Laura","Levente","Gyárfás","Rafael","Alajos","Paulina","Zoltán","Iván","Vilmos","János","László","Levente, Irén","Péter, Pál","Pál","" },
	{ "Annamária","Ottó","Kornél","Ulrik","Sarolta, Emese","Csaba","Appolónia","Ellák","Lukrécia","Amália","Nóra, Lili","Izabella","Jenõ","&Õrs","Henrik","Valter","Endre, Elek","Frigyes","Emília","Illés","Dániel","Magdolna","Lenke","Kinga, Kincsõ","Kristóf, Jakab","Anna, Anikó","Olga","Szabolcs","Márta","Judit","Oszkár" },
	{ "Boglárka","Lehel","Hermina","Domonkos","Krisztina","Berta","Ibolya","László","Emõd","Lörinc","Zsuzsanna","Klára","Ipoly","Marcell","Mária","Ábrahám","Jácint","Ilona","Huba","István","Sámuel","Menyhért","Bence","Bertalan","Lajos","Izsó","Gáspár","Ágoston","Beatrix","Rózsa","Erika" },
	{ "Egon","Rebeka","Hilda","Rozália","Viktor, Lõrinc","Zakariás","Regina","Mária","Ádám","Nikolett, Hunor","Teodóra","Mária","Kornél","Szeréna","Enikõ","Edit","Zsófia","Diána","Vilhelmina","Friderika","Máté","Móric","Tekla","Gellért","Eufrozina","Jusztina","Adalbert","Vencel","Mihály","Jeromos","" },
	{ "Malvin","Petra","Helga","Ferenc","Aurél","Renáta","Amália","Koppány","Dénes","Gedeon","Brigitta","Miksa","Kálmán","Helén","Teréz","Gál","Hedvig","Lukács","Nándor","Vendel","Orsolya","Elõd","Gyöngyi","Salamon","Bianka","Dömötör","Szabina","Simon","Nárcisz","Alfonz","Farkas" },
	{ "Marianna","Achilles","Gyõzõ","Károly","Imre","Lénárd","Rezsõ","Zsombor","Tivadar","Réka","Márton","Jónás, Renátó","Szilvia","Aliz","Albert, Lipót","Ödön","Hortenzia, Gergõ","Jenõ","Erzsébet","Jolán","Olivér","Cecília","Kelemen","Emma","Katalin","Virág","Virgil","Stefánia","Taksony","András, Andor","" },
	{ "Elza","Melinda","Ferenc","Barbara, Borbála","Vilma","Miklós","Ambrus","Mária","Natália","Judit","Árpád","Gabriella","Luca","Szilárda","Valér","Etelka","Lázár","Auguszta","Viola","Teofil","Tamás","Zéno","Viktória","Ádám, Éva","KARÁCSONY","KARÁCSONY","János","Kamilla","Tamás","Dávid","Szilveszter" },
};

void CommandMgr::HandleDatum(CommandMessage& recvData)
{
	CNick(recvData);
	int honap = Honap();
	int nap = Nap();

	for(uint8 x = 0; x < 12; x++)
	{
		uint8 honapdb = Honapok[x];
		string napdb = Nevnap[x][nap-1];

		if(honap == honapdb)
		{
			if(honap < 10)
			{
				if(nap < 10)
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ma %i. 0%i. 0%i. %s napja van.", Ev(), honap, nap, napdb.c_str());
				else
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ma %i. 0%i. %i. %s napja van.", Ev(), honap, nap, napdb.c_str());
			}
			else
			{
				if(nap < 10)
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ma %i. %i. 0%i. %s napja van.", Ev(), honap, nap, napdb.c_str());
				else
					sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Ma %i. %i. %i. %s napja van.", Ev(), honap, nap, napdb.c_str());
			}
		}
 	}
}

void CommandMgr::HandleIdo(CommandMessage& recvData)
{
	CNick(recvData);
	int perc = Perc();

	if(perc < 10)
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Helyi idõ: %i:0%i", Ora(), perc);
	else
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Helyi idõ: %i:%i", Ora(), perc);
}

void CommandMgr::HandleKeres(CommandMessage& recvData)
{
	CNick(recvData);

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	string kiiras = sVezerlo.urlencode(recvData.Args.substr(recvData.firstSpace+1));

	m_Curl = curl_easy_init();
	if(m_Curl)
	{
		string bufferdata;

		curl_easy_setopt(m_Curl, CURLOPT_URL, format("http://ajax.googleapis.com/ajax/services/search/web?v=1.0&start=0&rsz=small&q=%s", kiiras.c_str()).c_str());
		curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, CommandMgr::writer);
		curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, &bufferdata);
		CURLcode result = curl_easy_perform(m_Curl);

		curl_easy_cleanup(m_Curl);

		if(result == CURLE_OK)
		{
			boost::regex re("(\\\"unescapedUrl\\\"):\\\"(?<url>\\S+)\\\",\\\"url\\\"");
			boost::cmatch matches;

			boost::regex_search(bufferdata.c_str(), matches, re);
			string matched(matches[2].first, matches[2].second);

			boost::regex re1(".titleNoFormatting.:.(.*).,.content.:.");
			boost::cmatch matches1;

			boost::regex_search(bufferdata.c_str(), matches1, re1);
			string matched1(matches1[1].first, matches1[1].second);

			int szokoz = matched1.find("\",\"");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "2Title: %s", matched1.substr(0, szokoz).c_str());
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "2Link: 9%s", matched.c_str());
		}
		else
		{
			Log.Notice("IRCSession", "Keres: Hiba a Http lekerdezesben.");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás találat");
		}
	}
	else
		curl_easy_cleanup(m_Curl);
}

void CommandMgr::HandleForditas(CommandMessage& recvData)
{
	CNick(recvData);

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

	if(res.size() < 3)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs egy szó se megadva amit lekéne fordítani!");
		res.clear();
		return;
	}

	string nyelv = res[1];
	string alomany;
	int resAdat = res.size();

	for(int i = 2; i < resAdat; i++)
		alomany += " " + res[i];

	string iras = sVezerlo.urlencode(alomany.substr(1));

	m_Curl = curl_easy_init();
	if(m_Curl)
	{
		string bufferdata;

		curl_easy_setopt(m_Curl, CURLOPT_URL, format("http://ajax.googleapis.com/ajax/services/language/translate?v=1.0&q=%s&langpair=%s", iras.c_str(), nyelv.c_str()).c_str());
		curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, CommandMgr::writer);
		curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, &bufferdata);

		CURLcode result = curl_easy_perform(m_Curl);

		curl_easy_cleanup(m_Curl);

		if(result == CURLE_OK)
		{
			boost::regex re("\\{.translatedText.\\:.(.+).\\},");
			boost::cmatch matches;

			boost::regex_search(bufferdata.c_str(), matches, re);
			string matched(matches[1].first, matches[1].second);

			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s", matched.c_str());
		}
		else
		{
			Log.Notice("IRCSession", "Fordit: Hiba a Http lekerdezesben.");
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás fordítás");
		}
	}
	else
		curl_easy_cleanup(m_Curl);

	res.clear();
}

void CommandMgr::HandleSha1(CommandMessage& recvData)
{
	CNick(recvData);

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s", sVezerlo.Sha1(recvData.Args.substr(recvData.firstSpace+1)).c_str());
}

void CommandMgr::HandleMd5(CommandMessage& recvData)
{
	CNick(recvData);

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	MD5Hash md5;
	unsigned char* eredmeny = new unsigned char[MD5_DIGEST_LENGTH+1];

	md5.Initialize();
	md5.UpdateData(recvData.Args.substr(recvData.firstSpace+1));
	md5.Finalize();
	memcpy(eredmeny, md5.GetDigest(), MD5_DIGEST_LENGTH);

	stringstream ss;
	const size_t len = 16;

	for(size_t u = 0; u < len; ++u)
		ss << std::hex << std::setw(2) << std::setfill('0') << cast_int(eredmeny[u]);

	string hash;
	ss >> hash;

	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s", hash.c_str());
}

void CommandMgr::HandleXrev(CommandMessage& recvData)
{
	CNick(recvData);

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Emulátorok: sandshroud");
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

	if(iras == "sandshroud")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "A revision nincs megadva!");
			res.clear();
			return;
		}

		int rev;
		stringstream ss;
		ss << res[2];
		ss >> rev;

		sIRCSession.SvnSandshroud(rev, recvData.Channel);
	}
	else
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hibás név!");

	res.clear();
}

void CommandMgr::HandleIrc(CommandMessage& recvData)
{
	CNick(recvData);

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Parancsok: %sirc rang | %sirc rang1 | %sirc nick | %sirc kick | %sirc owner", sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel());
		return;
	}

	vector<string> res(1);
	split(recvData.Args.substr(recvData.firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT hasznalata FROM irc_parancsok WHERE parancs = '%s'", sVezerlo.GetSQLConn()->EscapeString(res[1]).c_str());
	if(db)
	{
		string hasznalata = db->Fetch()[0].GetString();
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s", hasznalata.c_str());
	}

	res.clear();
}

void CommandMgr::HandleSzam(CommandMessage& recvData)
{
	CNick(recvData);

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	sIRCSession.setConsts();
	sIRCSession.makepolishform(recvData.Args.substr(recvData.firstSpace+1));
	sIRCSession.calculate(recvData.Channel);
}

void CommandMgr::HandleUzenet(CommandMessage& recvData)
{
	CNick(recvData);

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

	string alomany;
	int resAdat = res.size();

	for(int i = 2; i < resAdat; i++)
		alomany += " " + res[i];

	if(resAdat == 2)
		sIRCSession.SendChatMessage(PRIVMSG, iras.c_str(), "Keresnek téged itt: %s", recvData.GetChannel());
	else if(resAdat >= 3)
		sIRCSession.SendChatMessage(PRIVMSG, iras.c_str(), "%s", alomany.substr(1).c_str());

	res.clear();
}

void CommandMgr::HandleJegyzet(CommandMessage& recvData)
{
	CNick(recvData);

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

	if(info == "kod")
	{
		if(res.size() < 3)
		{
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "A kod nincs megadva!");
			res.clear();

			return;
		}

		QueryResultPointer db = sVezerlo.GetSQLConn()->Query("SELECT jegyzet FROM jegyzetek WHERE kod = '%s'", sVezerlo.GetSQLConn()->EscapeString(res[2]).c_str());
		if(db)
		{
			string jegyzet = db->Fetch()[0].GetString();
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetNick(), "Jegyzet: %s", jegyzet.c_str());
		}
	}
	else
	{
		string alomany;
		int resAdat = res.size();

		for(int i = 1; i < resAdat; i++)
			alomany += " " + res[i];

		string kod = sIRCSession.RandomString(10, true, true, false);
		sVezerlo.GetSQLConn()->Query("INSERT INTO `jegyzetek`(kod, nick, jegyzet) VALUES ('%s', '%s', '%s')", kod.c_str(), recvData.GetNick(), sVezerlo.GetSQLConn()->EscapeString(alomany.substr(1)).c_str());
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetNick(), "Jegyzet kodja: %s", kod.c_str());
	}

	res.clear();
}

void CommandMgr::HandleWhois(CommandMessage& recvData)
{
	CNick(recvData);

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	string Nick = sIRCSession.m_NickTarolo;
	string nick = recvData.Args.substr(recvData.firstSpace+1);
	transform(Nick.begin(), Nick.end(), Nick.begin(), ::tolower);
	transform(nick.begin(), nick.end(), nick.begin(), ::tolower);

	if(nick == Nick)
		return;

	sIRCSession.m_WhoisPrivmsg = recvData.Channel;
	sIRCSession.WriteLine("WHOIS %s", recvData.Args.substr(recvData.firstSpace+1).c_str());
}

void CommandMgr::HandlePrime(CommandMessage& recvData)
{
	CNick(recvData);

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs paraméter!");
		return;
	}

	uint32 prim;
	stringstream ss;
	ss << recvData.Args.substr(recvData.firstSpace+1);
	ss >> prim;

	if(!isPrime(prim))
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%u nem primszám.", prim);
	else
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%u primszám.", prim);
}
