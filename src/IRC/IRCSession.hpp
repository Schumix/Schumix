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
	Operator      = 0,
	Administrator = 1
};

enum MessageType
{
	PRIVMSG,
	NOTICE
};

typedef void(IRCSession::*IRCCallback)(IRCMessage& recvData);

class IRCSession : public Singleton<IRCSession>
{
public:
	IRCSession(string host, uint32 port);
	~IRCSession();

	/*
	 * Kifelé induló fv-k melyek a szerver felé küldik az adatott. Bele van épitve a "\n".
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
	inline const char* GetParancsElojel() { return m_ParancsElojel.c_str(); }
	inline map<string, string> GetChannelLista() { return m_ChannelLista; }
	inline const char* GetUzemelteto() { return m_Uzemelteto.c_str(); }

	// Véletlen szerû jelszó generálható vele
	string RandomString(int length, bool letters, bool numbers, bool symbols);
	void SocketDisconnect();

	// Lehetséges kapcsolat fajták: CONN_CONNECTED, CONN_REGISTERING vagy CONN_REGISTERED
	uint8 m_ConnState;
	// Class leállása
	void Leallas();

	// Számologép
	void setConsts();
	void makepolishform(string szam);
	void calculate(string privmsg);

	// Channel funkciók beazonostásához való fv-k
	string FSelect(string nev);
	string FSelectChannel(string nev, string channel);

	void ChannelFunkcioReload();
	void ChannelListaReload();
	string ChannelFunkciok(string nev, string status, string channel);
	string ChannelFunkciokInfo(string channel);

	// Channel meghatárózás hova kell irni az adatott
	string m_ChannelPrivmsg;
	// Whois channel meghatárózás hova kell irni az adatott
	string m_WhoisPrivmsg;

	// Éppen használatban lévõ nick
	string m_NickTarolo;

	void SvnSandshroud(int rev, string channel);

protected:
	static Thread_void RunUpdateProc(void* smg);

	/*
	 * Rehashes the schumix.conf configuration file.
	 */
	void RehashConfig();

	/*
	 * Bejövö információ az irc szerver felõl. Szétosztja az opcodes-nek az adatokat stb.
	 */
	void BejovoInfo(string SInfo);

	/*
	 * Updates the IRC Session. Do not invoke directly.
	 */

	void Update();

	void InitHandler();
	void RegisterHandler(string code, IRCCallback method);

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

	void Logfajl(IRCMessage& recvData);

	// Schumix
	void Schumix(IRCMessage& recvData);
	void HLUzenet(IRCMessage& recvData);
	bool AutoKick(IRCMessage& recvData, string allapot);
	void CNick(IRCMessage& recvData);

	// Újra kapcsolodási szál
	void ReConnect();

	bool Running() { return m_running; }
	volatile bool m_running;

	// Szerver pingelési ideje
	uint32 m_LastPing;
	// Configból származó host
	string m_Host;
	// Configból származó port
	uint32 m_Port;

	// Felhasználó név
	string m_UserName;
	// Configból olvasható három nick név
	string m_NickName[3];

	// Nickserv Module
	bool m_UseNickServ;
	string m_NickServPassword;

	// Hostserv állapota
	bool m_UseHostServ;
	bool m_vhost;
	bool h_Aktivitas;

	// The name the server refers to itself as.
	string m_ServerRealName;

	// Channel lista és ahoz tartózó jelszavak
	map<string, string> m_ChannelLista;
	// Channelekhez tartózó funkciok
	vector<string> m_ChannelFunkcio;

	// The last received MOTD. Be sure to check whether mHasFullMotd is true before using this.
	//string mLastMotd;
	// Whether or not the mLastMotd variable is populated fully.
	//bool mHasFullMotd;

	// Parancs elõjelét tárólja
	string m_ParancsElojel;
	// Az üzemeltetõ nevét tárólja
	string m_Uzemelteto;

	// Meghatározza hogy mode fusson-e le vagy sem
	bool AutoMode;
	// Channel-t tároló a mode futatásához
	string ModeChannel;

	// Log helye
	string m_LogHelye;

private:
	// Url kezeles
	CURL* m_Curl;
	Mutex m_mutex;
	// Socket kapcsolat.
	SocketPointer m_Socket;

	CommandsPointer m_Commands;

	map<string, IRCCallback> MessageHandlerMap;

	static int writer(char* data, size_t size, size_t nmemb, string *buffer);
};

#define sIRCSession IRCSession::getSingleton()

#endif
