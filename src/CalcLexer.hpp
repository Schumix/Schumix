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

#ifndef _SCHUMIX_CALCLEXER_HPP
#define _SCHUMIX_CALCLEXER_HPP

struct Token
{
	Token(string tt, double d = NULL, string s = "");
	Token();
	string tkntype;
	double number;
	string str;
};

class CalcLexer
{
public:
	CalcLexer(string mystr);
	Token getNextToken();

protected:
	static string swspace;
	static string snumchr2;
	static string snumbers;
	static string sbin;
	static string shexa;
	static string soctal;
	static string sroman;
	static string s1cops;
	//static string s2cops;
	static string upletters;
	static string downletters;

	bool c_in_s(char c, string s);
	bool s_in_s(string kw, string s);
	double bin_string_to_f(string s);
	double hex_string_to_f(string s);
	double oct_string_to_f(string s);
	double roman_to_f(string s);

	string strlower(string s);
	int getHexVal(char c);
	int getRomanVal(char c);

	string str;
	int pos;
	int len;
	string getFloat();
	string getBinary();
	string getOctal();
	string getRoman();
	string getHexa();
};

int getpriority(Token tk);

#endif
