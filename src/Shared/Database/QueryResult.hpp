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

#ifndef _SCHUMIX_QUERY_RESULT_HPP
#define _SCHUMIX_QUERY_RESULT_HPP

class Field;

class QueryResult : public boost::enable_shared_from_this<QueryResult>
{
public:
	QueryResult(MYSQL_RES* res, uint32 uFields, uint32 uRows);
	~QueryResult();
	bool NextRow();
	Field* Fetch() { return m_CurrentRow; }

	uint32 GetAffectedFields() { return m_Fields; }
	uint32 GetAffectedRows() { return m_Rows; }

protected:
	MYSQL_RES* m_res;
	Field* m_CurrentRow;
	uint32 m_Fields;
	uint32 m_Rows;
};

#endif
