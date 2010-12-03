/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2008 Valroft <http://www.aspiredev.org/>
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

#ifndef _SCHUMIX_REPLIES_HPP
#define _SCHUMIX_REPLIES_HPP

struct IRCMessage
{
	string Hostmask;	// Egybenlévő hostmask melyből majd darabolodik a nick, user, host
	string Opcode;		// Opcode elnevezése vagy száma
	string Channel;		// Channel név ahonét jön az adat
	string Args;		// Szöveg ami bejön

	// Bejövő adat így darabolodik: :schumix!Schumix@schumix_host
	string Nick; // schumix
	string User; // Schumix
	string Host; // schumix_host

	// Minden
	string Minden;

	// .c_str() hozzáadva hogy const char* legyen
	inline const char* GetHostmask() { return Hostmask.c_str(); }
	inline const char* GetOpcode() { return Opcode.c_str(); }
	inline const char* GetChannel() { return Channel.c_str(); }
	inline const char* GetArgs() { return Args.c_str(); }
	inline const char* GetNick() { return Nick.c_str(); }
	inline const char* GetUser() { return User.c_str(); }
	inline const char* GetHost() { return Host.c_str(); }
	inline const char* GetMinden() { return Minden.c_str(); }
};

static string RPL_SUCCESSFUL_AUTH		= "001";
static string RPL_MOTD				= "372";
static string RPL_MOTDSTART			= "375";
static string RPL_ENDOFMOTD			= "376";
static string RPL_NOTICE				= "NOTICE";
static string RPL_PRIVMSG				= "PRIVMSG";
static string RPL_NICK				= "NICK";
static string RPL_JOIN				= "JOIN";
static string RPL_LEFT				= "PART";
static string RPL_QUIT				= "QUIT";

static string RPL_PING				= "PING";
static string RPL_PONG				= "PONG";

static string RPL_KILL				= "KILL";
static string RPL_KICK				= "KICK";
static string RPL_MODE				= "MODE";
static string RPL_404					= "404";
static string RPL_NICK_ERROR			= "433";
static string RPL_Channel_ban			= "474";
static string RPL_NoChannel_jelszo	= "475";
static string RPL_WHOIS					= "319";

/*#define RPL_ISMERETLEN			"002"
#define RPL_ISMERETLEN1			"003"
#define RPL_ISMERETLEN2			"004"
#define RPL_ISMERETLEN3			"005"
#define RPL_ISMERETLEN4			"042"
#define RPL_ISMERETLEN5			"251"
#define RPL_ISMERETLEN6			"252"
#define RPL_ISMERETLEN7			"253"
#define RPL_ISMERETLEN8			"254"
#define RPL_ISMERETLEN9			"255"
#define RPL_ISMERETLEN10		"265"
#define RPL_ISMERETLEN11		"266"
#define RPL_ISMERETLEN12		"332"
#define RPL_ISMERETLEN13		"333"
#define RPL_ISMERETLEN14		"353"
#define RPL_ISMERETLEN15		"366"
#define RPL_ISMERETLEN16		"412"
#define RPL_ISMERETLEN17		"439"
#define RPL_ISMERETLEN18		"462"
#define RPL_ISMERETLEN19		"451"

#define RPL_WHOIS				"310"
#define RPL_WHOIS1				"311"
#define RPL_WHOIS2				"312"
#define RPL_WHOIS3				"317"
#define RPL_WHOIS4				"318"
#define RPL_WHOIS5				"338"
#define RPL_WHOIS6				"301"
#define RPL_WHOIS7				"307"
#define RPL_WHOIS8				"671"
#define RPL_WHOIS9				"672"*/

#endif
