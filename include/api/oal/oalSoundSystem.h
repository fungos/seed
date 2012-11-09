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

#ifndef __OAL_SOUND_SYSTEM_H__
#define __OAL_SOUND_SYSTEM_H__

#include "Defines.h"

#if defined(USE_API_OAL)

#include "interface/ISoundSystem.h"
#include "Singleton.h"

#if defined(__APPLE_CC__)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#define SOUND_MASTER_VOLUME		0.2f

namespace Seed { namespace OAL {

/// OpenAL Sound system
class SEED_CORE_API SoundSystem : public ISoundSystem
{
	SEED_SINGLETON_DECLARE(SoundSystem)
	public:
		virtual void Pause();
		virtual void Resume();

		// IUpdatable
		virtual bool Update(f32 dt);

		// IModule
		virtual bool Initialize();
		virtual bool Reset();
		virtual bool Shutdown();

	private:
		SEED_DISABLE_COPY(SoundSystem);

		void UpdateMusic(f32 dt, IMusic *mus);
		void UpdateSounds(f32 dt);

	private:
		ALCdevice			*pDevice;
		ALCcontext			*pContext;
};

#define pSoundSystem Seed::OAL::SoundSystem::GetInstance()

}} // namespace

#else // USE_API_OAL
	#error "Include 'SoundSystem.h' instead 'api/oal/oalSoundSystem.h' directly."
#endif // USE_API_OAL
#endif // __OAL_SOUND_SYSTEM_H__
