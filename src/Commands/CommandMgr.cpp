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

initialiseSingleton(CommandMgr);

CommandMgr::CommandMgr()
{
	Log.Notice("CommandMgr", "CommandMgr elindult.");

	InitHandler();
}

CommandMgr::~CommandMgr()
{
#ifdef _DEBUG_MOD
	Log.Notice("CommandMgr", "~CommandMgr()");
#endif
}

void CommandMgr::BejovoInfo(IRCMessage& recvData)
{
	CommandMessage cm;

	cm.Args = recvData.Args;
	cm.Channel = recvData.Channel;
	cm.Nick = recvData.Nick;
	cm.User = recvData.User;
	cm.Host = recvData.Host;

	int firstSpace = recvData.Args.find(' ');
	if(firstSpace == string::npos)
		firstSpace = recvData.Args.length();

	cm.firstSpace = firstSpace;

	string cmd = recvData.Args.substr(1, firstSpace - 1);
	transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

	map<string, CommandHandler>::iterator itr = CommandHandlerMap.find(cmd);
	if(itr == CommandHandlerMap.end())
		return;

	CommandHandler ch = itr->second;
	(this->*ch)(cm);
}

void CommandMgr::InitHandler()
{
	// Felhasználói parancsok
	RegisterHandler("xbot",        cast_default(CommandHandler, &CommandMgr::HandleXbot));
	RegisterHandler("help",        cast_default(CommandHandler, &CommandMgr::HandleHelp));
	RegisterHandler("whois",       cast_default(CommandHandler, &CommandMgr::HandleWhois));
	RegisterHandler("jegyzet",     cast_default(CommandHandler, &CommandMgr::HandleJegyzet));
	RegisterHandler("info",        cast_default(CommandHandler, &CommandMgr::HandleInfo));
	RegisterHandler("roll",        cast_default(CommandHandler, &CommandMgr::HandleRoll));
	RegisterHandler("datum",       cast_default(CommandHandler, &CommandMgr::HandleDatum));
	RegisterHandler("ido",         cast_default(CommandHandler, &CommandMgr::HandleIdo));
	RegisterHandler("keres",       cast_default(CommandHandler, &CommandMgr::HandleKeres));
	RegisterHandler("fordit",      cast_default(CommandHandler, &CommandMgr::HandleForditas));
	RegisterHandler("xrev",        cast_default(CommandHandler, &CommandMgr::HandleXrev));
	RegisterHandler("irc",         cast_default(CommandHandler, &CommandMgr::HandleIrc));
	RegisterHandler("calc",        cast_default(CommandHandler, &CommandMgr::HandleSzam));
	RegisterHandler("uzenet",      cast_default(CommandHandler, &CommandMgr::HandleUzenet));
	RegisterHandler("sha1",        cast_default(CommandHandler, &CommandMgr::HandleSha1));
	RegisterHandler("md5",         cast_default(CommandHandler, &CommandMgr::HandleMd5));

	// Operátor parancsok
	RegisterHandler("admin",       cast_default(CommandHandler, &CommandMgr::HandleAdmin));
	RegisterHandler("channel",     cast_default(CommandHandler, &CommandMgr::HandleChannel));
	RegisterHandler("funkcio",     cast_default(CommandHandler, &CommandMgr::HandleFunkciok));
	RegisterHandler("autofunkcio", cast_default(CommandHandler, &CommandMgr::HandleAutoFunkcio));
	RegisterHandler("szinek",      cast_default(CommandHandler, &CommandMgr::HandleSzinek));
	RegisterHandler("sznap",       cast_default(CommandHandler, &CommandMgr::HandleSznap));
	RegisterHandler("nick",        cast_default(CommandHandler, &CommandMgr::HandleNick));
	RegisterHandler("join",        cast_default(CommandHandler, &CommandMgr::HandleJoin));
	RegisterHandler("left",        cast_default(CommandHandler, &CommandMgr::HandleLeft));
	RegisterHandler("kick",        cast_default(CommandHandler, &CommandMgr::HandleKick));
	RegisterHandler("mode",        cast_default(CommandHandler, &CommandMgr::HandleMode));
	RegisterHandler("svn",         cast_default(CommandHandler, &CommandMgr::HandleSvn));
	RegisterHandler("git",         cast_default(CommandHandler, &CommandMgr::HandleGit));
	RegisterHandler("hg",          cast_default(CommandHandler, &CommandMgr::HandleHg));

	// Adminisztrátor parancsok
	RegisterHandler("teszt",       cast_default(CommandHandler, &CommandMgr::HandleTeszt));
	RegisterHandler("reload",      cast_default(CommandHandler, &CommandMgr::HandleReload));
	RegisterHandler("kikapcs",     cast_default(CommandHandler, &CommandMgr::HandleKikapcsolas));

	Log.Notice("CommandMgr", "Osszes Command handler regisztralva.");
}

void CommandMgr::RegisterHandler(string code, CommandHandler method)
{
	CommandHandlerMap.insert(make_pair(code, method));
}

int CommandMgr::writer(char* data, size_t size, size_t nmemb, string *buffer)
{
	int result = NULL;

	if(buffer != NULL)
	{
		buffer->append(data, size* nmemb);
		result = size* nmemb;
	}
	return result;
}

void CommandMgr::CNick(CommandMessage& recvData)
{
	if(cast_int(recvData.Channel.find("#")) == string::npos)
		recvData.Channel = recvData.Nick;
}z
