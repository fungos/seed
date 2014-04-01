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

#ifndef __JSONWRITER_H__
#define __JSONWRITER_H__

#include "Defines.h"

#if SEED_USE_JSON == 1

#include "yajl/api/yajl_gen.h"
#include "interface/IWriter.h"
#include "Container.h"

namespace Seed {

/// JSON Data writer
class SEED_CORE_API JsonWriter: public IWriter
{
	SEED_DISABLE_COPY(JsonWriter)

	public:
		JsonWriter();
		virtual ~JsonWriter();

		// IWriter
		virtual bool Save(const String &file) override;
		virtual void WriteString(const char *key, const char *value) const override;
		virtual void WriteU32(u32 value) const override;
		virtual void WriteU32(const char *key, u32 value) const override;
		virtual void WriteS32(const char *key, s32 value) const override;
		virtual void WriteF32(const char *key, f32 value) const override;
		virtual void WriteBool(const char *key, bool value) const override;
		virtual void OpenArray(const char *key) override;
		virtual void CloseArray() override;
		virtual void OpenNode(const char *key = "") override;
		virtual void CloseNode() override;

	private:
		yajl_gen gen;
};

} // namespace

#endif // SEED_USE_JSON

#endif // __JSONWRITER_H__
