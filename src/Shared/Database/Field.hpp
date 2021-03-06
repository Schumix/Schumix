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

#ifndef _SCHUMIX_FIELD_HPP
#define _SCHUMIX_FIELD_HPP

class Field
{
public:
	void SetValue(char* value) { m_Value = value; }

	const char* GetString() { return m_Value; }
	float GetFloat() { return m_Value ? cast_float(atof(m_Value)) : 0.0f; }
	bool GetBool() { return m_Value ? atoi(m_Value) > 0 : false; }
	int8 GetInt8() { return m_Value ? cast_int8(atol(m_Value)) : cast_int8(0); }
	int16 GetInt16() { return m_Value ? cast_int16(atol(m_Value)) : cast_int16(0); }
	//int32 GetInt32() { return m_Value ? cast_int32(atol(m_Value)) : NULL; }
	uint8 GetUInt8() { return m_Value ? cast_uint8(atol(m_Value)) : cast_uint8(0); }
	uint16 GetUInt16() { return m_Value ? cast_uint16(atol(m_Value)) : cast_uint16(0); }
	uint32 GetUInt32() { return m_Value ? cast_uint32(atol(m_Value)) : cast_uint32(0); }
	uint64 GetUInt64()
	{
		if(m_Value)
		{
			uint64 value;
			sscanf(m_Value, I64FMTD, &value);
			return value;
		}
		else
			return NULL;
	}

private:
	char* m_Value;
};

#endif
