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

/*! \file FileSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief Filesystem module
*/

#include "FileSystem.h"
#include "Log.h"
#include "File.h"
#include "System.h"
#include "SeedInit.h"
#include "physfs/physfs.h"

#define FS_CHECK(x)	{ if (!x) { const char *_err = PHYSFS_getLastError(); Log(TAG "Error: %s", _err); }}

#define TAG	"[FileSystem] "

namespace Seed {

SEED_SINGLETON_DEFINE(FileSystem)

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
	this->Shutdown();
}

bool FileSystem::Initialize()
{
	FS_CHECK(PHYSFS_init(Seed::Private::pcArgv[0]));
	FS_CHECK(PHYSFS_setSaneConfig(Seed::pConfiguration->GetPublisherName(), Seed::pConfiguration->GetApplicationTitle(), "zip", false, false));

	return true;
}

bool FileSystem::Shutdown()
{
	FS_CHECK(PHYSFS_deinit());
	return true;
}

const FilePath *FileSystem::GetWorkDirectory() const
{
	return PHYSFS_getUserDir();
}

const FilePath *FileSystem::GetWriteableDirectory() const
{
	return PHYSFS_getWriteDir();
}

bool FileSystem::IsRequired() const
{
	return true;
}

const char *FileSystem::GetObjectName() const
{
	return "FileSystem";
}

} // namespace
