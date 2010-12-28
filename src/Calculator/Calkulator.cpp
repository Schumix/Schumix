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

TokenQueue q;
TokenStack sk;
VarVector varv;

void IRCSession::setConsts()
{
	varv.setVar("pi", 3.1415926535897932384626433832795f, true);  
}

Token getValue(Token t)
{
	if(t.tkntype == TKN_NUMBER)
		return t;
	else
	{
		t.tkntype = TKN_NUMBER;
		t.number = varv.getVar(t.str);
		return t;
	}
}

void IRCSession::makepolishform(string szam)
{
	string s = szam;
	q = TokenQueue();
	sk = TokenStack();
	CalcLexer cl = CalcLexer(s);
	Token t;
	do
	{
		if(t.tkntype == TKN_NEXT_LINE)
			cl = CalcLexer(s);

		t = cl.getNextToken();
		if(t.tkntype == TKN_NUMBER || t.tkntype == TKN_NAME)
			q.put(t);
		else if(t.tkntype == TKN_OPEN_PAR)
			sk.put(t);
		else if(t.tkntype == TKN_CLOSE_PAR)
		{
			Token t1;
			while(!sk.empty() && sk.seek().tkntype != TKN_OPEN_PAR)
				q.put(sk.get());

			sk.get();
		}
		else if(t.tkntype == TKN_OPERATOR)
		{
			while(!sk.empty() && getpriority(sk.seek()) <= getpriority(t))
				q.put(sk.get());   

			sk.put(t);
		}
	} while(t.tkntype != TKN_TERMINATION);

	while(!sk.empty())
		q.put(sk.get());
}

void IRCSession::calculate(string privmsg)
{
	sk = TokenStack();
	while(!q.empty())
	{
		Token t = q.get();
		if(t.tkntype == TKN_NUMBER || t.tkntype == TKN_NAME)
			sk.put(t);
		else if(t.tkntype == TKN_OPERATOR)
		{
			if(t.str == "+")
			{
				Token o1, o2;
				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o2 = getValue(sk.get());

				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o1 = getValue(sk.get());

				Token e = Token(TKN_NUMBER, o1.number+o2.number, "");
				sk.put(e);             
			}
			else if(t.str == "-")
			{
				Token o1, o2;
				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o2 = getValue(sk.get());

				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o1 = getValue(sk.get());

				Token e = Token(TKN_NUMBER, o1.number-o2.number, "");
				sk.put(e);             
			}
			else if(t.str == "*")
			{
				Token o1, o2;
				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o2 = getValue(sk.get());

				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o1 = getValue(sk.get());

				Token e = Token(TKN_NUMBER, o1.number*o2.number, "");
				sk.put(e);             
			}
			else if(t.str == "/")
			{
				Token o1, o2;
				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o2 = getValue(sk.get());

				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o1 = getValue(sk.get());

				Token e = Token(TKN_NUMBER, o1.number/o2.number, "");
				sk.put(e);             
			}
			else if(t.str == "%")
			{
				Token o1, o2;
				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o2 = getValue(sk.get());

				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o1 = getValue(sk.get());

				Token e = Token(TKN_NUMBER, cast_int(o1.number)%cast_int(o2.number), "");
				sk.put(e);             
			}
			else if(t.str == "^" || t.str == "**")
			{
				Token o1, o2;
				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o2 = getValue(sk.get());

				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o1 = getValue(sk.get());

				Token e = Token(TKN_NUMBER, pow(o1.number, o2.number), "");
				sk.put(e);             
			}
			/*else if(t.str == "sin")
			{
				Token o1, o2;
				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o2 = getValue(sk.get());

				if(!sk.empty() && (sk.seek().tkntype == TKN_NUMBER || sk.seek().tkntype == TKN_NAME))
					o1 = getValue(sk.get());

				Token e = Token(TKN_NUMBER, sin(12), "");
				sk.put(e);             
			}*/
		}
	}

	double d;

	if(!sk.empty())
		d = getValue(sk.get()).number;
	else
		d = 0.0f;

	if(d-cast_int(d) < 0.00000049)
		SendChatMessage(PRIVMSG, privmsg.c_str(), "%d", cast_int(d));
	else
		SendChatMessage(PRIVMSG, privmsg.c_str(), "%f", d);

	varv.clear();
}
