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

	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Verzi�: 10%s", revision);
	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "3Parancsok: %sinfo | %shelp | %sxrev | %sido | %sdatum | %sirc | %sroll | %scalc | %skeres | %sfordit | %ssha1 | %smd5 | %suzenet | %swhois | %sjegyzet | %sprime", sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel(), sIRCSession.GetParancsElojel());
	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Programmed by: 3Csaba");
}

void CommandMgr::HandleInfo(CommandMessage& recvData)
{
	CNick(recvData);
	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Programoz�m: Csaba, Twl �s Scymex. �zemeltet�: %s.", sIRCSession.GetUzemelteto());
}

void CommandMgr::HandleRoll(CommandMessage& recvData)
{
	CNick(recvData);
	uint32 adat = rand()%101;
	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Sz�zal�kos ar�nya %u!", adat);
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
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Helyi id�: %i:0%i", Ora(), perc);
	else
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Helyi id�: %i:%i", Ora(), perc);
}

void CommandMgr::HandleKeres(CommandMessage& recvData)
{
	CNick(recvData);

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs param�ter!");
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
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hib�s tal�lat");
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
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs param�ter!");
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
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs egy sz� se megadva amit lek�ne ford�tani!");
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
			sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hib�s ford�t�s");
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
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs param�ter!");
		return;
	}

	sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%s", sVezerlo.Sha1(recvData.Args.substr(recvData.firstSpace+1)).c_str());
}

void CommandMgr::HandleMd5(CommandMessage& recvData)
{
	CNick(recvData);

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs param�ter!");
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
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Emul�torok: sandshroud");
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
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Hib�s n�v!");

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
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs param�ter!");
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
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs param�ter!");
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
		sIRCSession.SendChatMessage(PRIVMSG, iras.c_str(), "Keresnek t�ged itt: %s", recvData.GetChannel());
	else if(resAdat >= 3)
		sIRCSession.SendChatMessage(PRIVMSG, iras.c_str(), "%s", alomany.substr(1).c_str());

	res.clear();
}

void CommandMgr::HandleJegyzet(CommandMessage& recvData)
{
	CNick(recvData);

	if(recvData.Args.length() <= recvData.firstSpace+1)
	{
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs param�ter!");
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
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs param�ter!");
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
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "Nincs param�ter!");
		return;
	}

	uint32 prim;
	stringstream ss;
	ss << recvData.Args.substr(recvData.firstSpace+1);
	ss >> prim;

	if(!isPrime(prim))
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%u nem primsz�m.", prim);
	else
		sIRCSession.SendChatMessage(PRIVMSG, recvData.GetChannel(), "%u primsz�m.", prim);
}
