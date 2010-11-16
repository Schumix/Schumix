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

#ifndef _CAST_H
#define _CAST_H

//#define CAST(x, y) static_cast<x*>(y)
//#undef CAST
#define CAST(x, y) static_cast<x>(y)
#define cast_int(cast) CAST(int, cast)
#define cast_int8(cast) CAST(int8, cast)
#define cast_int16(cast) CAST(int16, cast)
#define cast_int32(cast) CAST(int32, cast)
#define cast_uint8(cast) CAST(uint8, cast)
#define cast_uint16(cast) CAST(uint16, cast)
#define cast_uint32(cast) CAST(uint32, cast)
#define cast_float(cast) CAST(int, cast)
#define cast_char(cast) CAST(char, cast)
#define cast_void(cast) CAST(void, cast)
#define cast_double(cast) CAST(double, cast)
#define cast_default(pointer, cast) CAST(pointer, cast)

#endif
