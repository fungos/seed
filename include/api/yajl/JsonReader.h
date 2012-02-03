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

/*! \file JsonReader.h
	\author	Danny Angelo Carminati Grein
	\brief Json Reader
*/

#ifndef __JSONREADER_H__
#define __JSONREADER_H__

#include "Defines.h"

#if SEED_USE_JSON == 1

#include "yajl/api/yajl_tree.h"
#include "interface/IReader.h"

namespace Seed {

class SEED_CORE_API JsonReader : public IReader
{
	public:
		JsonReader();
		virtual ~JsonReader();

		// IReader
		bool Load(void *data);
		const char *ReadString(const char **path);
		s32 ReadS32(const char **path);
		f32 ReadF32(const char **path);
		bool ReadBool(const char **path);

	private:
		yajl_val _node;
};

} // namespace

#endif // SEED_USE_JSON

#endif // __JSONREADER_H__
