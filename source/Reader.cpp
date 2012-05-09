/*
* Copyright (c) 2012, Seed Developers
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
* 4. A credit line is required in the game credits, as the following line:
*    Powered by Seed Framework
* 5. A Seed Framework logo should be displayed within a splash screen, or in
*    the case where the game haven't or cannot have a splash, a logo should be
*    displayed somewhere in the game (eg. menu, credits, other place).
*
* THIS SOFTWARE IS PROVIDED BY SEED DEVELOPERS ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL SEED DEVELOPERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Reader.h"
#include "Configuration.h"
#include "api/yajl/JsonReader.h"

#define TAG "[Reader] "

namespace Seed {

SEED_CORE_API IReader cNullReader;

Reader::Reader()
	: IReader()
	, pOpaque(&cNullReader)
{
	this->Init();
}

Reader::Reader(const void *data)
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

	IReader *r = reader.pOpaque;
	pOpaque->Load(*r);
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
#if defined(SEED_USE_JSON)
			case ReaderJson:
			{
				Info(TAG "Creating reader json");
				pOpaque = New(JsonReader());
			}
			break;
#endif

			default:
			{
#if defined(SEED_USE_JSON)
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

const char *Reader::ReadString(const char *key, const char *value) const
{
	return pOpaque->ReadString(key, value);
}

s32 Reader::ReadS32(const char *key, s32 value) const
{
	return pOpaque->ReadS32(key, value);
}

u32 Reader::ReadU32(const char *key, u32 value) const
{
	return (u32)pOpaque->ReadS32(key, value);
}

f32 Reader::ReadF32(const char *key, f32 value) const
{
	return pOpaque->ReadF32(key, value);
}

bool Reader::ReadBool(const char *key, bool value) const
{
	return pOpaque->ReadBool(key, value);
}

u32 Reader::SelectArray(const char *key)
{
	return pOpaque->SelectArray(key);
}

void Reader::SelectNext()
{
	pOpaque->SelectNext();
}

void Reader::UnselectArray()
{
	pOpaque->UnselectArray();
}

bool Reader::IsNode(const char *key) const
{
	return pOpaque->IsNode(key);
}

bool Reader::SelectNode(const char *key)
{
	return pOpaque->SelectNode(key);
}

void Reader::UnselectNode()
{
	pOpaque->UnselectNode();
}

} // namespace

