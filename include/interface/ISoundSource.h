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

#ifndef __ISOUND_SOURCE_H__
#define __ISOUND_SOURCE_H__

#include "Defines.h"
#include "interface/ISceneObject.h"
#include "ISound.h"
#include "MathUtil.h"

namespace Seed {

/// Internal sound states
enum eSoundSourceState
{
	SourceNone,
	SourcePlay,
	SourcePlayStarted,
	SourcePlaying,
	SourceStop,
	SourceStopped,
	SourcePause,
	SourcePaused,
	SourceFadeIn,
	SourceFadingIn,
	SourceFadeOut,
	SourceFadingOut,
	SourceFading
};

/// Sound source interface
class SEED_CORE_API ISoundSource : public ISceneObject
{
	public:
		ISoundSource();
		virtual ~ISoundSource();

		virtual void SetVolume(f32 vol);
		virtual f32 GetVolume() const;
		/// When global volume is changed the SoundSystem will call UpdateVolume.
		virtual void UpdateVolume();

		virtual bool IsPlaying() const;
		virtual void Play();
		virtual void Stop(f32 ms = 0.0f);
		virtual void Pause();
		virtual void Resume();

		virtual void FadeOut(f32 ms);
		virtual void FadeIn(f32 ms);

		virtual void SetLoop(bool b);
		virtual bool IsLoop() const;

		// IDataObject
		virtual bool Load(Reader &reader, ResourceManager *res = pResourceManager);
		virtual bool Write(Writer &writer);
		virtual bool Unload();

		// IRenderable
		virtual void Render(const Matrix4f &worldTransform);

		// IObject
		virtual const String GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		virtual bool OnLoadFinished()= 0;
		virtual bool OnUnloadRequest() = 0;

		virtual eSoundSourceState GetState() const;

	protected:
		ISound *pSound;
		f32 fVolume;
		f32 fFadeTime;
		u64 fStartFadeTime;
		eSoundSourceState eState;
		bool bLoop;
		bool bAutoPlay;

	private:
		SEED_DISABLE_COPY(ISoundSource);
};

} // namespace

#endif // __ISOUND_SOURCE_H__
