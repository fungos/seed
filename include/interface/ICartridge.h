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

#ifndef __ICARTRIDGE_H__
#define __ICARTRIDGE_H__

#include "IManager.h"
#include "../Enum.h"

namespace Seed {

/// Cartridge Interface
/**
Manager for cartridge/save data, it can be a real cartridge like Nintendo DS builtin EPROM
and/or even a file in a PC implementation.
*/
class SEED_CORE_API ICartridge : public IManager
{
	SEED_DISABLE_COPY(ICartridge)

	public:
		ICartridge();
		virtual ~ICartridge();

		/// Need be called before initialization to set the type of cartridge to use.
		/**
		Pre-configure the cartridge module for the type of cartridge to be used.

		\param size eCartridgeSize type.
		\return true if cartridge is ready, false otherwise
		*/
		virtual bool Prepare(eCartridgeSize size);

		/// Read a cartridge data to an allocated buffer
		/**
		Read len bytes of the data begining in src to a dest buffer.

		\param src The offset from where start reading saved data.
		\param dest Destination buffer where the data will be written.
		\param len Total length of the data to be read.
		\return true if read succeeds, false otherwise
		*/
		virtual bool Read(u32 src, void *dest, u32 len);

		/// Write data from a buffer to a cartridge
		/**
		Read len bytes of the data begining in src to a dest buffer.

		\param src The offset from where start reading saved data.
		\param dest Destination buffer where the data will be written.
		\param len Total length of the data to be read.
		\return true if write succeeds, false otherwise
		*/
		virtual bool Write(u32 dest, const void *src, u32 len);

		/// Get the cartridge size
		/**
		\return Total size in bytes of the cartridge
		*/
		virtual u32	GetSize() const;


		/// Retrieves the last error ocurred
		/**
		Returns the last error ocurred if any.

		\return the last error ocurred.
		*/
		virtual eCartridgeError GetLastError() const;

		virtual u32 GetOffsetForSlot(u32 headerSize, u32 sharedDataSize, u32 slotDataSize, u32 slot) const;
		virtual u32 GetOffsetForSlotCRC(u32 headerSize, u32 sharedDataSize, u32 slotDataSize, u32 slot) const;
		virtual u32 GetRequiredSize(u32 headerSize, u32 sharedDataSize, u32 slotDataSize, u32 num_slots) const;

	protected:
		eCartridgeError nLastError;
		u32 iSize;
};

} // namespace

#endif // __ICARTRIDGE_H__
