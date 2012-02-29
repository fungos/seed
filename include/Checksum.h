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

#ifndef __CHECKSUM_H__
#define __CHECKSUM_H__

#include "Defines.h"
#include "Singleton.h"

namespace Seed {

/// Helper class to calculate CRC checksums
class SEED_CORE_API Checksum
{
	SEED_SINGLETON_DECLARE(Checksum)
	public:
		/// Sets the first polynomial to use when generating CRC tables
		void 	SetPolynomial1(unsigned int polynomial);

		/// Sets the seconds polynomial to use when generatinc CRC tables
		void	SetPolynomial2(unsigned int polynomial);

		/// Calculate a filename CRC (will ignore case and change '\' to '/'
		/**
		\param name A file name
		*/
		u64		Calculate(const char *name);

		/// Calculate CRC from an arbitrary data until len size
		/**
		\param data Pointer to the data to which generate a CRC
		\param len Size of the data
		*/
		u64		Calculate(const char *data, u32 len);

	private:
		SEED_DISABLE_COPY(Checksum);

	private:
		unsigned int	iPolynomial1;
		unsigned int	iPolynomial2;

		unsigned int	aiCRCTable1[256];
		unsigned int	aiCRCTable2[256];

};

//#define pChecksum Checksum::GetInstance()
extern Checksum *pChecksum;

} // namespace

#endif // __CHECKSUM_H__
