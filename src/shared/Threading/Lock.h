/*
 * This file is part of Schumix.
 * 
 * Cross-platform Mutex implementation.
 * Copyright (C) 2010 Twl
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

#ifndef _LOCK_H
#define _LOCK_H

class Lock
{
public:
	Lock(Mutex& mutex) : m_mutex(mutex)
	{
		this->m_mutex.Acquire();
	}

	~Lock()
	{
		this->m_mutex.Release();
	}

private:
	Mutex& m_mutex;
};

#endif