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

#ifndef _LOG_H
#define _LOG_H

#if PLATFORM == PLATFORM_WINDOWS

#define TRED FOREGROUND_RED | FOREGROUND_INTENSITY
#define TGREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define TYELLOW FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define TNORMAL FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE
#define TWHITE TNORMAL | FOREGROUND_INTENSITY
#define TBLUE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define TPURPLE FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY

#else

#define TRED 1
#define TGREEN 2
#define TYELLOW 3
#define TNORMAL 4
#define TWHITE 5
#define TBLUE 6
#define TPURPLE 7

#endif

class CLog
{
public:
	CLog()
	{
#if PLATFORM == PLATFORM_WINDOWS
		stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
		stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
		UNIXTIME = time(&UNIXTIME);
	}

	void Color(unsigned int color)
	{
#if PLATFORM == PLATFORM_UNIX
		const char* colorstrings[TBLUE+1] = {
			"",
			"\033[22;31m",
			"\033[22;32m",
			"\033[01;33m",
			//"\033[22;37m",
			"\033[0m",
			"\033[01;37m",
			"\033[1;34m",
		};

		fputs(colorstrings[color], stdout);
#else
		SetConsoleTextAttribute(stdout_handle, (WORD)color);
#endif
	}

	void Notice(const char * source, const char * format, ...)
	{
		Guard g(mutex);

		va_list ap;
		va_start(ap, format);
		Time();
		fputs("N ", stdout);
		if(*source)
		{
			Color(TWHITE);
			fputs(source, stdout);
			putchar(':');
			putchar(' ');
			Color(TNORMAL);
		}

		vprintf(format, ap);
		putchar('\n');
		va_end(ap);
		Color(TNORMAL);
	}

	void Info(const char * source, const char * format, ...)
	{
		Guard g(mutex);

		va_list ap;
		va_start(ap, format);
		Time();
		fputs("N ", stdout);
		if(*source)
		{
			Color(TWHITE);
			fputs(source, stdout);
			putchar(':');
			putchar(' ');
			Color(TPURPLE);
		}

		vprintf(format, ap);
		putchar('\n');
		va_end(ap);
		Color(TNORMAL);
	}

	void Warning(const char * source, const char * format, ...)
	{
		Guard g(mutex);

		va_list ap;
		va_start(ap, format);
		Time();
		Color(TYELLOW);
		fputs("W ", stdout);
		if(*source)
		{
			Color(TWHITE);
			fputs(source, stdout);
			putchar(':');
			putchar(' ');
			Color(TYELLOW);
		}

		vprintf(format, ap);
		putchar('\n');
		va_end(ap);
		Color(TNORMAL);
	}

	void Success(const char * source, const char * format, ...)
	{
		Guard g(mutex);

		va_list ap;
		va_start(ap, format);
		Time();
		Color(TGREEN);
		fputs("S ", stdout);
		if(*source)
		{
			Color(TWHITE);
			fputs(source, stdout);
			putchar(':');
			putchar(' ');
			Color(TGREEN);
		}

		vprintf(format, ap);
		putchar('\n');
		va_end(ap);
		Color(TNORMAL);
	}

	void Error(const char * source, const char * format, ...)
	{
		Guard g(mutex);

		va_list ap;
		va_start(ap, format);
		Time();
		Color(TRED);
		fputs("E ", stdout);
		if(*source)
		{
			Color(TWHITE);
			fputs(source, stdout);
			putchar(':');
			putchar(' ');
			Color(TRED);
		}

		vprintf(format, ap);
		putchar('\n');
		va_end(ap);
		Color(TNORMAL);
	}

	void Debug(const char * source, const char * format, ...)
	{
		Guard g(mutex);

		va_list ap;
		va_start(ap, format);
		Time();
		Color(TBLUE);
		fputs("D ", stdout);
		if(*source)
		{
			Color(TWHITE);
			fputs(source, stdout);
			putchar(':');
			putchar(' ');
			Color(TBLUE);
		}

		vprintf(format, ap);
		putchar('\n');
		va_end(ap);
		Color(TNORMAL);
	}

#define LARGERRORMESSAGE_ERROR 1
#define LARGERRORMESSAGE_WARNING 2
	
	void LargeErrorMessage(uint32 Colour, ...)
	{
		Guard g(mutex);

		vector<char*> lines;
		char* pointer;
		va_list ap;
		va_start(ap, Colour);
		
		size_t i, j, k;
		pointer = va_arg(ap, char*);
		while(pointer != NULL)
		{
			lines.push_back(pointer);
			pointer = va_arg(ap, char*);
		}

		delete pointer;

		if(Colour == LARGERRORMESSAGE_ERROR)
			Color(TRED);
		else
			Color(TYELLOW);

		printf("*********************************************************************\n");
		printf("*                        MAJOR ERROR/WARNING                        *\n");
		printf("*                        ===================                        *\n");

		for(vector<char*>::iterator itr = lines.begin(); itr != lines.end(); ++itr)
		{
			i = strlen(*itr);
			j = (i <= 65) ? 65 - i : 0;

			printf("* %s", *itr);
			for( k = 0; k < j; ++k )
				printf(" ");

			printf(" *\n");
		}

		printf("*********************************************************************\n");

#if PLATFORM == PLATFORM_WINDOWS
		string str = "MAJOR ERROR/WARNING:\n";
		for(vector<char*>::iterator itr = lines.begin(); itr != lines.end(); ++itr)
		{
			str += *itr;
			str += "\n";
		}

		MessageBoxA(0, str.c_str(), "Error", MB_OK);
#else
		printf("Sleeping for 5 seconds.\n");
		Sleep(5000);
#endif

		Color(TNORMAL);
	}

protected:
#if PLATFORM == PLATFORM_WINDOWS
	HANDLE stdout_handle, stderr_handle;
#endif
	Mutex mutex;
	//int32 log_level; Egyenlõre nincs használva.
	time_t UNIXTIME; /* update this every loop to avoid the time() syscall! */

	inline void Time()
	{
		tm * t = localtime(&UNIXTIME);
		printf("%02u:%02u ", t->tm_hour, t->tm_min);
	}
};

#endif

