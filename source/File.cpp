/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file EventSystem.cpp
	\author	Everton Fernando Patitucci da Silva
	\brief A system event
*/

#include "Defines.h"
#include "File.h"
#include "FileSystem.h"
#include "Log.h"

#define TAG		"[File] "

namespace Seed {

File::File()
	: pName(NULL)
	, pHandle(NULL)
	, iSize(0)
{
}

File::File(const File &other)
{
	pName = other.pName;
	pHandle = other.pHandle;
	iSize = other.iSize;
}

File::File &operator=(const File &other)
{
	if (this != &other)
	{
		pName = other.pName;
		pHandle = other.pHandle;
		iSize = other.iSize;
	}
}

File::File(const char *filename)
	: pName(filename)
	, pHandle(NULL)
	, iSize(0)
{

}

File::~File()
{
	this->Close();
}

bool File::Open(OpenMode mode)
{
	if (!pName) {
		Log(TAG "Error: Invalid filename!");
		return false;
	}

	switch (mode)
	{
		case ReadOnly:
			pHandle = PHYSFS_openRead(pName);
			break;
		case ReadWrite:
			pHandle = PHYSFS_openWrite(pName);
			break;
		case Append:
			pHandle = PHYSFS_openAppend(pName);
			break;
		default:
			break;
	}

	if (!pHandle) {
		Log(TAG "Error: %s", PHYSFS_getLastError());
		return false;
	}

	iSize = static_cast<u32>(PHYSFS_fileLength(pHandle));

	return true;
}


void File::Close()
{
	if (!PHYSFS_close(pHandle))
	{
		Log(TAG, "Error: %s", PHYSFS_getLastError());
	}
}

void File::SetSize(u32 size)
{
	iSize = size;
}

u32 File::GetSize() const
{
	return iSize;
}

void *File::ReadAll() const
{
	if (Check()) {
		void *data = Alloc(iSize);
		if (PHYSFS_read(pHandle, data, iSize, -1) != -1) {
			return data;
		}
	}

	return NULL;
}

bool File::Read(void *buffer, int size)
{
	if (Check()) {
		if (PHYSFS_read(pHandle, buffer, size, -1) != -1) {
			return true;
		}
	}

	return false;
}

bool File::Eof() const
{
	return (Check() && PHYSFS_eof(pHandle);
}

bool File::Seek(int pos) const
{
	return (Check() && (PHYSFS_seek(pHandle, pos) != 0);
}

u32 File::Tell() const
{
	return PHYSFS_tell(pHandle);
}

const char *File::GetName() const
{
	return pName;
}

void File::SetName(const char *name)
{
	pName = name;
}

const char *File::GetObjectName() const
{
	return "File";
}

int File::GetObjectType() const
{
	return Seed::ObjectFile;
}

bool File::Check()
{
	if (!pHandle) {
		Log(TAG, "Error: Invalid handle, open the file before call this function");
		return false;
	}

	return true;
}

} // namespace
