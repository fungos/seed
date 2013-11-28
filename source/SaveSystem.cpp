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

#include "Defines.h"
#include "SaveSystem.h"
#include "Log.h"
#include "Checksum.h"
#include "System.h"
#include "FileSystem.h"
#include "Memory.h"

#define TAG "[SaveSystem] "

namespace Seed {

SEED_SINGLETON_DEFINE(SaveSystem)
bool SaveSystem::bIsSaving = false;

SaveSystem::SaveSystem()
	: iTotalSlots(PLATFORM_SAVESYSTEM_SLOTS_MAX)
	, iNumSlots(PLATFORM_SAVESYSTEM_SLOTS_MAX)
	, iSlotDataSize(0)
	, iSharedDataSize(0)
	, iID(0)
	, cardType(eCartridgeSize::Unlimited)
	, bInitialized(false)
	, pcSaveGameFolder()
{
}

SaveSystem::~SaveSystem()
{
}

eCartridgeError SaveSystem::Initialize(eCartridgeSize type)
{
	if (!bInitialized)
	{
		Log(TAG "Initializing...");
		cardType = type;

		this->PrepareFilesystem();

		if (!pCartridge->Prepare(type))
		{
			Log(TAG "Failed to initialize cartridge.");
			bIsSaving = false;
			return pCartridge->GetLastError();
		}

		bInitialized = true;
		bIsSaving = false;
		Log(TAG "Initialization completed.");
	}
	else
	{
		Log(TAG "WARNING: SaveSystem Initilaize being called more than once. FIX IT.");
	}

	return eCartridgeError::None;
}

eCartridgeError SaveSystem::Check()
{
	sSaveInfo header;
	return this->ReadHeaderData(&header);
}

void SaveSystem::SetTotalSlots(u32 totalSlots)
{
	iTotalSlots = totalSlots;
}

u32 SaveSystem::GetTotalSlots() const
{
	return iTotalSlots;
}

eCartridgeError SaveSystem::Prepare(u32 myId, void *slotBlankData, u32 slotDataSize, void *sharedBlankData, u32 sharedDataSize)
{
	eCartridgeError error;

	if (!bInitialized)
	{
		Log(TAG "Save System not initialized!");
		return eCartridgeError::NotInitialized;
	}

	bIsSaving = true;
	Log(TAG "Preparing Save System");

	iSlotDataSize	= slotDataSize;
	iSharedDataSize	= sharedDataSize;
	iID				= myId;

	u32 requiredSize = pCartridge->GetRequiredSize(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, (iTotalSlots + 1));
	if (pCartridge->GetSize() < requiredSize)
	{
		Log(TAG "Cartridge %d bytes of space, slot size and header require %d bytes.", pCartridge->GetSize(), requiredSize);
		bIsSaving = false;
		return eCartridgeError::InvalidArgument;
	}

	// Test access to the card
	error = this->Check();

	if (error != eCartridgeError::None)
	{
		bIsSaving = false;
		return error;
	}

	eCartridgeError ret = eCartridgeError::None;

	// Check all shared data state
	if (sharedBlankData != nullptr && iSharedDataSize > 0)
	{
		void *sharedTestMemory = sdAlloc(iSharedDataSize);

		error = this->ReadSharedData(sharedTestMemory);
		if (error != eCartridgeError::None)
		{
			ret = error;

			if (error == eCartridgeError::DataCorrupt)
			{
				Log(TAG "Shared data CRC check failed, formatting...");
				this->FormatShared(sharedBlankData);
			}
		}

		sdFree(sharedTestMemory);
	}

	if (error == eCartridgeError::None)
	{
		// Check all slots state
		void *slotTestMemory = sdAlloc(iSlotDataSize);
		for (u8 i = 0; i < iTotalSlots; i ++)
		{
			error = this->Load(i, slotTestMemory);

			if (error != eCartridgeError::None)
			{
				ret = error;

				if (error == eCartridgeError::DataCorrupt)
				{
					Log(TAG "Slot %d CRC check failed, formatting...", i);
					this->FormatSlot(i, slotBlankData);
				}
			}
		}

		sdFree(slotTestMemory);
	}

	bIsSaving = false;
	Log(TAG "Save System Preparation Complete");

	return ret;
}

eCartridgeError SaveSystem::FormatCard(void *slotBlankData, void *sharedBlankData)
{
	eCartridgeError error;
	sSaveInfo header;
	bIsSaving = true;

	if (!bInitialized)
	{
		bIsSaving = false;
		return eCartridgeError::NotInitialized;
	}

	header.iID = iID;
	header.iLastSlot = 0;
	error = this->WriteHeaderData(&header);

	if (error != eCartridgeError::None)
	{
		bIsSaving = false;
		return error;
	}

	if (sharedBlankData)
	{
		error = this->FormatShared(sharedBlankData);
		if (error != eCartridgeError::None)
		{
			bIsSaving = false;
			return error;
		}
	}

	for (u8 i = 0; i < iTotalSlots; i++)
	{
		error = this->FormatSlot(i, slotBlankData);
		if (error != eCartridgeError::None)
		{
			bIsSaving = false;
			return error;
		}
	}

	bIsSaving = false;
	return eCartridgeError::None;
}

eCartridgeError SaveSystem::FormatShared(void *sharedBlankData)
{
	bIsSaving = true;

	if (!bInitialized)
	{
		bIsSaving = false;
		return eCartridgeError::NotInitialized;
	}

	if (iSharedDataSize == 0 || sharedBlankData == nullptr)
	{
		bIsSaving = false;
		return eCartridgeError::None;
	}

	u32 sharedBlankDataCRC	= (u32)pChecksum->Calculate((const char *)sharedBlankData, iSharedDataSize);
	u32 offset 				= sizeof(sSaveInfo);
	u32 offsetCrc 			= (u32)sizeof(sSaveInfo) + iSharedDataSize;
	if (!pCartridge->Write(offset, sharedBlankData, iSharedDataSize))
	{
		bIsSaving = false;
		return pCartridge->GetLastError();
	}

	if (!pCartridge->Write(offsetCrc, &sharedBlankDataCRC, sizeof(u32)))
	{
		bIsSaving = false;
		return pCartridge->GetLastError();
	}
	this->Wait();

	bIsSaving = false;
	return eCartridgeError::None;
}

eCartridgeError SaveSystem::FormatSlot(u8 slot, void *slotBlankData)
{
	bIsSaving = true;

	if (!bInitialized)
	{
		bIsSaving = false;
		return eCartridgeError::NotInitialized;
	}

	if (slot > iTotalSlots)
	{
		Log(TAG "Maximum of %d save game slots allowed.", iTotalSlots);
		bIsSaving = false;
		return eCartridgeError::InvalidArgument;
	}

	u32 blankDataCRC = (u32)pChecksum->Calculate((const char *)slotBlankData, iSlotDataSize);
	u32 offset 		= pCartridge->GetOffsetForSlot(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, slot);
	u32 offsetCrc 	= pCartridge->GetOffsetForSlotCRC(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, slot);

	if (!pCartridge->Write(offset, slotBlankData, iSlotDataSize))
	{
		bIsSaving = false;
		return pCartridge->GetLastError();
	}

	if (!pCartridge->Write(offsetCrc, &blankDataCRC, sizeof(u32)))
	{
		bIsSaving = false;
		return pCartridge->GetLastError();
	}
	this->Wait();

	bIsSaving = false;
	return eCartridgeError::None;
}

eCartridgeError SaveSystem::Load(u32 slot, void *loadAddress, void *sharedData)
{
	u32 slotCRC;

	if (!bInitialized)
		return eCartridgeError::NotInitialized;

	if (slot > iTotalSlots)
	{
		Log(TAG "Maximum of %d save game slots allowed.", iTotalSlots);
		return eCartridgeError::InvalidArgument;
	}

	u32 offset 		= pCartridge->GetOffsetForSlot(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, slot);
	u32 offsetCrc 	= pCartridge->GetOffsetForSlotCRC(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, slot);

	if (!pCartridge->Read(offset, loadAddress, iSlotDataSize))
		return pCartridge->GetLastError();

	if (!pCartridge->Read(offsetCrc, &slotCRC, sizeof(u32)))
		return pCartridge->GetLastError();

	eCartridgeError error = eCartridgeError::None;
	if (slotCRC != (u32)pChecksum->Calculate((const char *)loadAddress, iSlotDataSize))
	{
		Log(TAG "Slot CRC Failed.");
		error = eCartridgeError::DataCorrupt;
	}

	if (error == eCartridgeError::None)
		 error = this->ReadSharedData(sharedData);

	this->Wait();

	return error;
}

eCartridgeError SaveSystem::GetLastUsedSlot(u32 *lastSlot)
{
	eCartridgeError error;
	sSaveInfo header;

	if (!bInitialized)
		return eCartridgeError::NotInitialized;

	error = this->ReadHeaderData(&header);

	if (error == eCartridgeError::None)
	   *lastSlot = header.iLastSlot;

	return error;
}

eCartridgeError SaveSystem::Save(u32 slot, void *slotData, void *sharedData)
{
	bIsSaving = true;

	if (!bInitialized)
	{
		bIsSaving = false;
		return eCartridgeError::NotInitialized;
	}

	if (slot > iTotalSlots)
	{
		Log(TAG "Maximum of %d save game slots allowed.", iTotalSlots);
		bIsSaving = false;
		return eCartridgeError::InvalidArgument;
	}

	sSaveInfo header;
	header.iID 			= iID;
	header.iLastSlot 	= slot;
	eCartridgeError error = this->WriteHeaderData(&header);
	if (error != eCartridgeError::None)
	{
		Log(TAG "Error updating header information.");
		bIsSaving = false;
		return error;
	}

	u32 offset 		= pCartridge->GetOffsetForSlot(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, slot);
	u32 offsetCrc 	= pCartridge->GetOffsetForSlotCRC(sizeof(sSaveInfo), iSharedDataSize, iSlotDataSize, slot);

	if (!pCartridge->Write(offset, slotData, iSlotDataSize))
	{
		bIsSaving = false;
		return pCartridge->GetLastError();
	}

	u32 slotCRC = (u32)pChecksum->Calculate((const char *)slotData, iSlotDataSize);
	if (!pCartridge->Write(offsetCrc, &slotCRC, sizeof(u32)))
	{
		bIsSaving = false;
		return pCartridge->GetLastError();
	}

	error = this->WriteSharedData(sharedData);

	this->Wait();
	bIsSaving = false;

	return error;
}

eCartridgeError SaveSystem::ReadHeaderData(sSaveInfo *header)
{
	if (!bInitialized)
		return eCartridgeError::NotInitialized;

	if (!pCartridge->Read(0, header, sizeof(sSaveInfo)))
		return pCartridge->GetLastError();

	if (header->iID == iID)
	{
		if (header->iCRC == (u32)pChecksum->Calculate((const char *)header, sizeof(sSaveInfo) - sizeof(u32)))
			return eCartridgeError::None;

		return eCartridgeError::DataCorrupt;
	}

	return eCartridgeError::NotFormatted;
}

eCartridgeError SaveSystem::WriteHeaderData(sSaveInfo *header)
{
	if (!bInitialized)
		return eCartridgeError::NotInitialized;

	header->iCRC = (u32)pChecksum->Calculate((const char *)header, sizeof(sSaveInfo) - sizeof(u32));

	if (!pCartridge->Write(0, header, sizeof(sSaveInfo)))
		return pCartridge->GetLastError();

	return eCartridgeError::None;
}

eCartridgeError SaveSystem::ReadSharedData(void *sharedBlankData)
{
	if (!bInitialized)
		return eCartridgeError::NotInitialized;

	if (iSharedDataSize == 0 || sharedBlankData == nullptr)
		return eCartridgeError::None;

	if (!pCartridge->Read(sizeof(sSaveInfo), sharedBlankData, iSharedDataSize))
		return pCartridge->GetLastError();

	u32 crc = 0;
	if (!pCartridge->Read((u32)sizeof(sSaveInfo) + iSharedDataSize, &crc, sizeof(u32)))
		return pCartridge->GetLastError();

	if (crc == (u32)pChecksum->Calculate((const char *)sharedBlankData, iSharedDataSize))
		return eCartridgeError::None;

	return eCartridgeError::DataCorrupt;
}

eCartridgeError SaveSystem::WriteSharedData(void *sharedBlankData)
{
	if (!bInitialized)
		return eCartridgeError::NotInitialized;

	if (iSharedDataSize == 0 || sharedBlankData == nullptr)
		return eCartridgeError::None;

	u32 crc = (u32)pChecksum->Calculate((const char *)sharedBlankData, iSharedDataSize);

	if (!pCartridge->Write(sizeof(sSaveInfo), sharedBlankData, iSharedDataSize))
		return pCartridge->GetLastError();

	if (!pCartridge->Write((u32)sizeof(sSaveInfo) + iSharedDataSize, &crc, sizeof(u32)))
		return pCartridge->GetLastError();

	return eCartridgeError::None;
}

void SaveSystem::Wait()
{
	/*
	#if !defined(DEBUG_GAMESTATE_NODELAY)
	u8 i;
	for (i = 0; i < 24; i ++)
		pSystem->WaitForRetrace(ISystem::RATE_60FPS);
	#endif // DEBUG_GAMESTATE_NODELAY
	*/
}

bool SaveSystem::IsSaving() const
{
	return bIsSaving;
}

/* Temporary hack so we can move this out from user code */
void SaveSystem::PrepareFilesystem()
{
}

} // namespace
