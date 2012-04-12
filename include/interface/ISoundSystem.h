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

#ifndef __ISOUND_SYSTEM_H__
#define __ISOUND_SYSTEM_H__

#include "IModule.h"
#include "IUpdatable.h"
#include "ISoundSource.h"
#include "Container.h"

#define DEFAULT_FADE_OUT		15

namespace Seed {

class ISound;
class IMusic;

/// Sound interface
/**
Interface for sound implementations.
*/
class SEED_CORE_API ISoundSystem : public IModule, public IUpdatable
{
	protected:
		DECLARE_CONTAINER_TYPE(Vector, ISoundSource)

	public:
		ISoundSystem();
		virtual ~ISoundSystem();

		virtual void SetMusicVolume(f32 volume);
		virtual f32 GetMusicVolume() const;

		virtual void SetSfxVolume(f32 volume);
		virtual f32 GetSfxVolume() const;

		virtual void Mute();
		virtual void Unmute();

		virtual void PlayMusic(IMusic *mus, f32 ms = 0);
		virtual void StopMusic(f32 ms = 0, IMusic *mus = NULL);
		virtual void StopSounds();
		virtual void Pause();
		virtual void Resume();

		virtual void Add(ISoundSource *src);
		virtual void Remove(ISoundSource *src);

		// IObject
		virtual const String GetObjectName() const;

	protected:
		IMusic	*pCurrentMusic;
		IMusic	*pNewMusic;

		ISoundSourceVector	vSource;

		f32 	fMusicVolume;
		f32		fSfxVolume;
		f32 	fMusicVolumeOrig;
		f32		fSfxVolumeOrig;
		f32		fMusicStartFadeTime;
		f32		fMusicFadeTime;

		bool	bMuted;
		bool	bChanged;
		bool	bPaused;

	private:
		SEED_DISABLE_COPY(ISoundSystem);
};

} // namespace

#endif // __ISOUND_SYSTEM_H__
