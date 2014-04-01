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

#include "Defines.h"

#if SEED_USE_JSON == 1

#include "api/yajl/JsonWriter.h"
#include "Log.h"

#define TAG "[JsonWriter] "

namespace Seed {

JsonWriter::JsonWriter()
	: gen()
{
	gen = yajl_gen_alloc(nullptr);
	yajl_gen_config(gen, yajl_gen_beautify, 1);
}

JsonWriter::~JsonWriter()
{
	yajl_gen_free(gen);
}

bool JsonWriter::Save(const String &file)
{
	bool ret = false;

	FILE *fp = fopen(file.c_str(), "wt");
	if (fp)
	{
		const unsigned char *buf;
		size_t len;
		yajl_gen_get_buf(gen, &buf, &len);
		fwrite(buf, 1, len, fp);
		yajl_gen_clear(gen);

		fclose(fp);

		ret = true;
	}

	return ret;
}

void JsonWriter::WriteString(const char *key, const char *value) const
{
	yajl_gen_string(gen, (const unsigned char *)key, strlen(key));
	yajl_gen_string(gen, (const unsigned char *)value, strlen(value));
}

void JsonWriter::WriteU32(u32 value) const
{
	yajl_gen_integer(gen, value);
}

void JsonWriter::WriteU32(const char *key, u32 value) const
{
	yajl_gen_string(gen,(const unsigned char *) key, strlen(key));
	yajl_gen_integer(gen, value);
}

void JsonWriter::WriteS32(const char *key, s32 value) const
{
	yajl_gen_string(gen, (const unsigned char *)key, strlen(key));
	yajl_gen_integer(gen, value);
}

void JsonWriter::WriteF32(const char *key, f32 value) const
{
	yajl_gen_string(gen, (const unsigned char *)key, strlen(key));
	yajl_gen_double(gen, value);
}

void JsonWriter::WriteBool(const char *key, bool value) const
{
	yajl_gen_string(gen, (const unsigned char *)key, strlen(key));
	yajl_gen_bool(gen, value);
}

void JsonWriter::OpenArray(const char *key)
{
	yajl_gen_string(gen, (const unsigned char *)key, strlen(key));
	yajl_gen_array_open(gen);
}

void JsonWriter::CloseArray()
{
	yajl_gen_array_close(gen);
}

void JsonWriter::OpenNode(const char *key)
{
	if (strlen(key))
		yajl_gen_string(gen, (const unsigned char *)key, strlen(key));

	yajl_gen_map_open(gen);
}

void JsonWriter::CloseNode()
{
	yajl_gen_map_close(gen);
}

}

#endif // SEED_USE_JSON
