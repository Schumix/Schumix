/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2008 Valroft <http://www.aspiredev.org/>
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

#ifndef _SCHUMIX_MYSQL_CONNECTION_HPP
#define _SCHUMIX_MYSQL_CONNECTION_HPP

typedef void(*mySQLCallback)(QueryResultPointer);
struct ASyncQuery
{
	mySQLCallback callback;
	string query;
};

class MySQLConnection : public boost::enable_shared_from_this<MySQLConnection>
{
public:
	MySQLConnection(string host, string user, string password);
	~MySQLConnection();

	void UseDatabase(string database);
	void Execute(string query);
	QueryResultPointer Query(const char * query, ...);

	string EscapeString(string Escape);

	void QueryASync(mySQLCallback callback, const char * query, ...);
	volatile bool kiiras;
	inline bool GetSqlError() { return sql_error; }

protected:
	MYSQL* handle;

	string m_Host;
	string m_User;
	string m_Password;
	string m_Database;
	bool sql_error;

	deque<ASyncQueryPointer> AsynchronousQueries;
};

#endif
