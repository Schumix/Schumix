/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2010 Twl
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

// StdAfx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#ifndef STDAFX_INCLUDED_S
#define STDAFX_INCLUDED_S

#pragma once

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include <deque>
#include <queue>
#include <set>
#include <cstdarg>

using namespace std;
using std::string;

#include "shared/Common.h"

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/pool/detail/guard.hpp>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/enable_shared_from_this.hpp>

#if PLATFORM == PLATFORM_WINDOWS
#include <tchar.h>
#include "shared/targetver.h"
#include <WinSock2.h>
#include <Windows.h>
#include <Psapi.h>
#include "shared/WinAPIExtensions.h"
#include "shared/dirent.h"
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <dirent.h>
#endif

#if PLATFORM == PLATFORM_WINDOWS
inline uint32 getMSTime()
{
	return GetTickCount();
}
#else
inline uint32 getMSTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
#endif

#if PLATFORM == PLATFORM_WINDOWS
#include "../dep/VC/include/curl/curl.h"
#include "../dep/VC/include/mysql/mysql.h"
#else
#include <curl/curl.h>
#include <mysql/mysql.h>
#endif

#include "shared/Threading.h"
#include "shared/Log.h"
extern CLog Log;
#include "shared/SharedPointer.h"
#include "shared/Cast.h"
#include "shared/Singleton.h"
#include "shared/Config/Config.h"
#include "DefaultConfig.h"
#include "shared/SchumixConfig.h"
#include "shared/utf8.h"
#include "shared/Auth/MD5.h"
#include "shared/Auth/Sha1.h"
#include "Console.h"
#include "shared/Database/Field.h"
#include "shared/Database/QueryResult.h"
#include "shared/Database/MySQLConnection.h"
#include "SvnInfo.h"
#include "HgInfo.h"
#include "GitInfo.h"
#include "Replies.h"
#include "IRCSession.h"
#include "shared/Network/SocketMgr.h"
#include "shared/Network/Socket.h"
#include "shared/Network/SocketOps.h"
#include "Vezerlo.h"

#endif
