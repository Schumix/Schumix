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

#ifndef _THREADING_STARTER_HPP
#define _THREADING_STARTER_HPP

class ThreadContext
{
public:
	ThreadContext() { m_threadRunning = true; }
	virtual ~ThreadContext() {}
	virtual bool Run() = 0;

#ifdef WIN32
	HANDLE THREAD_HANDLE;
#else
	pthread_t THREAD_HANDLE;
#endif

	inline void Terminate() { m_threadRunning = false; }
	virtual void OnShutdown() { Terminate(); }

protected:
	bool m_threadRunning;
};

#endif

