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

#include "../../StdAfx.h"

MySQLConnection::MySQLConnection(string host, string user, string password)
{
	sql_error = false;
	handle = mysql_init(NULL);
	MYSQL* temp = mysql_real_connect(handle, host.c_str(), user.c_str(), password.c_str(), "", 3306, "", NULL);
	if(temp == NULL)
	{
		Log.Error("MySQL", "Error connecting to database on %s", host.c_str());
		sql_error = true;
		return;
	}

	m_Host = host;
	m_User = user;
	m_Password = password;
	kiiras = true;
}

MySQLConnection::~MySQLConnection()
{
#ifdef _DEBUG_MOD
	Log.Notice("MySQLConnection", "~MySQLConnection()");
#endif

	if(handle)
		mysql_close(handle);
}

void MySQLConnection::Execute(string query)
{
	//dunno what it does ...leaving untouched 
	int result = mysql_query(handle, query.c_str());

	if(result > 0)
	{
		uint32 errnom = mysql_errno(handle);
		const char * reason = mysql_error(handle);

		if(!kiiras)
			Log.Notice("MySQL", "Query Failed: %s\nReason: %s", query.c_str(), reason);

		// Errors which require us to reconnect.
		if(errnom == 2006 || errnom == 2008 || errnom == 2013 || errnom == 2055)
		{
			handle = mysql_init(NULL);
			mysql_real_connect(handle, m_Host.c_str(), m_User.c_str(), m_Password.c_str(), "", 3306, "", NULL);
			if(!handle)
				return;

			UseDatabase(m_Database);
			Execute(query);
		}
	}
}

void MySQLConnection::QueryASync(mySQLCallback callback, const char * query, ...)
{
	char* sql = new char[16384];
	va_list vlist;
	va_start(vlist, query);
	vsnprintf(sql, 16384, query, vlist);
	va_end(vlist);

	string querystring = string(sql);
	delete[] sql;

	ASyncQueryPointer pQuery = ASyncQueryPointer(new ASyncQuery);
	pQuery->callback = callback;
	pQuery->query = querystring;

	AsynchronousQueries.push_back(pQuery);
}

QueryResultPointer MySQLConnection::Query(const char * query, ...)
{
	char* sql = new char[16384];
	va_list vlist;
	va_start(vlist, query);
	vsnprintf(sql, 16384, query, vlist);
	va_end(vlist);

	string querystring = string(sql);
	delete[] sql;

	Execute(querystring);
	QueryResultPointer res;
	MYSQL_RES* pRes = mysql_store_result(handle);
	uint32 uRows = cast_uint32(mysql_affected_rows(handle));
	uint32 uFields = cast_uint32(mysql_field_count(handle));

	if(uRows == cast_uint32(0) || uFields == cast_uint32(0) || pRes == NULL)
	{
		if(pRes != NULL)
			mysql_free_result(pRes);

		return NULLQRL;
	}

	res = QueryResultPointer(new QueryResult(pRes, uFields, uRows));
	res->NextRow();

	return res;
}

string MySQLConnection::EscapeString(string Escape)
{
	char a2[16384] = {0};
	const char* ret;
	if(mysql_real_escape_string(handle, a2, Escape.c_str(), cast_uint32(Escape.length())) == cast_uint32(0))
		ret = Escape.c_str();
	else
		ret = a2;

	return string(ret);
}

void MySQLConnection::UseDatabase(string database)
{
	mysql_select_db(handle, database.c_str());
	m_Database = database;
}
