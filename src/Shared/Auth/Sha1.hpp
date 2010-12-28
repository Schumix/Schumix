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

#ifndef _SCHUMIX_SHA1_HPP
#define _SCHUMIX_SHA1_HPP

#include <openssl/sha.h>

class Sha1Hash
{
public:
	Sha1Hash();
	~Sha1Hash();

	void UpdateData(const string &str);
	void UpdateData(const uint8* dta, int len);

	void Initialize();
	void Finalize();

	uint8* GetDigest(void) { return mDigest; };
	int GetLength(void) { return SHA_DIGEST_LENGTH; };

private:
	SHA_CTX mC;
	uint8 mDigest[SHA_DIGEST_LENGTH];
};

#endif
