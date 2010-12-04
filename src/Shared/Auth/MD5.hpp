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

#ifndef _SCHUMIX_MD5_HPP
#define _SCHUMIX_MD5_HPP

#include <openssl/md5.h>

class MD5Hash
{
public:
	MD5Hash();
	~MD5Hash();

	void UpdateData(const uint8 *dta, int len);
	void UpdateData(const std::string &str);

	void Initialize();
	void Finalize();

	uint8 *GetDigest(void) { return mDigest; };
	int GetLength(void) { return MD5_DIGEST_LENGTH; };

private:
	MD5_CTX mC;
	uint8 mDigest[MD5_DIGEST_LENGTH];
};

#endif
