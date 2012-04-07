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

#include "Cartridge.h"
#include "LeakReport.h"

#if defined(BUILD_PC)

#include "Log.h"
#include "FileSystem.h"

#include <stdio.h>

#define TAG "[Cartridge] "

namespace Seed { namespace PC {

SEED_SINGLETON_DEFINE(Cartridge)

Cartridge::Cartridge()
	: iType(0)
	, iCurrentSlot(-1)
	, pData(NULL)
{
}

Cartridge::~Cartridge()
{
}

bool Cartridge::Initialize()
{
	Log(TAG "Initializing...");
	this->Reset();
	Log(TAG "Initialization completed.");

	return true;
}

bool Cartridge::Reset()
{
	Free(this->pData);

	iType = 0;
	iSize = 0;
	memset(strPath, '\0', PC_MAX_PATH);

	return true;
}

bool Cartridge::Shutdown()
{
	this->Reset();
	Log(TAG "Terminating...");
	Log(TAG "Terminated.");
	return true;
}

bool Cartridge::Prepare(eCartridgeSize size)
{
	iType = size;
	iSize = this->GetCardType(size);

	const char *p = pFileSystem->GetWriteableDirectory();
	SEED_ASSERT_MSG(p!=NULL, "You must set a WriteableDirectory!");

	memset(strPath, '\0', sizeof(strPath));

	strncpy(strPath, p, PC_MAX_PATH);
	strncat(strPath, PATH_SEPARATOR, PC_MAX_PATH - 1);
	strncat(strPath, CARTRIDGE_FILENAME, PC_MAX_PATH - strlen(strPath) - 1);

	this->pData = static_cast<u8 *>(Alloc(iSize));
	memset(this->pData, 0, iSize);

	if (!this->Verify(strPath, iSize))
	{
		if (!this->CreateSaveFile())
		{
			Log(TAG "ERROR: trying to create save file!");
			return false;
		}
	}

	return true;
}

bool Cartridge::Read(u32 src, void *dest, u32 len)
{
	if (src >= iSize)
	{
		Log(TAG "%s:%d: source address error.", __FILE__, __LINE__);
		return false;
	}

	if (len > iSize)
	{
		Log(TAG "%s:%d: length error.", __FILE__, __LINE__);
		return false;
	}

	if (src + len > iSize)
	{
		Log(TAG "%s:%d: source address error.", __FILE__, __LINE__);
		return false;
	}

	s32 iResult = 0;
	FILE *pFp = fopen(strPath, "rb");
	if (!pFp)
	{
		Info(TAG "fopen %s failed", strPath);
		eLastError = Seed::ErrorAccessDenied;
		return false;
	}

	iResult = fseek(pFp, 0, SEEK_SET);
	if (iResult)
	{
		Log(TAG "fseek failed: %d", iResult);
		fclose(pFp);
		eLastError = Seed::ErrorAccessDenied;
		return false;
	}

	u32 iAmountRead = (u32)fread(this->pData, 1, iSize, pFp);
	if (iAmountRead != iSize)
	{
		Log(TAG "fread: could not load save data!");
		fclose(pFp);
		eLastError = Seed::ErrorDeviceFull;
		return false;
	}

	iResult = fseek(pFp, static_cast<s32>(src), SEEK_SET);
	if (iResult)
	{
		Log(TAG "fseek failed: %d", iResult);
		fclose(pFp);
		eLastError = Seed::ErrorAccessDenied;
		return false;
	}

	iAmountRead = (u32)fread(dest, 1, len, pFp);
	fclose(pFp);

	if (iAmountRead != len)
	{
		Log(TAG "fread: Could not read all the data requested: AmountRead = %d Requested = %d\n", iAmountRead, len);
		eLastError = Seed::ErrorDeviceFull;
		return false;
	}

	return true;
}

bool Cartridge::Write(u32 dest, const void *src, u32 len)
{
	if (dest >= iSize)
	{
		Log(TAG "%s:%d: dest address error.", __FILE__, __LINE__);
		return false;
	}

	if (len > iSize)
	{
		Log(TAG "%s:%d: length error.", __FILE__, __LINE__);
		return false;
	}

	if (dest + len > iSize)
	{
		Log(TAG "%s:%d: dest address error.", __FILE__, __LINE__);
		return false;
	}

	s32 iResult = 0;

	u8 *ptr = &this->pData[dest];
	memcpy(ptr, src, len);

	FILE *pFp = fopen(strPath, "wb+");
	if (!pFp)
	{
		Info(TAG "fopen: Could not open '%s' for writing", strPath);
		eLastError = Seed::ErrorAccessDenied;
		return false;
	}

	iResult = fseek(pFp, 0, SEEK_SET); //fseek(pFp, static_cast<s32>(dest), SEEK_SET);
	if (iResult)
	{
		Log(TAG "fseek failed: %d", iResult);
		eLastError = Seed::ErrorAccessDenied;
		fclose(pFp);
		return false;
	}

	u32 iAmountWritten = (u32)fwrite(this->pData, 1, iSize, pFp); //fwrite(src, 1, len, pFp);
	fclose(pFp);

	if (iAmountWritten != iSize) //len)
	{
		Log(TAG "fwrite: Could not write all the data requested: AmountWritten = %d, Requested = %d\n", iAmountWritten, iSize); //len);
		eLastError = Seed::ErrorDeviceFull;
		return false;
	}

	return true;
}

u32 Cartridge::GetCardType(eCartridgeSize size)
{
	u32 i = 0;
	switch (size)
	{
		case Seed::Cartridge512b:
			i = 512;
		break;

		case Seed::Cartridge8192b:
			i = 8192;
		break;

		case Seed::Cartridge32768b:
			i = 32768;
		break;

		case Seed::Cartridge65536b:
			i = 65536;
		break;

		case Seed::Cartridge262144b:
			i = 262144;
		break;

		case Seed::CartridgeUnlimited:
			i = 1048576;
		break;

		default:
			Log(TAG "Cartridge size invalid.");
		break;
	}

	return i;
}

bool Cartridge::Verify(const char *filename, u32 filesize)
{
	u32 len = 0;
	bool ret = false;

	if (this->GetFileSize(filename, &len))
	{
		if (len == filesize)
		{
			ret = true;
		}
	}

	return ret;
}

bool Cartridge::GetFileSize(const char *filename, u32 *length)
{
	FILE *fp = fopen(filename, "rb");
	if (!fp)
	{
		this->eLastError = Seed::ErrorAccessDenied;
		return false;
	}

	s32 ret = fseek(fp, 0, SEEK_END);
	*length = static_cast<u32>(ftell(fp));
	if (ret != 0)
	{
		fclose(fp);
		this->eLastError = Seed::ErrorAccessDenied;
		return false;
	}

	ret = fclose(fp);
	if (ret != 0)
   {
		this->eLastError = Seed::ErrorAccessDenied;
		return false;
   }

	this->eLastError = Seed::ErrorNone;
	return true;
}

bool Cartridge::CreateSaveFile()
{
	Log(TAG "Creating save data...");

	FILE *fp = fopen(strPath, "wb+");
	if (fp)
	{
		void *pBlankData = Alloc(iSize);
		memset(pBlankData, 0, iSize);
		bool result = this->Write(0, pBlankData, iSize);
		Free(pBlankData);

		if (!result)
		{
			Log(TAG "Could not write into the newly created file.");
			this->eLastError = Seed::ErrorDeviceFull;
			return false;
		}
	}
	else
	{
		Log(TAG "could not create file.");
		this->eLastError = Seed::ErrorAccessDenied;
		return false;
	}

	Log(TAG "Save data created.");

	this->eLastError = Seed::ErrorNone;
	return true;
}

const String Cartridge::GetObjectName() const
{
	return "Cartridge";
}

}} // namespace

#endif // BUILD_PC
