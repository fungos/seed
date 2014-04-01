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

#include "interface/ICartridge.h"
#include "Log.h"

namespace Seed {

ICartridge::ICartridge()
	: nLastError(eCartridgeError::None)
	, iSize(0)
{
}

ICartridge::~ICartridge()
{
}

eCartridgeError ICartridge::GetLastError() const
{
	return nLastError;
}

bool ICartridge::Write(u32 dest, const void *src, u32 len)
{
	UNUSED(dest);
	UNUSED(src);
	UNUSED(len);
	SEED_ABSTRACT_METHOD;
	return false;
}

u32 ICartridge::GetSize() const
{
	return iSize;
}

bool ICartridge::Read(u32 src, void *dest, u32 len)
{
	UNUSED(dest);
	UNUSED(src);
	UNUSED(len);
	SEED_ABSTRACT_METHOD;
	return false;
}

bool ICartridge::Prepare(eCartridgeSize size)
{
	UNUSED(size);
	SEED_ABSTRACT_METHOD;
	return false;
}

u32 ICartridge::GetOffsetForSlot(u32 headerSize, u32 sharedSize, u32 dataSize, u32 slot) const
{
	return (u32)(headerSize + sharedSize + ((sharedSize > 0) ? sizeof(u32) : 0) + (slot * (dataSize + sizeof(u32))));
}

u32 ICartridge::GetOffsetForSlotCRC(u32 headerSize, u32 sharedSize, u32 dataSize, u32 slot) const
{
	return (u32)(headerSize + sharedSize + ((sharedSize > 0) ? sizeof(u32) : 0) + (slot * (dataSize + sizeof(u32))) + dataSize);
}

u32 ICartridge::GetRequiredSize(u32 headerSize, u32 sharedSize, u32 dataSize, u32 num_slots) const
{
	return (u32)(headerSize + sharedSize + ((sharedSize > 0) ? sizeof(u32) : 0) + (num_slots * (dataSize + sizeof(u32))));
}

} // namespace
