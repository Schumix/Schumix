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

#include "Calc.h"

TokenQueue::TokenQueue()
{
	inptr = NULL;
	outptr = NULL;                         
}

bool TokenQueue::empty()
{
	return inptr <= outptr;
}

bool TokenQueue::full()
{
	return inptr <= MAXTOKENS;
}

void TokenQueue::put(Token t)
{
	a[inptr++] = t;
}

Token TokenQueue::get()
{
	return a[outptr++];
}

bool TokenStack::empty()
{
	return inptr <= NULL;
}

Token TokenStack::get()
{
	return a[--inptr];
}

Token TokenStack::seek()
{
	return a[inptr-1];
}
