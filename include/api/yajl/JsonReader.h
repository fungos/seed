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

#ifndef __JSONREADER_H__
#define __JSONREADER_H__

#include "Defines.h"

#if SEED_USE_JSON == 1

#include "yajl/api/yajl_tree.h"
#include "interface/IReader.h"
#include "Array.h"

namespace Seed {

/// JSON Data reader
class SEED_CORE_API JsonReader : public IReader
{
	public:
		JsonReader();
		virtual ~JsonReader();

		JsonReader(const JsonReader &other);
		JsonReader &operator=(const JsonReader &other);

		// IReader
		virtual bool Load(const void *data);
		virtual bool Load(const IReader &reader);
		virtual const char *ReadString(const char *key, const char *value) const;
		virtual u32 ReadU32(const char *key, u32 value) const;
		virtual s32 ReadS32(const char *key, s32 value) const;
		virtual f32 ReadF32(const char *key, f32 value) const;
		virtual bool ReadBool(const char *key, bool value) const;

		virtual u32 SelectArray(const char *key);
		virtual void SelectNext();
		virtual void SelectNode(const char *key);
		virtual void Unselect();

	private:
		JsonReader(const yajl_val node);

	private:
		yajl_val	pRootNode;
		yajl_val	pCurNode;
		yajl_val	pCurArray;
		u32			iPos;
};

} // namespace

#endif // SEED_USE_JSON

#endif // __JSONREADER_H__
