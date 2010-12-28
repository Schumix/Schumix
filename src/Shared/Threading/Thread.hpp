/*
 * This file is part of Schumix.
 * 
 * Cross-platform thread class.
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

#ifndef _SCHUMIX_THREAD_HPP
#define _SCHUMIX_THREAD_HPP

#if PLATFORM == PLATFORM_LINUX
#include <pthread.h>
#endif

#if PLATFORM == PLATFORM_WINDOWS
inline void ThreadExit(DWORD dwExitCode) { ExitThread(dwExitCode); }
#else
inline void ThreadExit(void** t_state) { pthread_exit(t_state); }
#endif

#if PLATFORM == PLATFORM_WINDOWS
#define Thread_void unsigned long WINAPI
#else
#define Thread_void void*
#endif

/// Cross-platform thread class
class Thread
{
public:
#if PLATFORM == PLATFORM_WINDOWS
	Thread(LPTHREAD_START_ROUTINE lpStartRoutine, LPVOID lpParam)
#else
	Thread(void *start_routine(void*), void* threadParam)
#endif
	{
#if PLATFORM == PLATFORM_WINDOWS
		m_thread = CreateThread(NULL, 0, lpStartRoutine, lpParam, 0, &m_threadId);
#else
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
		//pthread_attr_setdetachedstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_create(&m_thread, &attr, start_routine, threadParam);
		pthread_attr_destroy(&attr);
#endif 
	}

#if PLATFORM == PLATFORM_WINDOWS
	void Join(DWORD dwMiliseconds = INFINITE)
#else
	void Join(uint64_t ulMiliseconds = 0)
#endif
	{
#if PLATFORM == PLATFORM_WINDOWS
		WaitForSingleObject(m_thread, dwMiliseconds);
#else
		pthread_join(m_thread, NULL);
#endif
	}

#if PLATFORM == PLATFORM_WINDOWS
	void Suspend() { SuspendThread(m_thread); }
#else
	void Terminate() { pthread_cancel(m_thread); }
#endif
protected:
private:
#if PLATFORM == PLATFORM_WINDOWS
	HTHREAD m_thread;
	DWORD m_threadId;
#else
	pthread_t m_thread;
#endif
};

#endif
