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

#ifndef __OAL_MUSIC_H__
#define __OAL_MUSIC_H__

#include "Defines.h"

#if defined(USE_API_OAL)

#include "File.h"
#include "interface/IMusic.h"
#include "Sound.h"
#include "api/oal/vorbis_util.h"

#if defined(__APPLE_CC__)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#define OPENAL_MUSIC_BUFFERS	2

namespace Seed { namespace OAL {

IResource *MusicResourceLoader(const String &filename, ResourceManager *res = pResourceManager);

/// OpenAL Ogg music resource
class SEED_CORE_API Music : public IMusic
{
	friend IResource *MusicResourceLoader(const String &filename, ResourceManager *res);
	friend class SoundSystem;

	public:
		Music();
		virtual ~Music();

		// IMusic
		virtual void Reset();
		virtual bool Update(f32 dt);
		virtual const void *GetData() const;

		virtual void SetVolume(f32 vol);
		virtual void UpdateVolume();

		// IResouce
		virtual bool Load(const String &filename, ResourceManager *res = pResourceManager);
		virtual bool Unload();

	protected:
		virtual bool DoStream(ALuint buffer);

	private:
		SEED_DISABLE_COPY(Music);

	private:
		ALuint			iBuffers[OPENAL_MUSIC_BUFFERS];
		ALuint			iSource;

		vorbis_info		*vorbisInfo;
		vorbis_comment	*vorbisComment;
		OggVorbis_File	oggStream;
		ALenum			eFormat;
		bool			bLoop;
};

}} // namespace

#else // USE_API_OAL
	#error "Include 'Music.h' instead 'api/oal/oalMusic.h' directly."
#endif // USE_API_OAL
#endif // __OAL_MUSIC_H__
