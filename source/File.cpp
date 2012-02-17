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

/*! \file File.cpp
	\author	Danny Angelo Carminati Grein
	\brief File
*/

#include "Defines.h"
#include "File.h"
#include "FileSystem.h"
#include "Log.h"

#define TAG		"[File] "

namespace Seed {

File::File(const char *filename)
	: pName(filename)
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
{
	pName = other.pName;
	pHandle = other.pHandle;
	pData = other.pData;
	iSize = other.iSize;
}

File &File::operator=(const File &other)
{
	if (this != &other)
	{
		pName = other.pName;
		pHandle = other.pHandle;
		pData = other.pData;
		iSize = other.iSize;
	}

	return *this;
}

bool File::Open()
{
	if (!pName)
	{
		Log(TAG "Error: Invalid filename!");
		return false;
	}

	pHandle = PHYSFS_openRead(pName);
	if (!pHandle)
	{
		Log(TAG "Error: %s", PHYSFS_getLastError());
		return false;
	}

	iSize = static_cast<u32>(PHYSFS_fileLength(pHandle));
	return true;
}

void File::Close()
{
	Delete(pData);

	if (!PHYSFS_close(pHandle))
	{
		Log(TAG, "Error: %s", PHYSFS_getLastError());
	}
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

void *File::GetData() const
{
	if (pData)
		return pData;

	if (this->Check())
	{
		pData = (void *)Alloc(iSize);
		if (PHYSFS_read(pHandle, pData, iSize, -1) != -1)
			return pData;
	}

	return NULL;
}

u32 File::GetSize() const
{
	return iSize;
}

const char *File::GetName() const
{
	return pName;
}

const char *File::GetObjectName() const
{
	return "File";
}

int File::GetObjectType() const
{
	return Seed::ObjectFile;
}

} // namespace
