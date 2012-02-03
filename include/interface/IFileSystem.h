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

/*! \file IFileSystem.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the File System class interface
*/

#ifndef __IFILESYSTEM_H__
#define __IFILESYSTEM_H__

#include "IModule.h"
#include "Base.h"
#include "Enum.h"

#include <vector>
#include <algorithm>

namespace Seed {

/// FileSystem Interface
/**
Any FileSystem implementation must implement this interface.
*/
class SEED_CORE_API IFileSystem : public IModule
{
	public:
		IFileSystem();
		virtual ~IFileSystem();

		virtual void SetWorkDirectory(const FilePath *dir);
		virtual void SetWriteableDirectory(const FilePath *dir);
		virtual void MakeDirectory(const FilePath *dir) const;

		const FilePath *GetWorkDirectory() const;
		const FilePath *GetWriteableDirectory() const;

		// IModule
		virtual bool Initialize();
		virtual bool IsRequired() const;

		// IObject
		virtual const char *GetObjectName() const;

	protected:
		const FilePath *pWorkDir;
		const FilePath *pWriteDir;

	private:
		SEED_DISABLE_COPY(IFileSystem);
};

} // namespace

#endif // __IFILESYSTEM_H__
