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

#ifndef __IOS_SOUND_SYSTEM_H__
#define __IOS_SOUND_SYSTEM_H__

#if defined(BUILD_IOS)

#include "interface/ISoundSystem.h"
#include "interface/IResource.h"
#include "File.h"
#include "Singleton.h"

#include <vector>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define IPHONE_MAX_VOICES		32
#define IPHONE_MAX_FILES		64
#define IPHONE_MASTER_VOLUME	0.2f

namespace Seed { namespace iOS {

/// iOS Sound Module
class SoundSystem : public ISoundSystem
{
	SEED_DECLARE_SINGLETON(SoundSystem)
	SEED_DISABLE_COPY(SoundSystem)

	public:
		// ISoundSystem
		//virtual void PlayMusic(IMusic *mus, f32 ms = 0);
		//virtual void StopMusic(f32 ms = 0, IMusic *mus = NULL);
		//virtual void StopSounds();
		virtual void Pause();
		virtual void Resume();

		//virtual void Add(ISoundSource *src);
		//virtual void Remove(ISoundSource *src);

		// IUpdatable
		virtual bool Update(Seconds dt);

		// IManager
		virtual bool Initialize();
		virtual bool Reset();
		virtual bool Shutdown();

	private:
		void UpdateMusic(Seconds dt, IMusic *mus);
		void UpdateSounds(Seconds dt);

	private:
		ALCdevice			*pDevice;
		ALCcontext			*pContext;
};

#define pSoundSystem Seed::iOS::SoundSystem::GetInstance()

}} // namespace

#else // BUILD_IOS
	#error "Include 'SoundSystem.h' instead 'platform/ios/iosSoundSystem.h' directly."
#endif // BUILD_IOS
#endif // __IHP_SOUND_SYSTEM_H__
