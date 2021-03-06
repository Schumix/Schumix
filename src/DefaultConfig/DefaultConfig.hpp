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

#ifndef _SCHUMIX_DEFAULT_CONFIG_HPP
#define _SCHUMIX_DEFAULT_CONFIG_HPP

class DefaultConfig
{
public:
	DefaultConfig(string conf);
	~DefaultConfig();

protected:
	void IrcConfig();
	void MysqlConfig();

private:
	FILE* ConfigFajl;
	static int writer(char* data, size_t size, size_t nmemb, string *buffer);
};

#endif
