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
#include "File.h"
#include "FileSystem.h"
#include "Log.h"
#include "Enum.h"
#include "Memory.h"

#define TAG		"[File] "

namespace Seed {

File::File()
	: IObject()
	, pHandle(nullptr)
	, pData(nullptr)
	, sFilename()
	, iSize(0)
{
}

File::File(const String &filename)
	: IObject()
	, pHandle(nullptr)
	, pData(nullptr)
	, sFilename(filename)
	, iSize(0)
{
	this->Load(filename);
}

File::~File()
{
	this->Close();
}

bool File::Load(const String &filename)
{
	this->Close();
	sFilename = filename;
	return this->Open();
}

bool  File::Unload()
{
	this->Close();
	return true;
}

bool File::Open()
{
	bool ret = true;
	SEED_ASSERT_MSG(sFilename.length(), "Error: No filename was given to open file!");
	pHandle = PHYSFS_openRead(sFilename.c_str());
	if (!pHandle)
	{
		Log(TAG "Error: file: %s - %s", sFilename.c_str(), PHYSFS_getLastError());
		//SEED_ASSERT_MSG(false, "Aborted, file not found.");
		return false;
	}
	else
		iSize = static_cast<u32>(PHYSFS_fileLength(pHandle));

	return ret;
}

void File::Close()
{
	sdFree(pData);
	if (pHandle)
		PHYSFS_close(pHandle);
	iSize = 0;
	sFilename = "";
}

bool File::Check() const
{
	bool ret = true;
	if (!pHandle)
	{
		Log(TAG "Error: Invalid handle, open the file before call this function");
		ret = false;
	}

	return ret;
}

u8 *File::GetData() const
{
	WARNING(FIXME - mutex lock guard here)
	if (pData)
		return pData;

	if (this->Check())
	{
		pData = (u8 *)sdAlloc(iSize + 1);
		pData[iSize] = 0;
		if (PHYSFS_read(pHandle, pData, iSize, 1) != -1)
			return pData;
	}

	return nullptr;
}

u32 File::GetSize() const
{
	return iSize;
}

const String &File::GetName() const
{
	return sFilename;
}


FileLoader::FileLoader(const String &filename, JobCallback fun)
	: Job(fun)
	, sFilename(filename)
	, pFile(nullptr)
{
}

FileLoader::~FileLoader()
{
	sdDelete(pFile);
}

bool FileLoader::Run()
{
	Log("[Job:FileLoader] Load: %s", sFilename.c_str());

	cMutex.lock();
	bool run = (nState == eJobState::Running);
	cMutex.unlock();

	if (run)
	{
		pFile = sdNew(File(sFilename));
		pFile->GetData();
		Log("[Job:FileLoader] Loaded: %s (%d bytes)", sFilename.c_str(), pFile->GetSize());

		cMutex.lock();
		nState = eJobState::Completed;
		cMutex.unlock();

		Log("[Job:FileLoader] Completed: %s", sFilename.c_str());
	}

	return false;
}

} // namespace
