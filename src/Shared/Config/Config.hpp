/*
 * This file is part of Schumix.
 * 
 * Copyright (C) 2008 Valroft <http://www.aspiredev.org/>
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

#ifndef _SCHUMIX_CONFIG_HPP
#define _SCHUMIX_CONFIG_HPP

struct ConfigSetting
{
	string AsString;
	bool AsBool;
	int AsInt;
	float AsFloat;
};

typedef map<uint32, ConfigSetting> ConfigBlock;

class ConfigFile
{
public:
	ConfigFile();
	~ConfigFile();

	bool SetSource(const char* file, bool ignorecase = true);
	ConfigSetting* GetSetting(const char* Block, const char* Setting);

	bool GetString(const char* block, const char* name, string* value);
	string GetStringDefault(const char* block, const char* name, const char* def);
	string GetStringVA(const char* block, const char* def, const char* name, ...);
	bool GetString(const char* block, char* buffer, const char* name, const char* def, uint32 len);

	bool GetBool(const char* block, const char* name, bool *value);
	bool GetBoolDefault(const char* block, const char* name, const bool def);

	bool GetInt(const char* block, const char* name, int* value);
	int GetIntDefault(const char* block, const char* name, const int def);
	int GetIntVA(const char* block, int def, const char* name, ...);

	bool GetFloat(const char* block, const char* name, float* value);
	float GetFloatDefault(const char* block, const char* name, const float def);
	float GetFloatVA(const char* block, float def, const char* name, ...);

private:
	map<uint32, ConfigBlock> m_settings;
};


class ConfigMgr
{
public:
	ConfigFile MainConfig;
};

extern ConfigMgr Config;

#endif
