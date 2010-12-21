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
	RegisterHandle("xbot",        cast_default(CommandHandler, &CommandMgr::HandleXbot));
	RegisterHandle("help",        cast_default(CommandHandler, &CommandMgr::HandleHelp));
	RegisterHandle("szinek",      cast_default(CommandHandler, &CommandMgr::HandleSzinek));
	RegisterHandle("info",        cast_default(CommandHandler, &CommandMgr::HandleInfo));
	RegisterHandle("roll",        cast_default(CommandHandler, &CommandMgr::HandleRoll));
	RegisterHandle("admin",       cast_default(CommandHandler, &CommandMgr::HandleAdmin));
	RegisterHandle("hozzaferes",  cast_default(CommandHandler, &CommandMgr::HandleHozzaferes));
	RegisterHandle("ujjelszo",    cast_default(CommandHandler, &CommandMgr::HandleUjjelszo));
	RegisterHandle("datum",       cast_default(CommandHandler, &CommandMgr::HandleDatum));
	RegisterHandle("ido",         cast_default(CommandHandler, &CommandMgr::HandleIdo));
	RegisterHandle("funkcio",     cast_default(CommandHandler, &CommandMgr::HandleFunkciok));
	RegisterHandle("keres",       cast_default(CommandHandler, &CommandMgr::HandleKeres));
	RegisterHandle("fordit",      cast_default(CommandHandler, &CommandMgr::HandleForditas));
	RegisterHandle("teszt",       cast_default(CommandHandler, &CommandMgr::HandleTeszt));
	RegisterHandle("sha1",        cast_default(CommandHandler, &CommandMgr::HandleSha1));
	RegisterHandle("md5",         cast_default(CommandHandler, &CommandMgr::HandleMd5));
	RegisterHandle("channel",     cast_default(CommandHandler, &CommandMgr::HandleChannel));
	RegisterHandle("sznap",       cast_default(CommandHandler, &CommandMgr::HandleSznap));
	RegisterHandle("xrev",        cast_default(CommandHandler, &CommandMgr::HandleXrev));
	RegisterHandle("szoba",       cast_default(CommandHandler, &CommandMgr::HandleSzoba));
	RegisterHandle("irc",         cast_default(CommandHandler, &CommandMgr::HandleIrc));
	RegisterHandle("szam",        cast_default(CommandHandler, &CommandMgr::HandleSzam));
	RegisterHandle("nick",        cast_default(CommandHandler, &CommandMgr::HandleNick));
	RegisterHandle("join",        cast_default(CommandHandler, &CommandMgr::HandleJoin));
	RegisterHandle("left",        cast_default(CommandHandler, &CommandMgr::HandleLeft));
	RegisterHandle("kick",        cast_default(CommandHandler, &CommandMgr::HandleKick));
	RegisterHandle("mode",        cast_default(CommandHandler, &CommandMgr::HandleMode));
	RegisterHandle("uzenet",      cast_default(CommandHandler, &CommandMgr::HandleUzenet));
	RegisterHandle("hluzenet",    cast_default(CommandHandler, &CommandMgr::HandleHLFunkcio));
	RegisterHandle("svn",         cast_default(CommandHandler, &CommandMgr::HandleSvn));
	RegisterHandle("git",         cast_default(CommandHandler, &CommandMgr::HandleGit));
	RegisterHandle("hg",          cast_default(CommandHandler, &CommandMgr::HandleHg));
	RegisterHandle("jegyzet",     cast_default(CommandHandler, &CommandMgr::HandleJegyzet));
	RegisterHandle("reload",      cast_default(CommandHandler, &CommandMgr::HandleReload));
	RegisterHandle("whois",       cast_default(CommandHandler, &CommandMgr::HandleWhois));
	RegisterHandle("autofunkcio", cast_default(CommandHandler, &CommandMgr::HandleAutoFunkcio));
	RegisterHandle("kikapcs",     cast_default(CommandHandler, &CommandMgr::HandleKikapcsolas));

	Log.Notice("CommandMgr", "Osszes Command handler regisztralva.");
}

void CommandMgr::RegisterHandle(string code, CommandHandler method)
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
