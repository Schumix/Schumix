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
	{ "�J�V","�bel","Genov�va","Titusz","Simon","Boldizs�r","Attila","Gy�ngyv�r","Marcell","Mel�nia","�gota","Ern�","Veronika","B�dog","L�r�nt","Guszt�v","Antal","Piroska","S�ra","Sebesty�n","�gnes","Vince","Zelma","Tim�t","P�l","Vanda","Angelika","K�roly,","Ad�l","Martina","Marcella" },
	{ "Ign�c","Karolina","Bal�zs","R�hel","�gota","D�ra","T�dor","Aranka","Abig�l","Elvira","Bertold","L�via","Ella, Linda","B�lint","Kolos","Julianna","Don�t","Bernadett","Zsuzsanna","�lmos","Eleon�ra","Gerzson","Alfr�d","M�ty�s","G�za","Edina","�kos, B�tor","Elem�r","","","" },
	{ "Albin","Lujza","Korn�lia","K�zm�r","Adorj�n","Leon�ra","Tam�s","Zolt�n","Franciska","Ildik�","Szil�rd","Gergely","Kriszti�n, Ajtony","Matild","Krist�f","Henrietta","Gertr�d","S�ndor","J�zsef","Klaudia","Benedek","Be�ta","Em�ke","G�bor","Ir�n","Em�nuel","Hajnalka","Gedeon","Auguszta","Zal�n","�rp�d" },
	{ "Hug�","�ron","Buda, Rich�rd","Izidor","Vince","Vilmos, B�borka","Herman","D�nes","Erhard","Zsolt","Zsolt, Le�","Gyula","Ida","Tibor","Tas, Anaszt�zia","Csongor","Rudolf","Andrea","Emma","Konr�d, Tivadar","Konr�d","Csilla","B�la","Gy�rgy","M�rk","Ervin","Zita","Val�ria","P�ter","Katalin, Kitti","" },
	{ "F�l�p","Zsigmond","T�mea","M�nika","Gy�rgyi","Ivett","Gizella","Mih�ly","Gergely","�rmin","Ferenc","Pongr�c","Szerv�c","Bonif�c","Zs�fia","Botond, M�zes","Paszk�l","Erik","Iv�, Mil�n","Bern�t, Fel�cia","Konstantin","J�lia, Rita","Dezs�","Eszter","Orb�n","F�l�p","Hella","Emil, Csan�d","Magdolna","Zsanett, Janka","Ang�la" },
	{ "T�nde","Anita, K�rmen","Klotild","Bulcs�","Fatime","Norbert","R�bert","Med�rd","F�lix","Margit","Barnab�s","Vill�","Antal, Anett","Vazul","Jol�n","Jusztin","Laura","Levente","Gy�rf�s","Rafael","Alajos","Paulina","Zolt�n","Iv�n","Vilmos","J�nos","L�szl�","Levente, Ir�n","P�ter, P�l","P�l","" },
	{ "Annam�ria","Ott�","Korn�l","Ulrik","Sarolta, Emese","Csaba","Appol�nia","Ell�k","Lukr�cia","Am�lia","N�ra, Lili","Izabella","Jen�","&�rs","Henrik","Valter","Endre, Elek","Frigyes","Em�lia","Ill�s","D�niel","Magdolna","Lenke","Kinga, Kincs�","Krist�f, Jakab","Anna, Anik�","Olga","Szabolcs","M�rta","Judit","Oszk�r" },
	{ "Bogl�rka","Lehel","Hermina","Domonkos","Krisztina","Berta","Ibolya","L�szl�","Em�d","L�rinc","Zsuzsanna","Kl�ra","Ipoly","Marcell","M�ria","�brah�m","J�cint","Ilona","Huba","Istv�n","S�muel","Menyh�rt","Bence","Bertalan","Lajos","Izs�","G�sp�r","�goston","Beatrix","R�zsa","Erika" },
	{ "Egon","Rebeka","Hilda","Roz�lia","Viktor, L�rinc","Zakari�s","Regina","M�ria","�d�m","Nikolett, Hunor","Teod�ra","M�ria","Korn�l","Szer�na","Enik�","Edit","Zs�fia","Di�na","Vilhelmina","Friderika","M�t�","M�ric","Tekla","Gell�rt","Eufrozina","Jusztina","Adalbert","Vencel","Mih�ly","Jeromos","" },
	{ "Malvin","Petra","Helga","Ferenc","Aur�l","Ren�ta","Am�lia","Kopp�ny","D�nes","Gedeon","Brigitta","Miksa","K�lm�n","Hel�n","Ter�z","G�l","Hedvig","Luk�cs","N�ndor","Vendel","Orsolya","El�d","Gy�ngyi","Salamon","Bianka","D�m�t�r","Szabina","Simon","N�rcisz","Alfonz","Farkas" },
	{ "Marianna","Achilles","Gy�z�","K�roly","Imre","L�n�rd","Rezs�","Zsombor","Tivadar","R�ka","M�rton","J�n�s, Ren�t�","Szilvia","Aliz","Albert, Lip�t","�d�n","Hortenzia, Gerg�","Jen�","Erzs�bet","Jol�n","Oliv�r","Cec�lia","Kelemen","Emma","Katalin","Vir�g","Virgil","Stef�nia","Taksony","Andr�s, Andor","" },
	{ "Elza","Melinda","Ferenc","Barbara, Borb�la","Vilma","Mikl�s","Ambrus","M�ria","Nat�lia","Judit","�rp�d","Gabriella","Luca","Szil�rda","Val�r","Etelka","L�z�r","Auguszta","Viola","Teofil","Tam�s","Z�no","Vikt�ria","�d�m, �va","KAR�CSONY","KAR�CSONY","J�nos","Kamilla","Tam�s","D�vid","Szilveszter" },
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
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Helyi id�: %i:0%i", sVezerlo.Ora(), perc);
	else
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Helyi id�: %i:%i", sVezerlo.Ora(), perc);
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
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Seg�ts�g a keres�shez!");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Funkci� haszn�lata: %skeres <ide j�n a kereset sz�veg>", m_ParancsElojel.c_str());
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
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hib�s tal�lat");
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
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Seg�ts�g a ford�t�shoz!");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Funkci� haszn�lata: %sfordit <mir�l|mire> <sz�veg>", m_ParancsElojel.c_str());
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
				SendChatMessage(PRIVMSG, recvData.target.c_str(), "Hib�s ford�t�s");
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
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Parancs haszn�lata: %sxrev <emul�tor neve> <rev>", m_ParancsElojel.c_str());
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
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Emul�torok: ascnhalf | arcemu | arcscripts | sandshroud | schumix");
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
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Seg�ts�g az irc-hez!");
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
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Segits�g az �zenethez!");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Funkci� haszn�lata: %s�zenet <ide j�n a szem�ly> <ha nem felhiv�s k�lden�l hanem saj�t �zenetet>", m_ParancsElojel.c_str());
	}
	else
	{
		string alomany;
		int resAdat = res.size();

		for(int i = 2; i < resAdat; i++)
			alomany += " " + res[i];

		if(resAdat == 2)
			SendChatMessage(PRIVMSG, iras.c_str(), "Keresnek t�ged itt: %s", recvData.target.c_str());
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
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Alparancsok haszn�lata:");
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Jegyzet bek�ld�se: %sjegyzet <amit feljegyezn�l>", m_ParancsElojel.c_str());
		SendChatMessage(PRIVMSG, recvData.target.c_str(), "Jegyzet kiolvas�sa: %sjegyzet kod <kod amit kapt�l>", m_ParancsElojel.c_str());
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
