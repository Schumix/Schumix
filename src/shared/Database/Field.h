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

#ifndef _FIELD_H
#define _FIELD_H

class Field
{
public:
	void SetValue(char* value) { mValue = value; }

	const char* GetString() { return mValue; }
	float GetFloat() { return mValue ? cast_float(atof(mValue)) : NULL; }
	bool GetBool() { return mValue ? atoi(mValue) > NULL : false; }
	int8 GetInt8() { return mValue ? cast_int8(atol(mValue)) : NULL; }
	int16 GetInt32() { return mValue ? cast_int16(atol(mValue)) : NULL; }
	//int32 GetInt32() { return mValue ? cast_int32(atol(mValue)) : NULL; }
	uint8 GetUInt8() { return mValue ? cast_uint8(atol(mValue)) : NULL; }
	uint16 GetUInt16() { return mValue ? cast_uint16(atol(mValue)) : NULL; }
	uint32 GetUInt32() { return mValue ? cast_uint32(atol(mValue)) : NULL; }
	uint64 GetUInt64()
	{
		if(mValue)
		{
			uint64 value;
			sscanf(mValue, I64FMTD, &value);
			return value;
		}
		else
			return NULL;
	}

private:
	char* mValue;
};

#endif
