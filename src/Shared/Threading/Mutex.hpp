/*
 * This file is part of Schumix.
 * 
 * Cross-platform Mutex implementation.
 * Copyright (C) 2010 Twl
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

#ifndef _SCHUMIX_MUTEX_HPP
#define _SCHUMIX_MUTEX_HPP

/// Class which implements a mutex pattern. Supports pthread and Win API threads.
class Mutex
{
public:
	Mutex()
	{
#if PLATFORM == PLATFORM_WINDOWS
		InitializeCriticalSection(&m_lock);
#else
		pthread_mutex_init(&m_lock, NULL);
#endif
	}

	~Mutex()
	{
#if PLATFORM == PLATFORM_WINDOWS
		DeleteCriticalSection(&m_lock);
#else
		pthread_mutex_destroy(&m_lock);
#endif
	}

	/// Acquires the mutex.
	void Acquire()
	{
#if PLATFORM == PLATFORM_WINDOWS
		EnterCriticalSection(&m_lock);
#else
		pthread_mutex_lock(&m_lock);
#endif
	}

	/// Releases the mutex.
	void Release()
	{
#if PLATFORM == PLATFORM_WINDOWS
		LeaveCriticalSection(&m_lock);
#else
		pthread_mutex_unlock(&m_lock);
#endif
	}

	/// Alias for Acquire().
	void Lock() { Acquire(); }
	/// Alias for Release().
	void Unlock() { Release(); }

protected:
private:
#if PLATFORM == PLATFORM_WINDOWS
	CRITICAL_SECTION m_lock;
#else
	pthread_mutex_t m_lock;
#endif
};

#endif
