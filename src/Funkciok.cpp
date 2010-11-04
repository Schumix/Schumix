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

void IRCSession::Logfajl(IRCMessage& recvData)
{
	if(FSelect(LOG) == bekapcsol && FSelectChannel(LOG, recvData.target) == bekapcsol)
	{
		char* log = new char[5000];
		snprintf(log, 5000, "[%i. %i. %i. %i:%i] <%s> %s\n", sVezerlo.Ev(), sVezerlo.Honap(), sVezerlo.Nap(), sVezerlo.Ora(), sVezerlo.Perc(), recvData.source_nick.c_str(), recvData.args.c_str());

		char* fajl = new char[30];
		snprintf(fajl, 30, "%s/%s.log", LogHelye, recvData.target.c_str());

		FILE* LogSzoba = fopen(fajl, "a+");
		if(!LogSzoba || LogSzoba == NULL)
		{
			Log.Error("Log", "Sikertelen olvasas.\n");
			return;
		}

		//utf8printf(LogSzoba, "%s", log);
		fprintf(LogSzoba, "%s", log);
		fclose(LogSzoba);
		delete[] log;
		delete[] fajl;
	}
#ifdef _DEBUG_MOD
	else
		Log.Warning("Funkcio", "A %s funkcio nem uzemel!", LOG);
#endif
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

void IRCSession::Datum(IRCMessage& recvData)
{
	int honap = sVezerlo.Honap();
	int nap = sVezerlo.Nap();

	for(uint8 x = 0; x < 12; x++)
	{
		uint8 honapdb = Honapok[x];
		string napdb = Nevnap[x][nap-1];

		if(honap == honapdb)
		{
			if(honap < 10)
			{
				if(nap < 10)
					SendChatMessage(PRIVMSG, recvData.target.c_str(), "Ma %i. 0%i. 0%i. %s napja van.", sVezerlo.Ev(), honap, nap, napdb.c_str());
				else
					SendChatMessage(PRIVMSG, recvData.target.c_str(), "Ma %i. 0%i. %i. %s napja van.", sVezerlo.Ev(), honap, nap, napdb.c_str());
			}
			else
			{
				if(nap < 10)
					SendChatMessage(PRIVMSG, recvData.target.c_str(), "Ma %i. %i. 0%i. %s napja van.", sVezerlo.Ev(), honap, nap, napdb.c_str());
				else
					SendChatMessage(PRIVMSG, recvData.target.c_str(), "Ma %i. %i. %i. %s napja van.", sVezerlo.Ev(), honap, nap, napdb.c_str());
			}
		}
 	}
}

void IRCSession::Ido(IRCMessage& recvData)
{
	int perc = sVezerlo.Perc();

	if(perc < 10)
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Helyi idõ: %i:0%i", sVezerlo.Ora(), perc);
	else
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Helyi idõ: %i:%i", sVezerlo.Ora(), perc);
}

void IRCSession::Keres(IRCMessage& recvData)
{
	if(recvData.args.length() <= firstSpace+1)
		return;

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
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Segítség a kereséshez!");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Funkció használata: %skeres <ide jön a kereset szöveg>", m_ParancsElojel.c_str());
	}
	else
	{
		string alomany;
		int resAdat = res.size();

		for(int i = 1; i < resAdat; i++)
			alomany += " " + res[i];

		string kiiras = sVezerlo.urlencode(alomany.substr(1));

		char* eredmeny = new char[5000];
		snprintf(eredmeny, 5000, "http://ajax.googleapis.com/ajax/services/search/web?v=1.0&start=0&rsz=small&q=%s", kiiras.c_str());

		m_Curl = curl_easy_init();
		if(m_Curl)
		{
			string bufferdata;

			curl_easy_setopt(m_Curl, CURLOPT_URL, eredmeny);
			curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, IRCSession::writer);
			curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, &bufferdata);
			CURLcode result = curl_easy_perform(m_Curl);

			curl_easy_cleanup(m_Curl);
			delete[] eredmeny;

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

				int szokoz = matched1.find('","');
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "2Title: %s", matched1.substr(0, szokoz).c_str());
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "2Link: 9%s", matched.c_str());
			}
			else
			{
				Log.Notice("IRCSession", "Keres: Hiba a Http lekerdezesben.");
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hibás találat");
			}
		}
	}

	res.clear();
}

void IRCSession::Forditas(IRCMessage& recvData)
{
	if(recvData.args.length() <= firstSpace+1)
		return;

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	if(res[1] == Help)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Segítség a fordításhoz!");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Funkció használata: %sfordit <mirõl|mire> <szöveg>", m_ParancsElojel.c_str());
	}
	else
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		string nyelv = res[1];
		string alomany;
		int resAdat = res.size();

		for(int i = 2; i < resAdat; i++)
			alomany += " " + res[i];

		string iras = sVezerlo.urlencode(alomany.substr(1));

		char* eredmeny = new char[5000];
		snprintf(eredmeny, 5000, "http://ajax.googleapis.com/ajax/services/language/translate?v=1.0&q=%s&langpair=%s", iras.c_str(), nyelv.c_str());

		m_Curl = curl_easy_init();
		if(m_Curl)
		{
			string bufferdata;

			curl_easy_setopt(m_Curl, CURLOPT_URL, eredmeny);
			curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, IRCSession::writer);
			curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, &bufferdata);

			CURLcode result = curl_easy_perform(m_Curl);

			curl_easy_cleanup(m_Curl);
			delete[] eredmeny;

			if(result == CURLE_OK)
			{
				boost::regex re("\\{.translatedText.\\:.(.+).\\},");
				boost::cmatch matches;

				boost::regex_search(bufferdata.c_str(), matches, re);
				string matched(matches[1].first, matches[1].second);

				SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s", matched.c_str());
			}
			else
			{
				Log.Notice("IRCSession", "Fordit: Hiba a Http lekerdezesben.");
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hibás fordítás");
			}
		}
	}

	res.clear();
}

void IRCSession::Sha1(IRCMessage& recvData)
{
	if(recvData.args.length() <= firstSpace+1)
		return;

	Sha1Hash sha1;
	unsigned char* eredmeny = new unsigned char[SHA_DIGEST_LENGTH+1];

	sha1.Initialize();
	sha1.UpdateData(recvData.args.substr(firstSpace+1));
	sha1.Finalize();
	memcpy(eredmeny, sha1.GetDigest(), SHA_DIGEST_LENGTH);

	stringstream ss;
	const size_t len = 20;

	for(size_t u = 0; u < len; ++u)
		ss << std::hex << std::setw(2) << std::setfill('0') << cast_int(eredmeny[u]);

	string hash;
	ss >> hash;

	SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s", hash.c_str());
}

void IRCSession::Md5(IRCMessage& recvData)
{
	if(recvData.args.length() <= firstSpace+1)
		return;

	MD5Hash md5;
	unsigned char* eredmeny = new unsigned char[MD5_DIGEST_LENGTH+1];

	md5.Initialize();
	md5.UpdateData(recvData.args.substr(firstSpace+1));
	md5.Finalize();
	memcpy(eredmeny, md5.GetDigest(), MD5_DIGEST_LENGTH);

	stringstream ss;
	const size_t len = 16;

	for(size_t u = 0; u < len; ++u)
		ss << std::hex << std::setw(2) << std::setfill('0') << cast_int(eredmeny[u]);

	string hash;
	ss >> hash;

	SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s", hash.c_str());
}

void IRCSession::Xrev(IRCMessage& recvData)
{
	if(recvData.args.length() <= firstSpace+1)
		return;

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
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Parancs használata: %sxrev <emulátor neve> <rev>", m_ParancsElojel.c_str());
		res.clear();
		return;
	}
	else if(iras == "ascnhalf")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		int rev;
		stringstream ss;
		ss << res[2];
		ss >> rev;

		SvnAscNHalf(rev, recvData.target);
	}
	else if(iras =="arcemu")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		int rev;
		stringstream ss;
		ss << res[2];
		ss >> rev;

		SvnArcemu(rev, recvData.target);
	}
	else if(iras == "arcscripts")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		int rev;
		stringstream ss;
		ss << res[2];
		ss >> rev;

		SvnArcScripts(rev, recvData.target);
	}
	else if(iras == "sandshroud")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		int rev;
		stringstream ss;
		ss << res[2];
		ss >> rev;

		SvnSandshroud(rev, recvData.target);
	}
	else if(iras == "schumix")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		int rev;
		stringstream ss;
		ss << res[2];
		ss >> rev;

		SvnSchumix(rev, recvData.target);
	}
	else
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Emulátorok: ascnhalf | arcemu | arcscripts | sandshroud | schumix");
		res.clear();
		return;
	}

	res.clear();
}

void IRCSession::Irc(IRCMessage& recvData)
{
	if(recvData.args.length() <= firstSpace+1)
		return;

	vector<string> res(1);
	sVezerlo.split(recvData.args.substr(firstSpace+1), " ", res);

	if(res.size() < 2)
	{
		res.clear();
		return;
	}

	string irc = res[1];

	if(irc == Help)
	{
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Segítség az irc-hez!");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Parancsok: %sirc rang | %sirc rang1 | %sirc nick | %sirc kick | %sirc owner", m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str(), m_ParancsElojel.c_str());
	}
	else
	{
		QueryResultPointer db = m_SQLConn->Query("SELECT hasznalata FROM irc_parancsok WHERE parancs = '%s'", irc.c_str());
		if(db)
		{
			string hasznalata = db->Fetch()[0].GetString();
			SendChatMessage(PRIVMSG, recvData.target.c_str(), "%s", hasznalata.c_str());
		}
	}

	res.clear();
}

void IRCSession::Szam(IRCMessage& recvData)
{
	if(recvData.args.length() <= firstSpace+1)
		return;

	setConsts();
	makepolishform(recvData.args.substr(firstSpace+1));
	calculate(recvData.target);
}

void IRCSession::Uzenet(IRCMessage& recvData)
{
	if(recvData.args.length() <= firstSpace+1)
		return;

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
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Segitség az üzenethez!");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Funkció használata: %süzenet <ide jön a személy> <ha nem felhivás küldenél hanem saját üzenetet>", m_ParancsElojel.c_str());
	}
	else
	{
		string alomany;
		int resAdat = res.size();

		for(int i = 2; i < resAdat; i++)
			alomany += " " + res[i];

		if(resAdat == 2)
			SendChatMessage(PRIVMSG, iras.c_str(), "Keresnek téged itt: %s", recvData.target.c_str());
		else if(resAdat >= 3)
			SendChatMessage(PRIVMSG, iras.c_str(), "%s", alomany.substr(1).c_str());
	}

	res.clear();
}

void IRCSession::Jegyzet(IRCMessage& recvData)
{
	if(recvData.args.length() <= firstSpace+1)
		return;

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
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Jegyzet beküldése: %sjegyzet <amit feljegyeznél>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Jegyzet kiolvasása: %sjegyzet kod <kod amit kaptál>", m_ParancsElojel.c_str());
	}
	else if(info == "kod")
	{
		if(res.size() < 3)
		{
			res.clear();
			return;
		}

		QueryResultPointer db = m_SQLConn->Query("SELECT jegyzet FROM jegyzetek WHERE kod = '%s'", res[2].c_str());
		if(db)
		{
			string jegyzet = db->Fetch()[0].GetString();
			SendChatMessage(PRIVMSG, recvData.source_nick.c_str(), "Jegyzet: %s", jegyzet.c_str());
		}
	}
	else
	{
		string alomany;
		int resAdat = res.size();

		for(int i = 1; i < resAdat; i++)
			alomany += " " + res[i];

		string kod = randomString(10, true, true, false);
		m_SQLConn->Query("INSERT INTO `jegyzetek`(kod, nick, jegyzet) VALUES ('%s', '%s', '%s')", kod.c_str(), recvData.source_nick.c_str(), alomany.substr(1).c_str());
		SendChatMessage(PRIVMSG, recvData.source_nick.c_str(), "Jegyzet kodja: %s", kod.c_str());
	}

	res.clear();
}
