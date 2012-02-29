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

#ifndef __VORBIS_UTIL_H__
#define __VORBIS_UTIL_H__

#if defined(BUILD_SDL) || defined(BUILD_QT)

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

#if defined(__APPLE_CC__)
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif

#include "Defines.h"

// 40kb para que em maquinas lentas nao fique engasgando no som
#define VORBIS_BUFFER_SIZE SEED_MUSIC_STREAMING_BUFFER_SIZE


struct SEED_CORE_API sOggFile
{
	u8		*dataPtr;
	u32		dataSize;
	u64		dataRead;

	sOggFile()
		: dataPtr(NULL)
		, dataSize(0)
		, dataRead(0)
	{}
};


size_t vorbis_read(void *ptr, size_t byteSize, size_t sizeToRead, void *datasource);
int vorbis_seek(void *datasource, ogg_int64_t offset, int whence);
int vorbis_close(void *datasource);
long vorbis_tell(void *datasource);
bool ogg_update_stream(OggVorbis_File *oggStream, ogg_int64_t rate, ALenum format, ALuint buffer, bool loop);

#endif // BUILD_SDL || BUILD_QT

#endif // __VORBIS_UTIL_H__
