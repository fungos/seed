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

/*! \file Reader.cpp
	\author	Danny Angelo Carminati Grein
	\brief File reader
*/

#include "Reader.h"
#include "SeedInit.h"

#define TAG "[Reader] "

namespace Seed {

SEED_CORE_API IReader cNullReader;

Reader::Reader()
	: IReader()
	, pOpaque(&cNullReader)
{
	this->Init();
}

Reader::Reader(void *data)
	: IReader()
	, pOpaque(&cNullReader)
{
	this->Init();
	pOpaque->Load(data);
}

Reader::Reader(const File &file)
	: IReader()
	, pOpaque(&cNullReader)
{
	this->Init();
	pOpaque->Load(file.GetData());
}

Reader::Reader(Reader &reader)
	: IReader()
	, pOpaque(&cNullReader)
{
	this->Init();
	pOpaque->Load(reader.pOpaque);
}


Reader::~Reader()
{
	if (pOpaque != &cNullReader)
		Delete(pOpaque);

	pOpaque = NULL;
}

void Reader::Init()
{
	if (pOpaque == &cNullReader || pOpaque == NULL)
	{
		switch (pConfiguration->GetReaderType())
		{
#if defined(SEED_ENABLE_JSON)
			case ReaderJson:
			{
				Info(TAG "Creating reader json");
				pOpaque = New(JsonReader());
			}
			break;
#endif

			default:
			{
#if defined(SEED_ENABLE_JSON)
				Info(TAG "Creating reader json");
				pOpaque = New(JsonReader());
#else
				Info(TAG "Failed creating reader, using null");
#endif
			}
			break;
		}
	}
}

const char *Reader::ReadString(const char *key) const
{
	return pOpaque->ReadString(key);
}

s32 Reader::ReadS32(const char *key) const
{
	return pOpaque->ReadS32(key);
}

u32 Reader::ReadU32(const char *key) const
{
	return (u32)pOpaque->ReadS32(key);
}

f32 Reader::ReadF32(const char *key) const
{
	return pOpaque->ReadF32(key);
}

bool Reader::ReadBool(const char *key) const
{
	return pOpaque->ReadBool(key);
}

u32 Reader::SelectArray(const char *key) const
{
	return pOpaque->SelectArray(key);
}

void Reader::Next()
{
	return pOpaque->Next();
}

IReader &Reader::GetNext() const
{
	return pOpaque->GetNext();
}

IReader &Reader::GetNode(const char *key) const
{
	return pOpaque->GetNode(key);
}

} // namespace

