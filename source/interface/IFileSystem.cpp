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

/*! \file IFileSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the File System class interface
*/

#include "interface/IFileSystem.h"
#include "interface/IEventFileSystemListener.h"
#include "Log.h"
#include "File.h"
#include "System.h"
#include "SeedInit.h"

#define TAG	"[IFileSystem] "

namespace Seed {

IFileSystem::IFileSystem()
	: pWorkDir(NULL)
	, pWriteDir(NULL)
	, vListeners()
	, pFile(NULL)
{
}

IFileSystem::~IFileSystem()
{
	this->Shutdown();
}

bool IFileSystem::Initialize()
{
	FS_CHECK(PHYSFS_init(Seed::Private::pcArgv));
	if (!pWriteDir)
		this->SetWriteableDirectory(pSystem->GetHomeFolder());

	return TRUE;
}

bool IFileSystem::Shutdown()
{
	FS_CHECK(PHYSFS_deinit());
	return TRUE;
}

void IFileSystem::SetWorkDirectory(const FilePath *dir)
{
	if (pWorkDir)
		FS_CHECK(PHYSFS_removeFromSearchPath(dir));

	pWorkDir = dir;

	FS_CHECK(PHYSFS_mount(dir));
}

const FilePath *IFileSystem::GetWorkDirectory() const
{
	return pWorkDir;
}

void IFileSystem::SetWriteableDirectory(const FilePath *dir)
{
	pWriteDir = dir;
	FS_CHECK(PHYSFS_setWriteDir(dir));
}

const FilePath *IFileSystem::GetWriteableDirectory() const
{
	return pWriteDir;
}

void IFileSystem::MakeDirectory(const FilePath *dir) const
{
	FS_CHECK(PHYSFS_mkdir(dir));
}

bool IFileSystem::IsRequired() const
{
	return TRUE;
}

const char *IFileSystem::GetObjectName() const
{
	return "FileSystem";
}

} // namespace
