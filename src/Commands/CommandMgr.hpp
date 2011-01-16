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

#ifndef _SCHUMIX_COMMAND_MGR_HPP
#define _SCHUMIX_COMMAND_MGR_HPP

struct CommandMessage
{
	string Channel;
	string Args;

	string Nick;
	string User;
	string Host;

	uint8 firstSpace;

	// .c_str() hozzáadva hogy const char* legyen
	inline const char* GetChannel() { return Channel.c_str(); }
	inline const char* GetArgs() { return Args.c_str(); }
	inline const char* GetNick() { return Nick.c_str(); }
	inline const char* GetUser() { return User.c_str(); }
	inline const char* GetHost() { return Host.c_str(); }
};

class CommandMgr;
typedef void(CommandMgr::*CommandHandler)(CommandMessage& recvData);

class CommandMgr : public Singleton<CommandMgr>
{
public:
	CommandMgr();
	~CommandMgr();

	void BejovoInfo(IRCMessage& recvData);

protected:
	void InitHandler();
	void RegisterHandler(string code, CommandHandler method);

	void HandleXbot(CommandMessage& recvData);
	void HandleHelp(CommandMessage& recvData);
	void HandleSzinek(CommandMessage& recvData);
	void HandleInfo(CommandMessage& recvData);
	void HandlePrime(CommandMessage& recvData);
	void HandleRoll(CommandMessage& recvData);
	void HandleAdmin(CommandMessage& recvData);
	void HandleDatum(CommandMessage& recvData);
	void HandleIdo(CommandMessage& recvData);
	void HandleFunkciok(CommandMessage& recvData);
	void HandleKeres(CommandMessage& recvData);
	void HandleForditas(CommandMessage& recvData);
	void HandleTeszt(CommandMessage& recvData);
	void HandleSha1(CommandMessage& recvData);
	void HandleMd5(CommandMessage& recvData);
	void HandleChannel(CommandMessage& recvData);
	void HandleSznap(CommandMessage& recvData);
	void HandleXrev(CommandMessage& recvData);
	void HandleIrc(CommandMessage& recvData);
	void HandleSzam(CommandMessage& recvData);
	void HandleNick(CommandMessage& recvData);
	void HandleJoin(CommandMessage& recvData);
	void HandleLeft(CommandMessage& recvData);
	void HandleKick(CommandMessage& recvData);
	void HandleMode(CommandMessage& recvData);
	void HandleUzenet(CommandMessage& recvData);
	void HandleSvn(CommandMessage& recvData);
	void HandleGit(CommandMessage& recvData);
	void HandleHg(CommandMessage& recvData);
	void HandleJegyzet(CommandMessage& recvData);
	void HandleReload(CommandMessage& recvData);
	void HandleWhois(CommandMessage& recvData);
	void HandleAutoFunkcio(CommandMessage& recvData);
	void HandleKikapcsolas(CommandMessage& recvData);

	void CNick(CommandMessage& recvData);

private:
	// Url kezeles
	CURL* m_Curl;
	static int writer(char* data, size_t size, size_t nmemb, string *buffer);

	map<string, CommandHandler> CommandHandlerMap;
};

#define sCommandMgr CommandMgr::getSingleton()

#endif
