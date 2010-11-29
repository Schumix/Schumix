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

#include "shared/Common.hpp"

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

#include "Schumix.hpp"
using namespace Schumix;

#include <curl/curl.h>
#include <mysql/mysql.h>

#include "shared/Threading.h"
#include "shared/Log.hpp"
extern CLog Log;
#include "shared/SharedPointer.hpp"
#include "shared/Cast.h"
#include "shared/format.h"
#include "shared/Singleton.h"
#include "shared/Config/Config.hpp"
#include "DefaultConfig.hpp"
#include "shared/SchumixConfig.h"
#include "shared/utf8.h"
#include "shared/Auth/MD5.hpp"
#include "shared/Auth/Sha1.hpp"
#include "Console.hpp"
#include "shared/Database/Field.hpp"
#include "shared/Database/QueryResult.hpp"
#include "shared/Database/MySQLConnection.hpp"
#include "SvnInfo.hpp"
#include "HgInfo.hpp"
#include "GitInfo.hpp"
#include "Replies.hpp"
#include "IRCSession.hpp"
#include "shared/Network/SocketMgr.hpp"
#include "shared/Network/Socket.hpp"
#include "shared/Network/SocketOps.hpp"
#include "Vezerlo.hpp"

#endif
