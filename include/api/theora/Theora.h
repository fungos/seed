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

#ifndef __THEORA_H__
#define __THEORA_H__

#include "Defines.h"

#if SEED_USE_THEORA == 1 && !defined(BUILD_IOS)

#include "Thread.h"
#include "Mutex.h"
#include "Image.h"
#include "Texture.h"
#include "Semaphore.h"
#include "interface/ISceneObject.h"

#include <oggplay/oggplay.h>

namespace Seed {

/// Theora video player
class SEED_CORE_API Theora : public Thread, public Image /*, public IVideo*/
{
	SEED_DISABLE_COPY(Theora)

	public:
		Theora();
		virtual ~Theora();

		virtual void Rewind();

		using Image::Load;
		virtual bool Load(const String &filename);
		virtual bool Load(const String &filename, ResourceManager *res = pResourceManager);
		virtual void Reset();
		virtual bool Unload();

		virtual bool GoToFrame(u32 frame);

		virtual u32 GetFrameCount() const;
		virtual f32 GetFrameRate() const;

		virtual void Play();
		virtual bool IsPlaying() const;

		virtual void Stop();
		virtual bool IsStopped() const;

		virtual void Pause();
		virtual bool IsPaused() const;

		virtual u32 Size() const;

		virtual void PlayToFrame(u32 frame);

		// IRenderable
		virtual void Update(f32 delta) override;
		virtual void Render() override;

		// Thread
		virtual bool Run() override;

	protected:
		bool WaitFrameRate();
		void DoPlay();
		void ProcessVideoData(OggPlayVideoData *data);
		void ConfigureRendering();

	public:
		OggPlay		*pPlayer;
		u8			*pTexData;
		Semaphore	*pSem;

		u32			iDuration;
		f32			fFps;
		u32			iFpsDenom;
		u32			iFpsNum;
		f32			fDelay;
		u32			iFrameCount;
		u32			iUntilFrame;
		u32			iTrack;
		u32			iTotalFrames;

		u32			iWidth;
		u32			iHeight;

		u32			iUVWidth;
		u32			iUVHeight;

		u32			iTexWidth;
		u32			iTexHeight;

		f32			fTexScaleX;
		f32			fTexScaleY;

		Milliseconds fLastFrameTime;
		Milliseconds fElapsedTime;

		Texture		cTexture;

		bool		bLoaded : 1;
		bool		bPaused : 1;
		bool		bPlaying : 1;
		bool		bFinished : 1;
		bool		bTerminateThread : 1;
};

} // namespace

#endif // SEED_USE_THEORA

#endif // __THEORA_H__
