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

#ifndef _SCHUMIX_HPP
#define _SCHUMIX_HPP

namespace Schumix
{
#if PLATFORM == PLATFORM_WINDOWS
	inline uint32 getMSTime()
	{
		return GetTickCount();
	}
#else
inline uint32 getMSTime()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	}
#endif

	inline vector<string> split(const string str, const string delim, vector<string>& result)
	{
		int at = 0;
		string text = str;

		while((at = text.find_first_of(delim)) != text.npos)
		{
			if(at > 0)
				result.push_back(text.substr(0, at));

			text = text.substr(at+1);
		}

		if(text.length() > 0)
			result.push_back(text);

		return result;
	}

	inline int Ev()
	{
		time_t raw;
		struct tm *tinf = NULL;
		char* buffer = new char[80];

		time(&raw);
		tinf = localtime(&raw);

		strftime(buffer, 80, "%Y", tinf);
		string ev(buffer);
		delete[] buffer;

		stringstream ss;
		int iev;
		ss << ev;
		ss >> iev;

		return iev;
	}

	inline int Honap()
	{
		time_t raw;
		struct tm *tinf = NULL;
		char* buffer = new char[80];

		time(&raw);
		tinf = localtime(&raw);

		strftime(buffer, 80, "%m", tinf);
		string honap(buffer);
		delete[] buffer;

		stringstream ss;
		int ihonap;
		ss << honap;
		ss >> ihonap;

		return ihonap;
	}

	inline int Nap()
	{
		time_t raw;
		struct tm *tinf = NULL;
		char* buffer = new char[80];

		time(&raw);
		tinf = localtime(&raw);

		strftime(buffer, 80, "%d", tinf);
		string nap(buffer);
		delete[] buffer;

		stringstream ss;
		int inap;
		ss << nap;
		ss >> inap;

		return inap;
	}

	inline int Ora()
	{
		time_t raw;
		struct tm *tinf = NULL;
		char* buffer = new char[80];

		time(&raw);
		tinf = localtime(&raw);

		strftime(buffer, 80, "%H", tinf);
		string ido(buffer);
		delete[] buffer;

		stringstream ss;
		int iido;
		ss << ido;
		ss >> iido;

		return iido;
	}

	inline int Perc()
	{
		time_t raw;
		struct tm *tinf = NULL;
		char* buffer = new char[80];

		time(&raw);
		tinf = localtime(&raw);

		strftime(buffer, 80, "%M", tinf);
		string perc(buffer);
		delete[] buffer;

		stringstream ss;
		int iperc;
		ss << perc;
		ss >> iperc;

		return iperc;
	}
}

#endif
