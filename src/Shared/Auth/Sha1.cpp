/*
 * This file is part of Schumix.
 * 
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

#include "../../StdAfx.h"

Sha1Hash::Sha1Hash()
{
	SHA1_Init(&mC);
}

Sha1Hash::~Sha1Hash()
{

}

void Sha1Hash::UpdateData(const uint8* dta, int len)
{
	SHA1_Update(&mC, dta, len);
}

void Sha1Hash::UpdateData(const std::string &str)
{
	UpdateData((uint8*)str.c_str(), cast_int(str.length()));
}

void Sha1Hash::Initialize()
{
	SHA1_Init(&mC);
}

void Sha1Hash::Finalize(void)
{
	SHA1_Final(mDigest, &mC);
}
