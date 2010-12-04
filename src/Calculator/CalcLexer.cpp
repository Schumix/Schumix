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

int getpriority(Token tk)
{
	if(tk.str == "^")
		return 5;
	else if(tk.str == "**")
		return 5;
	else if(tk.str == "*")
		return 6;
	else if(tk.str == "/")
		return 6;
	else if(tk.str == "%")
		return 6;
	else if(tk.str == "+")
		return 7;
	else if(tk.str == "-")
		return 7;
	else if(tk.str == ":")
		return 16;
	else if(tk.str == "(")
		return 100;
	else
		return 0;
}

string CalcLexer::swspace = " \n\r\t";
string CalcLexer::snumchr2 = "0123456789._hxrob";
string CalcLexer::snumbers = "_0123456789.";
string CalcLexer::sbin = "_01";
string CalcLexer::shexa = "_0123456789abcdef";
string CalcLexer::soctal = "_01234567";
string CalcLexer::sroman = "_ivxlcdm";
string CalcLexer::s1cops = "~+-/*%!:^=<>()";
//string CalcLexer::s2cops = " sin ";
string CalcLexer::upletters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ[]{}";
string CalcLexer::downletters = "abcdefghijklmnopqrstuvwxyz()()";

string CalcLexer::strlower(string s)
{
	for(int i = 0; i < cast_int(s.length()); i++)
	{
		if(upletters.find(s[i]) != string::npos)
			s[i] = downletters[upletters.find(s[i])];
	}
	return s;
}

bool CalcLexer::c_in_s(char c, string s)
{
	return s.find(c) != string::npos;
}

bool CalcLexer::s_in_s(string kw, string s)
{
	return s.find(" " + kw + " ") != string::npos;
}

double CalcLexer::bin_string_to_f(string s)
{
	double d = 0;
	int i, p;
	for(i = cast_int(s.length())-1, p = 0; i >= 0; i--, p++)
	{
		if(s[i] == '1')
#if PLATFORM == PLATFORM_WINDOWS
			d += pow(2, float(p));
#else
			d += pow(2, cast_float(p));
#endif
	}

	return d;
}

int CalcLexer::getHexVal(char c)
{
	switch(c)
	{
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'a':
			return 10;
		case 'b':
			return 11;
		case 'c':
			return 12;
		case 'd':
			return 13;
		case 'e':
			return 14;
		case 'f':
			return 15;
		default:
			return -1;
	}
}

int CalcLexer::getRomanVal(char c)
{
	switch(c)
	{
		case 'm':
			return 1000;
		case 'd':
			return 500;
		case 'c':
			return 100;
		case 'l':
			return 50;
		case 'x':
			return 10;
		case 'v':
			return 5;
		case 'i':
			return 1;
		default:
			return -1;
	}
}

double CalcLexer::hex_string_to_f(string s)
{
	double d = 0;
	int i, p;
	for(i = cast_int(s.length())-1, p = 0; i >= 0; i--, p++)
#if PLATFORM == PLATFORM_WINDOWS
		d += getHexVal(s[i])*pow(16, float(p));
#else
		d += getHexVal(s[i])*pow(16, cast_float(p));
#endif

	return d;
}

double CalcLexer::oct_string_to_f(string s)
{
	double d = 0;
	int i, p;
	for(i = cast_int(s.length())-1, p = 0; i >= 0; i--, p++)
	{
		char c = s[i];
#if PLATFORM == PLATFORM_WINDOWS
		d += atoi(&c)*pow(8, float(p));
#else
		d += atoi(&c)*pow(8, cast_float(p));
#endif
	} 
	return d;
}

double CalcLexer::roman_to_f(string s)
{
	int lastval = 0;
	double d = 0;
	int i;
	for(i = 0; i < cast_int(s.length()); i++)
	{
		int val = getRomanVal(s[i]);
		if(lastval < val)
			d -= lastval;
		else
			d += lastval;

		lastval = val;
	}

	d += lastval;
	return d;
}

Token::Token(string tt, double d, string s)
{
	tkntype = tt;
	number = d; 
	str = s;
}
Token::Token()
{
	tkntype = TKN_UNKNOWN;
	number = 0;
	str = "";              
}

CalcLexer::CalcLexer(string mystr)
{
	str = strlower(mystr);
	pos = 0;                            
	len = cast_int(str.length());
}

string CalcLexer::getFloat()
{
	string s = "";
	while(pos < len && c_in_s(str[pos], snumbers))
	{
		if(str[pos]!='_') s += str[pos];
		pos++;
	}
	pos--;
	return s;
}

string CalcLexer::getBinary()
{
	pos += 2;
	string s;
	while(pos < len && c_in_s(str[pos], sbin))
	{
		if(str[pos]!='_') s += str[pos];
		pos++;
	} 
	pos--;
	return s;
}

string CalcLexer::getHexa()
{
	pos += 2;
	string s;
	while(pos < len && c_in_s(str[pos] , shexa))
	{
		if(str[pos]!='_') s+=str[pos];
		pos++;
	} 
	pos--;
	return s;
}

string CalcLexer::getOctal()
{
	pos += 2;
	string s;
	while(pos < len && c_in_s(str[pos], soctal))
	{
		if(str[pos]!='_')  s += str[pos];
		pos++;
	} 
	pos--;
	return s;
}

string CalcLexer::getRoman()
{
	pos += 2;
	string s;
	while(pos < len && c_in_s(str[pos], sroman))
	{
		if(str[pos]!='_')  s += str[pos];
		pos++;
	} 
	pos--;
	return s;
}

Token CalcLexer::getNextToken()
{ 
	Token tkn = Token();
	while(pos < len && c_in_s(str[pos], swspace))
		pos++;

	if(pos < len)
	{
		string tmps = "";
		//2 ch long ops       
		/*if(pos+1 < len && s_in_s((tmps = "", tmps += str[pos], tmps += str[pos+1]), s2cops))
		{
			tkn.tkntype = TKN_OPERATOR;
			tkn.str = tmps;
			pos++; // because its 2 char
		}*/
		//1 ch long ops       
		if(c_in_s(str[pos], s1cops))
		{
			if (str[pos] == '(')
			{
				tkn.tkntype = TKN_OPEN_PAR;
				tkn.str = '(';
			}
			else if(str[pos] == ')')
			{
				tkn.tkntype = TKN_CLOSE_PAR;
				tkn.str = ')';
			}
			else
			{
				tkn.tkntype = TKN_OPERATOR;
				tkn.str = str[pos];
			}
		}
		//numbers
		else if(('0' <= str[pos] && str[pos] <= '9') || str[pos] == '.')
		{
			tkn.tkntype = TKN_NUMBER;
			if(((pos+1)<len) && c_in_s (str[pos+1], snumchr2))
			{
				if(c_in_s(str[pos+1], snumbers))
				{
					string s = getFloat();
					tkn.number = atof(s.c_str());
				}

				if(str[pos+1] == 'b')
				{
					string s = getBinary();
					tkn.number = bin_string_to_f(s);
				}

				if(str[pos+1] == 'h' || str[pos+1] == 'x')
				{
					string s = getHexa();
					tkn.number = hex_string_to_f(s);
				}

				if(str[pos+1] == 'o')
				{
					string s = getOctal();
					tkn.number = oct_string_to_f(s);
				}

				if(str[pos+1] == 'r')
				{
					string s = getRoman();
					tkn.number = roman_to_f(s);
				}
			}
			else
			{
				char c = str[pos];
				tkn.number = atof(&c);
			}
		}
		pos++;
	}
	else
		tkn.tkntype = TKN_TERMINATION;

	return tkn;
}
