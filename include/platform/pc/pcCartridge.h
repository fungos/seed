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

/*! \file pcCartridge.h
	\author	Danny Angelo Carminati Grein
	\brief Cartridge PC Implementation
*/

#ifndef __PC_CARTRIDGE_H__
#define __PC_CARTRIDGE_H__

#include "Defines.h"

#if defined(BUILD_PC)

#include "interface/ICartridge.h"
#include "FileSystem.h"
#include "Singleton.h"

#include <stdio.h>
#include <stdlib.h>

#define CARTRIDGE_SECTOR_ROUND(n, a)     (((u32) (n) + (a) - 1) & ~((a) - 1))

#if SEED_PATH_WIDE == 1
#define CARTRIDGE_FILENAME	L"savedata"
#define PATH_SEPARATOR		L"/"
#else
#define CARTRIDGE_FILENAME	"savedata"
#define PATH_SEPARATOR		"/"
#endif

#define PC_MAX_PATH		2048

namespace Seed { namespace PC {

class SEED_CORE_API Cartridge : public ICartridge
{
	SEED_SINGLETON_DECLARE(Cartridge)
	public:
		// IModule
		virtual bool Initialize();
		virtual bool Reset();
		virtual bool Shutdown();

		virtual bool Prepare(eCartridgeSize size);

		virtual bool Read(u32 src, void *dest, u32 len);
		virtual bool Write(u32 dest, const void *src, u32 len);

		// IObject
		virtual const char *GetObjectName() const;

	private:
		SEED_DISABLE_COPY(Cartridge);

		u32 GetCardType(eCartridgeSize size);
		bool CreateSaveFile();
		bool Verify(const FilePath *filename, u32 filesize);
		bool GetFileSize(const FilePath *filename, u32 *length);

	private:
		FilePath strPath[PC_MAX_PATH];
		u32 iType;
		s32 iCurrentSlot;
		u8 *pData;
};

#define pCartridge Seed::PC::Cartridge::GetInstance()

}} // namespace

#else // BUILD_PC
	#error "Include 'Cartridge.h' instead 'platform/pc/pcCartridge.h' directly."
#endif // BUILD_PC
#endif // __PC_CARTRIDGE_H__
