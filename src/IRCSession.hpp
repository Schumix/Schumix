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

#ifndef _SCHUMIX_IRC_SESSION_HPP
#define _SCHUMIX_IRC_SESSION_HPP

// funkciok
#define bekapcsol "be"
#define kikapcsol "ki"
#define added "add"
#define delet "del"
#define update "update"
#define INFO "info"
#define Help "help"

#define RECONNECT "reconnect"
#define KOSZONES "koszones"
#define LOG "log"
#define SVN "svn"
#define GIT "git"
#define HG "hg"
#define REJOIN "rejoin"
#define HL "hl"
#define PARANCSOK "parancsok"
#define KICK "kick"
#define MODE "mode"

class MySQLConnection;
class Socket;
class SvnInfo;
class GitInfo;
class HgInfo;
class Console;

enum Conn
{
	CONN_CONNECTED = 0,
	CONN_REGISTERING,
	CONN_REGISTERED
};

enum AdminFlag
{
	Operator		= 0,
	Administrator	= 1
};

enum MessageType
{
	PRIVMSG,
	NOTICE,
};

typedef void(IRCSession::*IRCCallback)(IRCMessage& recvData);
typedef map<string, IRCCallback> MessageHandlerMap;
extern MessageHandlerMap IRCMessageHandlerMap;

class IRCSession : public Singleton<IRCSession>
{
public:
	IRCSession(string host, uint32 port, string sqlhost, string user, string pass, string database);
	~IRCSession();

	/*
	 * Kifel� indul� fv-k melyek a szerver fel� k�ldik az adatott. Bele van �pitve a "\n".
	 */
	void WriteLine(const char * format, ...);
	void WriteLineForce(const char * format, ...);

	/*
	 * Send a simple chat message. 
	 * @param MessageType Either "PRIVMSG" or "NOTICE" now.
	 * @param target The target of the message. Can be a channel or a user.
	 * @param format The message to send. May include formatters such as %s, %u, etc.
	 */
	void SendChatMessage(MessageType type, const char * target, const char * format, ...);

	inline string GetHost() { return m_Host; }
	inline uint32 GetPort() { return m_Port; }
	inline SocketPointer GetSocket() { return m_Socket; }
	inline string GetNickTarolo() { return m_NickTarolo; }
	inline string GetUserName() { return m_UserName; }

	// V�letlen szer� jelsz� gener�lhat� vele
	string RandomString(int length, bool letters, bool numbers, bool symbols);
	void SocketDisconnect();

	// Lehets�ges kapcsolat fajt�k: CONN_CONNECTED, CONN_REGISTERING vagy CONN_REGISTERED
	uint8 m_ConnState;
	// Class le�ll�sa
	void Leallas();

protected:
	static Thread_void RunUpdateProc(void* smg);
	void Handler();
	void RegisterHandler(string code, IRCCallback method);

	/*
	 * Rehashes the schumix.conf configuration file.
	 */
	void RehashConfig(string host, string user, string pass, string database);

	/*
	 * Bej�v� inform�ci� az irc szerver fel�l. Sz�tosztja az opcodes-nek az adatokat stb.
	 */
	void BejovoInfo(string SInfo);

	/*
	 * Updates the IRC Session. Do not invoke directly.
	 */

	void Update();

	/* Message Handlers
	 * --------------------------------
	 * Invoked automatically by BejovoInfo. Do not invoke directly.
	 */
	void HandleSuccessfulAuth(IRCMessage& recvData);
	void HandleMotdStart(IRCMessage& recvData);
	void HandleMotd(IRCMessage& recvData);
	void HandleMotdStop(IRCMessage& recvData);
	void HandlePrivmsg(IRCMessage& recvData);
	void HandleNotice(IRCMessage& recvData);
	void HandlePing(IRCMessage& recvData);
	void HandlePong(IRCMessage& recvData);
	void HandleKick(IRCMessage& recvData);
	void HandleMode(IRCMessage& recvData);
	void HandleJoin(IRCMessage& recvData);
	void HandleLeft(IRCMessage& recvData);
	void HandleQuit(IRCMessage& recvData);
	void HandleReJoin(IRCMessage& recvData);
	void HandleNick(IRCMessage& recvData);
	void HandleNickError(IRCMessage& recvData);
	void HandleNoChannelJelszo(IRCMessage& recvData);
	void HandleChannelBan(IRCMessage& recvData);
	void HandleWhois(IRCMessage& recvData);

	// Funkciok
	void Logfajl(IRCMessage& recvData);
	void Admin(IRCMessage& recvData);
	void Hozzaferes(IRCMessage& recvData);
	void Ujjelszo(IRCMessage& recvData);
	void Datum(IRCMessage& recvData);
	void Ido(IRCMessage& recvData);
	void Funkciok(IRCMessage& recvData);
	void Keres(IRCMessage& recvData);
	void Forditas(IRCMessage& recvData);
	void Teszt(IRCMessage& recvData);
	void Sha1(IRCMessage& recvData);
	void Md5(IRCMessage& recvData);
	void Channel(IRCMessage& recvData);
	void Sznap(IRCMessage& recvData);
	void Xrev(IRCMessage& recvData);
	void Szoba(IRCMessage& recvData);
	void Irc(IRCMessage& recvData);
	void Szam(IRCMessage& recvData);
	void Nick(IRCMessage& recvData);
	void Join(IRCMessage& recvData);
	void Left(IRCMessage& recvData);
	void Kick(IRCMessage& recvData);
	void Mode(IRCMessage& recvData);
	void Uzenet(IRCMessage& recvData);
	void HLFunkcio(IRCMessage& recvData);
	void Svn(IRCMessage& recvData);
	void Git(IRCMessage& recvData);
	void Hg(IRCMessage& recvData);
	void Jegyzet(IRCMessage& recvData);
	void Reload(IRCMessage& recvData);
	void Whois(IRCMessage& recvData);
	void AutoFunkcio(IRCMessage& recvData);
	void Kikapcsolas(IRCMessage& recvData);

	// Schumix
	void Schumix(IRCMessage& recvData);
	void HLUzenet(IRCMessage& recvData);
	bool AutoKick(IRCMessage& recvData, string allapot);

	// Sz�molog�p
	void setConsts();
	void makepolishform(string szam);
	void calculate(string privmsg);

	// Channel funkci�k beazonost�s�hoz val� fv-k
	string FSelect(string nev);
	string FSelectChannel(string nev, string channel);

	// Adminok meg�llapit�sa
	bool Admin(string nick);
	bool Admin(string nick, AdminFlag Flag);
	bool Admin(string nick, string Vhost, AdminFlag Flag);

	void SvnSandshroud(int rev, string channel);

	void ChannelFunkcioReload();
	void ChannelListaReload();
	string ChannelFunkciok(string nev, string status, string channel);
	string ChannelFunkciokInfo(string channel);

	// �jra kapcsolod�si sz�l
	void ReConnect();

	bool Running() { return m_running; }
	volatile bool m_running;

	// Szerver pingel�si ideje
	uint32 m_LastPing;
	// Configb�l sz�rmaz� host
	string m_Host;
	// Configb�l sz�rmaz� port
	uint32 m_Port;

	// Mysql hozz�f�r�s
	string _mysql[4];

	// Felhaszn�l� n�v
	string m_UserName;
	// �ppen haszn�latban l�v� nick
	string m_NickTarolo;
	// Configb�l olvashat� h�rom nick n�v
	string m_NickName[3];

	// Channel meghat�r�z�s hova kell irni az adatott
	string m_ChannelPrivmsg;

	// Nickserv Module
	bool m_UseNickServ;
	string m_NickServPassword;

	// Hostserv �llapota
	bool m_UseHostServ;
	bool m_vhost;
	bool h_Aktivitas;

	// The name the server refers to itself as.
	string m_ServerRealName;

	// Channel lista �s ahoz tart�z� jelszavak
	map<string, string> m_ChannelLista;
	// Channelekhez tart�z� funkciok
	vector<string> m_ChannelFunkcio;

	// The last received MOTD. Be sure to check whether mHasFullMotd is true before using this.
	//string mLastMotd;
	// Whether or not the mLastMotd variable is populated fully.
	//bool mHasFullMotd;

	// Szok�z a parancsokn�l
	uint8 firstSpace;
	// Whois channel meghat�r�z�s hova kell irni az adatott
	string m_WhoisPrivmsg;

	// Parancs el�jel�t t�r�lja
	string m_ParancsElojel;
	// Az �zemeltet� nev�t t�r�lja
	string m_Uzemelteto;

	// Meghat�rozza hogy mode fusson-e le vagy sem
	bool AutoMode;
	// Channel-t t�rol� a mode futat�s�hoz
	string ModeChannel;

	// Log helye
	string m_LogHelye;

private:
	// Url kezeles
	CURL* m_Curl;
	// Socket kapcsolat.
	SocketPointer m_Socket;
	// Mysql kapcsolat.
	MySQLConnectionPointer m_SQLConn;
	static int writer(char* data, size_t size, size_t nmemb, string *buffer);
};

#define sIRCSession IRCSession::getSingleton()

#endif
