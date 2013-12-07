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

#ifndef __SAVESYSTEM_H__
#define __SAVESYSTEM_H__

#include "Cartridge.h"
#include "Singleton.h"

#define SAVESYSTEM_SLOT_UNUSED		(255)

namespace Seed {

/// Save System
class SEED_CORE_API SaveSystem
{
	SEED_DECLARE_SINGLETON(SaveSystem)
	SEED_DISABLE_COPY(SaveSystem)

	public:
		/// Save Header
		struct SEED_CORE_API sSaveInfo
		{
			u32 iID;
			u32 iLastSlot;
			u32 iCRC;
		};

	public:
		eCartridgeError Initialize(eCartridgeSize type);
		eCartridgeError Prepare(u32 myId, void *slotBlankData, u32 slotDataSize, void *sharedBlankData = nullptr, u32 sharedSize = 0);
		eCartridgeError Check();
		eCartridgeError FormatCard(void *slotBlankData, void *sharedBlankData = nullptr);
		eCartridgeError FormatSlot(u8 slot, void *slotBankData);

		eCartridgeError Load(u32 slot, void *loadAddress, void *sharedData = nullptr);
		eCartridgeError Save(u32 slot, void *data, void *sharedData = nullptr);
		eCartridgeError GetLastUsedSlot(u32 *lastSlot);

		eCartridgeError ReadSharedData(void *sharedData);
		eCartridgeError WriteSharedData(void *sharedData);

		void SetTotalSlots(u32 s);
		u32 GetTotalSlots() const;

		bool IsSaving() const;

	private:
		eCartridgeError GetNewSlotBuffer(u32 slot, u8 *slotBuffer);
		eCartridgeError GetSlotBuffer(u32 slot, u8 *slotBuffer);
		eCartridgeError SetSlotBuffer(u32 slot, u8 slotBuffer);
		eCartridgeError ReadHeaderData(sSaveInfo *header);
		eCartridgeError WriteHeaderData(sSaveInfo *header);
		eCartridgeError FormatShared(void *sharedBlankData);

		void Wait();
		void PrepareFilesystem();

	private:
		u32 iTotalSlots;
		u32 iNumSlots;
		u32 iSlotDataSize;
		u32 iSharedDataSize;
		u32 iID;
		eCartridgeSize cardType;

		static bool bIsSaving;
		bool bInitialized : 1;

		char *pcSaveGameFolder[SEED_MAX_FOLDER_SIZE];
};

#define pSaveSystem SaveSystem::GetInstance()

} // namespace

#endif // __SAVESYSTEM_H__
