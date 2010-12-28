/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2008 Valroft <http://www.aspiredev.org/>
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

#include "StdAfx.h"
CLog Log;

#define BUILD_TIME __TIME__
#define BUILD_DATE __DATE__

int main()
{
	Log.Init(3);
	Log.Color(TBLUE);
	printf("Schumix Verzio: %s\n", revision);
	printf("A program leallitasahoz hasznald a <Ctrl+C> parancsot vagy <kikapcs>\n\n");
	Log.Color(TNORMAL);
	printf("Keszulesi ido: %s %s\n", BUILD_DATE, BUILD_TIME);
	printf("Keszitette Megax, Twl, Valroft es Scymex http://megaxx.info\n");
	printf("==============================================================================\n");
	printf("\n");
	Log.Notice("Schumix", "Rendszer indul...\n");

#if PLATFORM == PLATFORM_WINDOWS
	// WSA Setup
	WSADATA info;
	int error;

	if((err = WSAStartup(MAKEWORD(2, 2), &info)) != 0)
	{
		Log.Error("WSA", "Hiba tortent a WSA inicializalasanal. Hibakod: %i", error);
		return -1;
	}
#endif

	Log.Notice("Config", "Config fajl betoltese...");

	string config = string(Elnevezes) + ".conf";

	// Config fájl
	if(!Config.MainConfig.SetSource(format("./%s", config.c_str()).c_str()))
	{
		Log.Error("Config", ">> %s", config.c_str());
		new DefaultConfig(config);

		if(Config.MainConfig.SetSource(format("./%s", config.c_str()).c_str()))
			Log.Success("Config", ">> Config adatbazis sikeresen betoltve.");
		else
		{
			Log.Error("Config", ">> Ujabb sikertelen betoltes. Ellenorizze le kezileg.");
			return 0;
		}
	}
	else
		Log.Success("Config", ">> %s", config.c_str());

	printf("\n");

	Log.Init(cast_uint8(Config.MainConfig.GetIntDefault("Log", "Loglevel", 3)));
	new Vezerlo();

#if PLATFORM == PLATFORM_WINDOWS
	WSACleanup();
#endif

	Log.Notice("Schumix", "Program leallt.");
	return 0;
}
