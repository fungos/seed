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
	, pData(NULL)
	, iSize(0)
	, bPackaged(FALSE)
{
}

File::File(const File &other)
{
	pName = other.pName;
	pData = other.pData;
	iSize = other.iSize;
	bPackaged = other.bPackaged;
}

File::File &operator=(const File &other)
{
	if (this != &other)
	{
		pName = other.pName;
		pData = other.pData;
		iSize = other.iSize;
		bPackaged = other.bPackaged;
	}
}

File::File(const char *filename)
	: pName(filename)
	, pData(NULL)
	, iSize(0)
	, bPackaged(FALSE)
{

}

File::~File()
{
	this->Close();
}

void File::Close()
{
	if (!bPackaged && pData)
	{
		Free(const_cast<void *>(pData));
	}

	pName = NULL;
	pData = NULL;
	iSize = 0;
	bPackaged = FALSE;
}

void File::SetSize(u32 size)
{
	iSize = size;
}

u32 File::GetSize() const
{
	return iSize;
}

void File::SetData(const void *data)
{
	pData = data;
}

const void *File::GetData() const
{
	return pData;
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

} // namespace
