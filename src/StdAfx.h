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
#ifndef _SCHUMIX_STDAFX_H
#define _SCHUMIX_STDAFX_H

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
#include <signal.h>
#include <cassert>

using namespace std;
using std::string;

#include "Shared/Common.hpp"

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/pool/detail/guard.hpp>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/enable_shared_from_this.hpp>
//#include <boost/assert.hpp>

#if PLATFORM == PLATFORM_WINDOWS
#include <tchar.h>
#include "Shared/targetver.h"
#include <WinSock2.h>
#include <Windows.h>
#include <Psapi.h>
#include "Shared/WinAPIExtensions.h"
#include "Shared/dirent.h"
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

#include "Schumix.hpp"
using namespace Schumix;

#include <curl/curl.h>
#include <mysql/mysql.h>

#include "Shared/Threading.h"
#include "Shared/Log.hpp"
extern CLog Log;
#include "Shared/SharedPointer.hpp"
#include "Shared/Cast.h"
#include "Shared/format.h"
#include "Shared/Singleton.h"
#include "Shared/Config/Config.hpp"
#include "DefaultConfig/DefaultConfig.hpp"
#include "Shared/SchumixConfig.h"
#include "Shared/utf8.h"
#include "Shared/Auth/MD5.hpp"
#include "Shared/Auth/Sha1.hpp"
#include "Console/Console.hpp"
#include "Shared/Database/Field.hpp"
#include "Shared/Database/QueryResult.hpp"
#include "Shared/Database/MySQLConnection.hpp"
#include "Rss/SvnInfo.hpp"
#include "Rss/HgInfo.hpp"
#include "Rss/GitInfo.hpp"
#include "IRC/Replies.hpp"
#include "IRC/IRCSession.hpp"
#include "Commands/Commands.hpp"
#include "Commands/CommandMgr.hpp"
#include "Shared/Network/SocketMgr.hpp"
#include "Shared/Network/Socket.hpp"
#include "Shared/Network/SocketOps.hpp"
#include "Vezerlo/Vezerlo.hpp"

#endif
