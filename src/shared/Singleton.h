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

#ifndef _SINGLETON_H
#define _SINGLETON_H

template <class type> class Singleton
{
public:
	// Constructor
	Singleton()
	{
		// If you hit this assert, this singleton already exists -- you can't create another one!
		assert(mSingleton == NULL);
		mSingleton = static_cast<type*>(this);
	}
	// Destructor
	virtual ~Singleton()
	{
		mSingleton = NULL;
	}

	inline static type& getSingleton() { return *mSingleton; }
	inline static type* getSingletonPtr() { return mSingleton; }

protected:
	// Singleton pointer, must be set to 0 prior to creating the object
	static type* mSingleton;
};

// Should be placed in the appropriate .cpp file somewhere
#define initialiseSingleton(type) template <> type* Singleton <type> :: mSingleton = NULL

// To be used as a replacement for initialiseSingleton()
// Creates a file-scoped Singleton object, to be retrieved with getSingleton
#define createFileSingleton(type) initialiseSingleton(type); type the##type

#endif
