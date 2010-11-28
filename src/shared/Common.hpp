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
 *
 * Written by: Twl
 */

#ifndef _SCHUMIX_COMMON_HPP
#define _SCHUMIX_COMMON_HPP

#include "Platforms.h"

#ifndef __cplusplus
#error "A valid C++ compiler is required."
#endif

#if ((defined(WIN32) || defined(WINDOWS) || defined(WIN_NT) || defined(WIN64)) && defined(_MSC_VER))
#define PLATFORM PLATFORM_WINDOWS
#define PLATFORM_TEXT "Win32"
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#define PLATFORM_TEXT "MacOSX"
#elif (defined(LINUX) || defined(__linux__) || defined(__unix__) || defined(UNIX) || defined(__BSD__))
#define PLATFORM PLATFORM_LINUX
#if !defined(__GNUC__)
#error "GNU C++ compiler is required."
#endif

#define GCC_VERSION (__GNUC__ * 10000 \
	+ __GNUC_MINOR__ * 100 \
	+ __GNUC_PATCHLEVEL__)

#ifdef USE_KQUEUE
#define PLATFORM_TEXT "FreeBSD"
#else
#define PLATFORM_TEXT "Linux"
#endif
#else
#define PLATFORM PLATFORM_OTHER
#endif

// create a wrapper to support existing code
#define PLATFORM_UNIX PLATFORM_LINUX

#include <boost/cstdint.hpp>

typedef boost::int8_t int8;
typedef boost::int16_t int16;
typedef boost::int32_t int32;
typedef boost::int64_t int64;

typedef boost::uint8_t uint8;
typedef boost::uint16_t uint16;
typedef boost::uint32_t uint32;
typedef boost::uint64_t uint64;

#if PLATFORM == PLATFORM_WINDOWS

#if _MSC_VER >= 1600
#define HAS_LAMBDAS 1
#define HAS_BUILTIN_POINTER 1
#endif

#else

#if GCC_VERSION >= 45000
#define HAS_LAMBDAS 1
#define HAS_BUILTIN_POINTER 1
#endif

#endif

#if PLATFORM == PLATFORM_WINDOWS
#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"
#define snprintf _snprintf
#define atoll __atoi64
#define NEWLINE "\r\n"
#pragma warning(disable: 4275)
#pragma warning(disable: 4996)
#pragma warning(disable: 4146)
#pragma warning(disable: 4267)
#pragma warning(disable: 4018)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)
#else
#define NEWLINE "\n"
#define stricmp strcasecmp
#define strnicmp strncasecmp
#define I64FMT "%016llX"
#define I64FMTD "%llu"
#define SI64FMTD "%lld"
#define Sleep(ms) usleep(ms*1000)
#endif

#endif
