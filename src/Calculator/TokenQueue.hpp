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

#ifndef _SCHUMIX_TOKEN_QUEUE_HPP
#define _SCHUMIX_TOKEN_QUEUE_HPP

class TokenQueue
{
public:
	TokenQueue();
	virtual bool empty();
	bool full();
	void put(Token t);
	virtual Token get();

protected:
	Token a[MAXTOKENS];
	int inptr;
	int outptr;
};

class TokenStack : public TokenQueue
{
public:
	bool empty();
	Token get();
	Token seek();
};

#endif
