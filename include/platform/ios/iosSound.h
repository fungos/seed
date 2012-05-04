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

#ifndef __IOSONE_SOUND_H__
#define __IOSONE_SOUND_H__

#include "Defines.h"

#if defined(BUILD_IOS)

#include "File.h"
#include "interface/IResource.h"
#include "interface/ISound.h"

#include <OpenAL/al.h>

ALvoid  alBufferDataStaticProc(const ALint bid, ALenum format, ALvoid *data, ALsizei size, ALsizei freq);

namespace Seed { namespace iOS {

IResource *SoundResourceLoader(const String &filename, ResourceManager *res = pResourceManager);

/// iOS Sound resource
class SEED_CORE_API Sound : public ISound
{
	friend IResource *SoundResourceLoader(const String &ilename, ResourceManager *res);
	friend class SoundSystem;
	friend class SoundSource;

	public:
		Sound();
		virtual ~Sound();

		void Reset();

		// IResource
		virtual bool Load(const String &filename, ResourceManager *res = pResourceManager);
		virtual bool Unload();
		virtual u32 GetUsedMemory() const;

	protected:
		virtual const void *GetData() const;

	private:
		SEED_DISABLE_COPY(Sound);

		void ReadData(const char *file);

	private:
		ALuint		iBuffer;
		ALsizei		iSize;
		ALsizei		iFreq;
		ALenum		eFormat;
		void		*pData;

};

}} // namespace

#else // BUILD_IOS
	#error "Include 'Sound.h' instead 'platform/ios/iosSound.h' directly."
#endif // BUILD_IOS
#endif // __IOSONE_SOUND_H__