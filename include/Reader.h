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

/*! \file Reader.h
	\author	Danny Angelo Carminati Grein
	\brief File reader
*/

#ifndef __READER_H__
#define __READER_H__

#include "Defines.h"
#include "File.h"
#include "interface/IReader.h"

namespace Seed {

#define ReaderPath(s) const char *s[] = {"##s##", (const char *)0};
#define ReaderPath1(s, p1) const char *s[] = {p1, (const char *)0};
#define ReaderPath2(s, p1, p2) const char *s[] = {p1, p2, (const char *)0};

extern SEED_CORE_API IReader cNullReader;

class SEED_CORE_API Reader : public IReader
{
	public:
		Reader();
		Reader(void *data);
		Reader(const File &file);
		virtual ~Reader();

		// IReader
		bool Load(const void *data);
		const char *ReadString(const char **path);
		s32 ReadS32(const char **path);
		u32 ReadU32(const char **path);
		f32 ReadF32(const char **path);
		bool ReadBool(const char **path);

	private:
		SEED_DISABLE_COPY(Reader);

		IReader	*pOpaque;
};

} // namespace

#endif // __READER_H__
