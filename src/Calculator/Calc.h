/*
 * This file is part of Schumix.
 * 
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

#ifndef _SCHUMIX_CALC_H
#define _SCHUMIX_CALC_H

#define LG 1
#define RG 2
#define EQ 0

#define MAXTOKENS 1000

#define TKN_UNKNOWN string("TKN_UNKNOWN")
#define TKN_NUMBER string("TKN_NUMBER")
#define TKN_OPERATOR string("TKN_OPERATOR")
#define TKN_NAME string("TKN_NAME")
#define TKN_OPEN_PAR string("TKN_OPEN_PAR")
#define TKN_CLOSE_PAR string("TKN_CLOSE_PAR")
#define TKN_TERMINATION string("TKN_TERMINATION")
#define TKN_NEXT_LINE string("TKN_NEXT_LINE")

#include "../StdAfx.h"
#include "VarVector.hpp"
#include "CalcLexer.hpp"
#include "TokenQueue.hpp"

#endif
