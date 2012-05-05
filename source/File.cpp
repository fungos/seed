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

#define TAG		"[File] "

namespace Seed {

File::File()
	: IObject()
	, sName()
	, pHandle(NULL)
	, pData(NULL)
	, iSize(0)
{
}

File::File(const String &filename)
	: IObject()
	, sName(filename)
	, pHandle(NULL)
	, pData(NULL)
	, iSize(0)
{
	this->Open();
}

File::~File()
{
	this->Close();
}

File::File(const File &other)
	: IObject()
	, sName(other.sName)
	, pHandle(other.pHandle)
	, pData(other.pData)
	, iSize(other.iSize)
{
}

File &File::operator=(const File &other)
{
	if (this != &other)
	{
		sName = other.sName;
		pHandle = other.pHandle;
		pData = other.pData;
		iSize = other.iSize;
	}

	return *this;
}

void File::Load(const String &filename)
{
	this->Close();
	sName = filename;
	this->Open();
}

void File::Open()
{
	SEED_ASSERT_MSG(sName.length(), TAG "Error: No filename was given to open file!");
	pHandle = PHYSFS_openRead(sName.c_str());
	if (!pHandle)
	{
		Log(TAG "Error: file: %s - %s", sName.c_str(), PHYSFS_getLastError());
		SEED_ASSERT_MSG(false, "Aborted, file not found.");
	}

	iSize = static_cast<u32>(PHYSFS_fileLength(pHandle));
}

void File::Close()
{
	Delete(pData);
	PHYSFS_close(pHandle);
	iSize = 0;
	sName = "";
}

bool File::Check() const
{
	bool ret = true;
	if (!pHandle)
	{
		Log(TAG, "Error: Invalid handle, open the file before call this function");
		ret = false;
	}

	return ret;
}

u8 *File::GetData() const
{
	if (pData)
		return pData;

	if (this->Check())
	{
		pData = (u8 *)Alloc(iSize + 1);
		pData[iSize] = 0;
		if (PHYSFS_read(pHandle, pData, iSize, 1) != -1)
			return pData;
	}

	return NULL;
}

u32 File::GetSize() const
{
	return iSize;
}

const String &File::GetName() const
{
	return sName;
}

const String File::GetObjectName() const
{
	return "File";
}

int File::GetObjectType() const
{
	return Seed::TypeFile;
}

} // namespace
