/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2008 Valroft <http://www.aspiredev.org/>
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

#include "../../StdAfx.h"

QueryResult::QueryResult(MYSQL_RES* res, uint32 uFields, uint32 uRows)
{
	m_res = res;
	m_Fields = uFields;
	m_Rows = uRows;
	m_CurrentRow = new Field[uFields];
}

QueryResult::~QueryResult()
{
	mysql_free_result(m_res);
	delete[] m_CurrentRow;
}

bool QueryResult::NextRow()
{
	MYSQL_ROW row = mysql_fetch_row(m_res);
	if(row == NULL)
		return false;

	for(uint32 i = 0; i < m_Fields; ++i)
		m_CurrentRow[i].SetValue(row[i]);

	return true;
}
