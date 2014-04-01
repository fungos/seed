/*
* Copyright (c) 2012, Seed Developers
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
* 4. A credit line is required in the game credits, as the following line:
*    Powered by Seed Framework
* 5. A Seed Framework logo should be displayed within a splash screen, or in
*    the case where the game haven't or cannot have a splash, a logo should be
*    displayed somewhere in the game (eg. menu, credits, other place).
*
* THIS SOFTWARE IS PROVIDED BY SEED DEVELOPERS ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL SEED DEVELOPERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Checksum.h"

namespace Seed {

Checksum *pChecksum = nullptr;
SEED_SINGLETON_DEFINE(Checksum)

Checksum::Checksum()
	: iPolynomial1(0)
	, iPolynomial2(0)
{
	this->SetPolynomial1(0xedb88320);
	this->SetPolynomial2(0x12477cdf);
}

Checksum::~Checksum()
{
}

void Checksum::SetPolynomial1(unsigned int polynomial)
{
	iPolynomial1 = polynomial;

	for (int n = 0; n < 256; n++)
	{
		unsigned long c = (unsigned long)n;

		for (int b = 0; b < 8; b++)
			c = c & 1 ? iPolynomial1 ^ (c >> 1) : c >> 1;

		aiCRCTable1[n] = (int)c;
	}
}

void Checksum::SetPolynomial2(unsigned int polynomial)
{
	iPolynomial2 = polynomial;

	for ( int n = 0; n < 256; n++)
	{
		unsigned long c = (unsigned long)n;

		for (int b = 0; b < 8; b++)
			c = c & 1 ? iPolynomial2 ^ (c >> 1) : c >> 1;

		aiCRCTable2[n] = (int)c;
	}
}

u64 Checksum::Calculate(const char *name)
{
	u32 crc1 = 0;
	u32 crc2 = 0;

	while (*name != '\0')
	{
		u8 chr = (u8)*name++;

		if (chr >= 'a' && chr <= 'z')
			chr = (u8)((chr - (u8)'a') + (u8)'A');
		else if (chr == '\\')
			chr = (u8)'/';

		crc1 = aiCRCTable1[(crc1 ^ chr) & 0xff] ^ (crc1 >> 8);
		crc2 = aiCRCTable2[(crc2 ^ chr) & 0xff] ^ (crc2 >> 8);
	}

	return ((static_cast<u64>(crc1) << 32) | static_cast<u64>(crc2));
}

u64 Checksum::Calculate(const char *data, u32 len)
{
	u32 crc1 = 0;
	u32 crc2 = 0;

	for (u32 i = 0; i < len; i++)
	{
		u8 chr = (u8)*data++;

		crc1 = aiCRCTable1[(crc1 ^ chr) & 0xff] ^ (crc1 >> 8);
		crc2 = aiCRCTable2[(crc2 ^ chr) & 0xff] ^ (crc2 >> 8);
	}

	return ((static_cast<u64>(crc1) << 32) | static_cast<u64>(crc2));
}

} // namespace
