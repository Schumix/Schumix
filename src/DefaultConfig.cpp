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

DefaultConfig::DefaultConfig(string conf)
{
	Log.Notice("DefaultConfig", "Nincs config fajl!");
	Log.Notice("DefaultConfig", "Három lehetőség közül lehet választani:");
	Log.Notice("DefaultConfig", "1. Letöltöd a konfigot. Parancs: net");
	Log.Notice("DefaultConfig", "2. Konzolba bekonfugurálod. Parancs: konzol");
	Log.Notice("DefaultConfig", "3. Konzolba bekonfugurálod. Parancs: kikapcs");
	char valasz[20];
	fgets(valasz, 20, stdin);
	string _valasz = valasz;
	int szokoz = _valasz.find("\n");

	if(_valasz.substr(0, szokoz) == "net")
	{
		CURL* Curl = curl_easy_init();
		if(Curl)
		{
			string bufferdata;

			curl_easy_setopt(Curl, CURLOPT_URL, "https://github.com/megax/Schumix/raw/master/schumix.conf");
			curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, DefaultConfig::writer);
			curl_easy_setopt(Curl, CURLOPT_WRITEDATA, &bufferdata);
			CURLcode result = curl_easy_perform(Curl);

			curl_easy_cleanup(Curl);

			if(result == CURLE_OK)
			{
				ConfigFajl = fopen(conf.c_str(), "a+");
				if(!ConfigFajl || ConfigFajl == NULL)
				{
					Log.Error("DefaultConfig", "Sikertelen olvasas.\n");
					return;
				}

				fprintf(ConfigFajl, bufferdata.c_str());
				fclose(ConfigFajl);
			}
			else
				Log.Notice("DefaultConfig", "Hiba a Http lekerdezesben.");
		}
	}
	else if(_valasz.substr(0, szokoz) == "konzol")
	{
		ConfigFajl = fopen(conf.c_str(), "a+");
		if(!ConfigFajl || ConfigFajl == NULL)
		{
			Log.Error("DefaultConfig", "Sikertelen olvasas.\n");
			return;
		}

		Log.Notice("DefaultConfig", "Irc reszeinek kitoltese.");
		IrcConfig();
		Log.Notice("DefaultConfig", "Mysql reszeinek kitoltese.");
		MysqlConfig();

		printf("Parancs elojel: ");
		char parancs[20];
		fgets(parancs, 20, stdin);
		string _parancs = parancs;
		szokoz = _parancs.find("\n");

		fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
		fprintf(ConfigFajl, "# Parancs\n");
		fprintf(ConfigFajl, "#    Elojel\n");
		fprintf(ConfigFajl, "#         Meghatározza a parancsok előjelét.\n");
		fprintf(ConfigFajl, "#         Alapértelmezés: &\n");
		fprintf(ConfigFajl, "#    Uzemelteto\n");
		fprintf(ConfigFajl, "#         Meghatározza az üzemeltető nevét is azt fogja kiirni az info parancsnál.\n");
		fprintf(ConfigFajl, "#         Alapértelmezés: Schumix\n");
		fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
		fprintf(ConfigFajl, "<Parancs Elojel = \"%s\">\n\n", _parancs.substr(0, szokoz).c_str());
		fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
		fprintf(ConfigFajl, "# LekerdezesiIdo\n");
		fprintf(ConfigFajl, "#    Gitinfo\n");
		fprintf(ConfigFajl, "#         Meghatározza hogy hány másodpercenként legyen lekérdezés.\n");
		fprintf(ConfigFajl, "#         Alapértelmezés: 15\n");
		fprintf(ConfigFajl, "#    Hginfo\n");
		fprintf(ConfigFajl, "#         Meghatározza hogy hány másodpercenként legyen lekérdezés.\n");
		fprintf(ConfigFajl, "#         Alapértelmezés: 15\n");
		fprintf(ConfigFajl, "#    Svninfo\n");
		fprintf(ConfigFajl, "#         Meghatározza hogy hány másodpercenként legyen lekérdezés.\n");
		fprintf(ConfigFajl, "#         Alapértelmezés: 15\n");
		fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
		fprintf(ConfigFajl, "<LekerdezesiIdo Gitinfo = \"15\"\n");
		fprintf(ConfigFajl, "                Hginfo = \"15\"\n");
		fprintf(ConfigFajl, "                Svninfo = \"15\">\n\n");
		fclose(ConfigFajl);
	}
	else if(_valasz.substr(0, szokoz) == "kikapcs")
		exit(1);
}

DefaultConfig::~DefaultConfig()
{

}

void DefaultConfig::IrcConfig()
{
	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	fprintf(ConfigFajl, "#                       Schumix konfig fájla                          #\n");
	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	fprintf(ConfigFajl, "\n");
	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	fprintf(ConfigFajl, "# IRC\n");
	fprintf(ConfigFajl, "#     Server\n");
	fprintf(ConfigFajl, "#         Az irc szerver címe.\n");
	fprintf(ConfigFajl, "#     Port\n");
	fprintf(ConfigFajl, "#         Az irc szerver kapcsolodási portja.\n");
	fprintf(ConfigFajl, "#         Alapértelmezés: 6667\n");
	fprintf(ConfigFajl, "#     LinesPerSecond\n");
	fprintf(ConfigFajl, "#         Flood elleni védelem. Másodpercben megadva lasitja az üzenet küldését.\n");
	fprintf(ConfigFajl, "#         Alapértelmezés: 3\n");
	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");

	printf("Ird be a szervert: ");
	char server[300];
	fgets(server, 300, stdin);
	string _server = server;
	int szokoz = _server.find("\n");
	string host = _server.substr(0, szokoz);
	fprintf(ConfigFajl, "<IRC Server = \"%s\"\n", host.c_str());

	printf("Ird be a portot: ");
	char port[30];
	fgets(port, 30, stdin);
	string _port = port;
	szokoz = _port.find("\n");
	fprintf(ConfigFajl, "     Port = \"%s\"\n", _port.substr(0, szokoz).c_str());
	fprintf(ConfigFajl, "     LinesPerSecond = \"3\">\n\n");

	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	fprintf(ConfigFajl, "# Log\n");
	fprintf(ConfigFajl, "#     Loglevel\n");
	fprintf(ConfigFajl, "#         Van három logolási szint.\n");
	fprintf(ConfigFajl, "#         Első lehetőség, csak a semilyen szintüek futnak. Száma: 0\n");
	fprintf(ConfigFajl, "#         Második lehetőség, csak az error fut le pluszba. Száma: 1\n");
	fprintf(ConfigFajl, "#         Harmadik lehetőség, az error mellet lefut a warring is. Száma: 2\n");
	fprintf(ConfigFajl, "#         Negyedik lehetőség, lefut a debug is. Száma: 3\n");
	fprintf(ConfigFajl, "#         Alapértelmezés: 3\n");
	fprintf(ConfigFajl, "#     Loghelye\n");
	fprintf(ConfigFajl, "#         Meghatározza hogy a logolást hova mentse a bot.\n");
	fprintf(ConfigFajl, "#         Alapértelmezés: szoba\n");
	fprintf(ConfigFajl, "#     Irclog\n");
	fprintf(ConfigFajl, "#         Ha az értéke 1-re van állitva akkor a konzolba kiirja a notice, privmsg stb adatokat.\n");
	fprintf(ConfigFajl, "#         Alapértelmezés: 0\n");
	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	fprintf(ConfigFajl, "<Log Loglevel = \"3\"\n");
	fprintf(ConfigFajl, "     Loghelye = \"szoba\"\n");
	fprintf(ConfigFajl, "     Irclog = \"0\">\n\n");

	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	fprintf(ConfigFajl, "# User\n");
	fprintf(ConfigFajl, "#     Nick\n");
	fprintf(ConfigFajl, "#         Elsődleges nick.\n");
	fprintf(ConfigFajl, "#     Nick2\n");
	fprintf(ConfigFajl, "#         Másodlagos nick.\n");
	fprintf(ConfigFajl, "#     Nick3\n");
	fprintf(ConfigFajl, "#         Harmadlagos nick.\n");
	fprintf(ConfigFajl, "#     Username\n");
	fprintf(ConfigFajl, "#         Felhasználó név.\n");
	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");

	printf("Ird be a nicket: ");
	char nick[50];
	fgets(nick, 50, stdin);
	string _nick = nick;
	szokoz = _nick.find("\n");

	fprintf(ConfigFajl, "<User Nick = \"%s\"\n", _nick.substr(0, szokoz).c_str());
	fprintf(ConfigFajl, "      Nick2 = \"_%s\"\n", _nick.substr(0, szokoz).c_str());
	fprintf(ConfigFajl, "      Nick3 = \"__%s\"\n", _nick.substr(0, szokoz).c_str());
	fprintf(ConfigFajl, "      Username = \"%s\"\n", _nick.substr(0, szokoz).c_str());
	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	fprintf(ConfigFajl, "# NickServ\n");
	fprintf(ConfigFajl, "#    Enable\n");
	fprintf(ConfigFajl, "#         Engedélyezi vagy tiltja a NickServ-hez a nick azonositoját.\n");
	fprintf(ConfigFajl, "#         Alapértelmezés: 0\n");
	fprintf(ConfigFajl, "#    Password\n");
	fprintf(ConfigFajl, "#         Jelszó a NickServ-hez a nick azonósitáshoz.\n");
	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	fprintf(ConfigFajl, "<NickServ Enable = \"0\"\n");
	fprintf(ConfigFajl, "          Password = \"jelszo\">\n\n");

	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	fprintf(ConfigFajl, "# HostServ\n");
	fprintf(ConfigFajl, "#    Enable\n");
	fprintf(ConfigFajl, "#         Vhost ki be kapcsolása. 0 kikapcsolva, 1 bekapcsolva.\n");
	fprintf(ConfigFajl, "#         Alapértelmezés: 0\n");
	fprintf(ConfigFajl, "#    Vhost\n");
	fprintf(ConfigFajl, "#         Ha nincs vhost-od ted 0-ra ha van 1-re.\n");
	fprintf(ConfigFajl, "#         Alapértelmezés: 0\n");
	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	fprintf(ConfigFajl, "<HostServ Enable = \"0\"\n");
 	fprintf(ConfigFajl, "          Vhost = \"0\">\n\n");
}

void DefaultConfig::MysqlConfig()
{
	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");
	fprintf(ConfigFajl, "# MySQL\n");
	fprintf(ConfigFajl, "#    Host\n");
	fprintf(ConfigFajl, "#         Mysql szerver host neve.\n");
	fprintf(ConfigFajl, "#    Port\n");
	fprintf(ConfigFajl, "#         Mysql-hez használt port.\n");
	fprintf(ConfigFajl, "#    User\n");
	fprintf(ConfigFajl, "#         Felhasználónév a mysql szerverhez.\n");
	fprintf(ConfigFajl, "#    Password\n");
	fprintf(ConfigFajl, "#         Jelszó a mysql szerverhez.\n");
	fprintf(ConfigFajl, "#    Database\n");
	fprintf(ConfigFajl, "#         Az adatbázis neve.\n");
	fprintf(ConfigFajl, "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#\n");

	printf("Ird be a hostot: ");
	char server[300];
	fgets(server, 300, stdin);
	string _server = server;
	int szokoz = _server.find("\n");
	fprintf(ConfigFajl, "<MySQL Host = \"%s\"\n", _server.substr(0, szokoz).c_str());

	printf("Ird be a portot: ");
	char port[30];
	fgets(port, 30, stdin);
	string _port = port;
	szokoz = _port.find("\n");
	fprintf(ConfigFajl, "       Port = \"%s\"\n", _port.substr(0, szokoz).c_str());

	printf("Ird be a felhasznalo: ");
	char user[50];
	fgets(user, 50, stdin);
	string _user = user;
	szokoz = _user.find("\n");

	fprintf(ConfigFajl, "       User = \"%s\"\n", _user.substr(0, szokoz).c_str());

	printf("Ird be a jelszo: ");
	char pass[50];
	fgets(pass, 50, stdin);
	string _pass = pass;
	szokoz = _pass.find("\n");

	fprintf(ConfigFajl, "       Password = \"%s\"\n", _pass.substr(0, szokoz).c_str());

	printf("Ird be a adatbazis: ");
	char db[50];
	fgets(db, 50, stdin);
	string _db = db;
	szokoz = _db.find("\n");

	fprintf(ConfigFajl, "       Database = \"%s\">\n\n", _db.substr(0, szokoz).c_str());
}

int DefaultConfig::writer(char* data, size_t size, size_t nmemb, string *buffer)
{
	int result = NULL;

	if(buffer != NULL)
	{
		buffer->append(data, size* nmemb);
		result = size* nmemb;
	}
	return result;
}
