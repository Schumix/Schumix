/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2004-2005 Antrix Team
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 AspireDev <http://www.aspiredev.org/>
 * Copyright (C) 2009-2010 Sandshroud <http://www.sandshroud.org/>
 * Copyright (C) 2010-2011 Sandshroud <http://www.sandshroud.org/>
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

#include "../../StdAfx.h"

#if PLATFORM == PLATFORM_WINDOWS
	#include <process.h>
#else
	volatile int threadid_count = 0;
	Mutex m_threadIdLock;
	int GenerateThreadId()
	{
		m_threadIdLock.Acquire();
		int i = ++threadid_count;
		m_threadIdLock.Release();
		return i;
	}
#endif

CThreadPool ThreadPool;

void SetThreadName(const char* format, ...)
{
	// This isn't supported on nix?
	va_list ap;
	va_start(ap, format);

#ifdef WIN32
	char thread_name[200];
	vsnprintf(thread_name, 200, format, ap);

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.dwThreadID = GetCurrentThreadId();
	info.dwFlags = 0;
	info.szName = thread_name;
	Log.Debug("ThreadMaker", "Creating thread %u with name %s", info.dwThreadID, info.szName);
	ThreadPool.SetThreadInfo(info.dwThreadID, info);

	__try
	{
#ifdef _WIN64
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (ULONG_PTR*)&info);
#else
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info);
#endif
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{

	}
#endif

	va_end(ap);
}

CThreadPool::CThreadPool()
{
	_threadsExitedSinceLastCheck = 0;
	_threadsRequestedSinceLastCheck = 0;
	_threadsEaten = 0;
	_threadsFreedSinceLastCheck = 0;
}

bool CThreadPool::ThreadExit(Thread * t)
{
	_mutex.Acquire();
	
	// we're definitely no longer active
	m_activeThreads.erase(t);

	// do we have to kill off some threads?
	if(_threadsToExit > 0)
	{
		// kill us.
		--_threadsToExit;
		++_threadsExitedSinceLastCheck;
		if(t->DeleteAfterExit)
			m_freeThreads.erase(t);

		_mutex.Release();
		delete t;
		return false;
	}

	// enter the "suspended" pool
	++_threadsExitedSinceLastCheck;
	++_threadsEaten;
	std::set<Thread*>::iterator itr = m_freeThreads.find(t);

	if(itr != m_freeThreads.end())
		printf("Thread %u duplicated with thread %u\n", (*itr)->ControlInterface.GetId(), t->ControlInterface.GetId());

	m_freeThreads.insert(t);

	Log.Debug("ThreadPool", "Thread %u entered the free pool.", t->ControlInterface.GetId());
	_mutex.Release();
	return true;
}

void CThreadPool::ExecuteTask(ThreadContext * ExecutionTarget)
{
	Thread* t;
	_mutex.Acquire();
	++_threadsRequestedSinceLastCheck;
	--_threadsEaten;

	// grab one from the pool, if we have any.
	if(m_freeThreads.size())
	{
		t = *m_freeThreads.begin();
		m_freeThreads.erase(m_freeThreads.begin());

		// execute the task on this thread.
		t->ExecutionTarget = ExecutionTarget;

		// resume the thread, and it should start working.
		t->ControlInterface.Resume();
		Log.Debug("ThreadPool", "Thread %u left the thread pool.", t->ControlInterface.GetId());
	}
	else
	{
		// creating a new thread means it heads straight to its task.
		// no need to resume it :)
		t = StartThread(ExecutionTarget);
	}

	// add the thread to the active set
#if PLATFORM == PLATFORM_WINDOWS
	Log.Debug("ThreadPool", "Thread %u is now executing task at 0x%p.", t->ControlInterface.GetId(), ExecutionTarget);
#else
	Log.Debug("ThreadPool", "Thread %u is now executing task at %p.", t->ControlInterface.GetId(), ExecutionTarget);
#endif
	m_activeThreads.insert(t);
	_mutex.Release();
}

void CThreadPool::Startup(uint8 ThreadCount /* = 8 */)
{
	for(uint8 i = 0; i < ThreadCount; ++i)
		StartThread(NULL);

	Log.Debug("ThreadPool", "Startup, launched %u threads.", ThreadCount);
}

void CThreadPool::ShowStats()
{
	_mutex.Acquire();
	Log.Debug("ThreadPool", "============ ThreadPool Status =============");
	Log.Debug("ThreadPool", "Active Threads: %u", m_activeThreads.size());
	Log.Debug("ThreadPool", "Suspended Threads: %u", m_freeThreads.size());
	Log.Debug("ThreadPool", "Requested-To-Freed Ratio: %.3f%% (%u/%u)", cast_float(cast_float(_threadsRequestedSinceLastCheck+1) / cast_float(_threadsExitedSinceLastCheck+1) * 100.0f), _threadsRequestedSinceLastCheck, _threadsExitedSinceLastCheck);
	Log.Debug("ThreadPool", "Eaten Count: %d (negative is bad!)", _threadsEaten);
	Log.Debug("ThreadPool", "============================================");
	_mutex.Release();
}

void CThreadPool::IntegrityCheck(uint8 ThreadCount)
{
	_mutex.Acquire();
	int32 gobbled = _threadsEaten;

	if(gobbled < 0)
	{
		// this means we requested more threads than we had in the pool last time.
		// spawn "gobbled" + THREAD_RESERVE extra threads.
		uint32 new_threads = abs(gobbled) + ThreadCount;
		_threadsEaten = 0;

		for(uint32 i = 0; i < new_threads; ++i)
			StartThread(NULL);

		Log.Debug("ThreadPool", "IntegrityCheck: (gobbled < 0) Spawning %u threads.", new_threads);
	}
	else if(gobbled < ThreadCount)
	{
		// this means while we didn't run out of threads, we were getting damn low.
		// spawn enough threads to keep the reserve amount up.
		uint32 new_threads = (THREAD_RESERVE - gobbled);
		for(uint32 i = 0; i < new_threads; ++i)
			StartThread(NULL);

		Log.Debug("ThreadPool", "IntegrityCheck: (gobbled <= 5) Spawning %u threads.", new_threads);
	}
	else if(gobbled > ThreadCount)
	{
		// this means we had "excess" threads sitting around doing nothing.
		// lets kill some of them off.
		uint32 kill_count = (gobbled - ThreadCount);
		KillFreeThreads(kill_count);
		_threadsEaten -= kill_count;
		Log.Debug("ThreadPool", "IntegrityCheck: (gobbled > 5) Killing %u threads.", kill_count);
	}
	else
	{
		// perfect! we have the ideal number of free threads.
		Log.Debug("ThreadPool", "IntegrityCheck: Perfect!");
	}

	_threadsExitedSinceLastCheck = 0;
	_threadsRequestedSinceLastCheck = 0;
	_threadsFreedSinceLastCheck = 0;

	_mutex.Release();
}

void CThreadPool::KillFreeThreads(uint32 count)
{
	Log.Debug("ThreadPool", "Killing %u excess threads.", count);
	_mutex.Acquire();
	Thread * t;
	ThreadSet::iterator itr;
	uint32 i;
	for(i = 0, itr = m_freeThreads.begin(); i < count && itr != m_freeThreads.end(); ++i, ++itr)
	{
		t = *itr;
		t->ExecutionTarget = NULL; 
		t->DeleteAfterExit = true;
		++_threadsToExit;
		t->ControlInterface.Resume();
	}
	_mutex.Release();
}

void CThreadPool::Shutdown()
{
	//_mutex.Acquire();
	size_t tcount = m_activeThreads.size() + m_freeThreads.size();	// exit all
	Log.Debug("ThreadPool", "Shutting down %u threads.", tcount);
	KillFreeThreads((uint32)m_freeThreads.size());
	_threadsToExit += (uint32)m_activeThreads.size();

	for(ThreadSet::iterator itr = m_activeThreads.begin(); itr != m_activeThreads.end(); ++itr)
	{
		if((*itr)->ExecutionTarget)
			(*itr)->ExecutionTarget->OnShutdown();
	}
	//_mutex.Release();

#if PLATFORM == PLATFORM_WINDOWS
	uint32 listcount = 0;
	for(;;)
	{
		//_mutex.Acquire();
		if(listcount > 24)
		{
			listcount = 0;
			Log.Debug("ThreadPool", "Listing threads");
			if(m_activeThreads.size())
			{
				for(ThreadSet::iterator itr = m_activeThreads.begin(); itr != m_activeThreads.end(); ++itr)
				{
					if((*itr)->ExecutionTarget)
						Log.Debug("ActiveThreadPool", "%u(%s) thread...", (*itr)->ControlInterface.GetId(), (*itr)->threadinfo.szName);
				}
			}

			if(m_freeThreads.size())
			{
				for(ThreadSet::iterator itr = m_freeThreads.begin(); itr != m_freeThreads.end(); ++itr)
				{
					if((*itr)->ExecutionTarget)
						Log.Debug("FreeThreadPool", "%u(%s) thread...", (*itr)->ControlInterface.GetId(), (*itr)->threadinfo.szName);
				}
			}
		}
#else
	for(;;)
	{
		//_mutex.Acquire();
#endif
		if(m_activeThreads.size() || m_freeThreads.size())
		{
			Log.Debug("ThreadPool", "%u active threads, %u free threads remaining...", m_activeThreads.size(), m_freeThreads.size());
			//_mutex.Release();

#if PLATFORM == PLATFORM_WINDOWS
			listcount++;
#endif

			Sleep(1000);
			//continue; leállást gátolja egyenlőre még nem tudom a hiba okát
		}

		m_activeThreads.clear();
		m_freeThreads.clear();
		//_mutex.Release();

		break;
	}
}

/* this is the only platform-specific code. neat, huh! */
#if PLATFORM == PLATFORM_WINDOWS

bool RunThread(ThreadContext * target)
{
	bool res = false;
	THREAD_TRY_EXECUTION
	{
		res = target->Run();
	}
	THREAD_HANDLE_CRASH
	return res;
}

static unsigned long WINAPI thread_proc(void* param)
{
	Thread * t = (Thread*)param;
	t->SetupMutex.Acquire();
	uint32 tid = t->ControlInterface.GetId();
	bool ht = (t->ExecutionTarget != NULL);
	t->SetupMutex.Release();
	//Log.Debug("ThreadPool", "Thread %u started.", t->ControlInterface.GetId());

	for(;;)
	{
		if(t->ExecutionTarget != NULL)
		{
			if(RunThread(t->ExecutionTarget))
				delete t->ExecutionTarget;

			t->ExecutionTarget = NULL;
		}

		if(!ThreadPool.ThreadExit(t))
		{
			Log.Debug("ThreadPool", "Thread %u exiting.", tid);
			break;
		}
		else
		{
			if(ht)
				Log.Debug("ThreadPool", "Thread %u waiting for a new task.", tid);
			// enter "suspended" state. when we return, the threadpool will either tell us to fuk off, or to execute a new task.
			t->ControlInterface.Suspend();
			// after resuming, this is where we will end up. start the loop again, check for tasks, then go back to the threadpool.
		}
	}

	// at this point the t pointer has already been freed, so we can just cleanly exit.
	//ExitThread(0);

	// not reached
	return 0;
}

Thread * CThreadPool::StartThread(ThreadContext * ExecutionTarget)
{
	SetThreadName("Thread Starter");
	Thread * t = new Thread;
	t->DeleteAfterExit = false;
	t->ExecutionTarget = ExecutionTarget;
	t->SetupMutex.Acquire();
	t->ControlInterface.Setup(CreateThread(NULL, 0, &thread_proc, (LPVOID)t, 0, (LPDWORD)&t->ControlInterface.thread_id));
	t->SetupMutex.Release();
	return t;
}

#else

static void * thread_proc(void * param)
{
	Thread * t = (Thread*)param;
	t->SetupMutex.Acquire();
	Log.Debug("ThreadPool", "Thread %u started.", t->ControlInterface.GetId());
	t->SetupMutex.Release();

	for(;;)
	{
		if(t->ExecutionTarget != NULL)
		{
			if(t->ExecutionTarget->Run())
				delete t->ExecutionTarget;

			t->ExecutionTarget = NULL;
		}

		if(!ThreadPool.ThreadExit(t))
			break;
		else
		{
			// enter "suspended" state. when we return, the threadpool will either tell us to fuk off, or to execute a new task.
			t->ControlInterface.Suspend();
			// after resuming, this is where we will end up. start the loop again, check for tasks, then go back to the threadpool.
		}
	}

	//pthread_exit(0);
	return NULL;
}

Thread * CThreadPool::StartThread(ThreadContext * ExecutionTarget)
{
	pthread_t target;
	Thread * t = new Thread;
	t->ExecutionTarget = ExecutionTarget;
	t->DeleteAfterExit = false;

	// lock the main mutex, to make sure id generation doesn't get messed up
	//_mutex.Acquire();
	t->SetupMutex.Acquire();
	pthread_create(&target, NULL, &thread_proc, (void*)t);
	t->ControlInterface.Setup(target);
	t->SetupMutex.Release();
	//_mutex.Release();
	return t;
}

#endif
