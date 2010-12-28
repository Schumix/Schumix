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

#include "Calc.h"

Variable::Variable(string s, double d, bool c)
{
	name = s;
	value = d;     
	cons = c;                     
}

double VarVector::getVar(string s)
{
	if(s.length() < 1)
		return 0.0f;

	int maxi = cast_int(size());
	for(int i = 0; i < maxi; i++)
	{
		if(at(i).name == s)
			return at(i).value;
	}
	insert(end(), Variable(s, 0.0f));
	return 0.0f;
}

void VarVector::setVar(string s, double d, bool c)
{
	if(s.length() < 1)
		return;

	int maxi = cast_int(size());
	for(int i = 0; i < maxi; i++)
	{
		if(at(i).name == s)
		{
			if(!at(i).cons)
			{
				at(i).value = d;
				at(i).cons = c;
			}
			return;
		}
	}
	insert(end(), Variable(s, d, c));
	return;
}

void VarVector::delVar(string s)
{
	int maxi = cast_int(size());
	for(int i = 0; i < maxi; i++)
	{
		if(at(i).name == s)
		{
			if(!at(i).cons)
				erase(begin()+i);

			return;
		}
	}
	return;
}
