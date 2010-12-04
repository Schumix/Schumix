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

#ifndef _SCHUMIX_SHARED_PTR_WRAPPER
#define _SCHUMIX_SHARED_PTR_WRAPPER

class Socket;
class MySQLConnection;
class QueryResult;
class Commands;
class CommandMgr;
struct ASyncQuery;

typedef boost::shared_ptr<Socket> SocketPointer;
typedef boost::shared_ptr<MySQLConnection> MySQLConnectionPointer;
typedef boost::shared_ptr<QueryResult> QueryResultPointer;
typedef boost::shared_ptr<Commands> CommandsPointer;
typedef boost::shared_ptr<CommandMgr> CommandMgrPointer;
typedef boost::shared_ptr<ASyncQuery> ASyncQueryPointer;

//#define NULLIRC						boost::shared_ptr<IRCSession>()
#define NULLSSO							boost::shared_ptr<Socket>()
//#define NULLSMGR						boost::shared_ptr<SocketMgr>()
#define NULLMSQL						boost::shared_ptr<MySQLConnection>()
#define NULLQRL							boost::shared_ptr<QueryResult>()
#define NULLASQ							boost::shared_ptr<ASyncQuery>()
//#define NULLGIF						boost::shared_ptr<GitInfo>()

#endif
