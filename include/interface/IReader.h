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

/*! \file IReader.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the file format reader interface
*/

#ifndef __IREADER_H__
#define __IREADER_H__

#include "Defines.h"

namespace Seed {

class SEED_CORE_API IReader
{
	public:
		IReader();
		virtual ~IReader();

		IReader(const IReader &other);
		IReader &operator=(const IReader &other);

		bool Load(const void *data);
		bool Load(IReader *reader);
		const char *ReadString(const char *key) const;
		u32 ReadU32(const char *key) const;
		s32 ReadS32(const char *key) const;
		f32 ReadF32(const char *key) const;
		bool ReadBool(const char *key) const;

		u32 SelectArray(const char *key) const;
		void Next();
		IReader &GetNext() const;
		IReader &GetNode(const char *key) const;
};

} // namespace

#endif // __IREADER_H__
