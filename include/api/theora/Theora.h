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
#include "interface/ISceneObject.h"

#include <oggplay/oggplay.h>

/* FIXME: Somebody please create a Semaphore class! */
#if defined(linux) || defined(SOLARIS) || defined(AIX) || defined(__FreeBSD__) || defined(LINUX)
#if defined(BUILD_SDL)
	#include <SDL/SDL.h>
	#include <SDL/SDL_thread.h>
	#include <SDL/SDL_mutex.h>
	#define SEM_CREATE(p,s)		(!(p = (void *)SDL_CreateSemaphore(s)))
	#define SEM_SIGNAL(p)		SDL_SemPost((SDL_sem*)p)
	#define SEM_WAIT(p)			SDL_SemWait((SDL_sem*)p)
	#define SEM_CLOSE(p)		SDL_DestroySemaphore((SDL_sem*)p)
	#define SEM_CHECK(p)		if (p)
	#define SEM_CLEAR(p)		p = 0
	typedef void*				semaphore;
#else
	#if !defined(QT_SEM_WARNING)
		#warning "Qt Semaphores not tested!"
		#define QT_SEM_WARNING 1
	#endif
	#include <QtCore>
	#define SEM_CREATE(p, s)	{}
	#define SEM_SIGNAL(p)		p->release()
	#define SEM_CLOSE(p)		{}
	#define SEM_WAIT(p)			p->acquire()
	#define SEM_CHECK(p)		if (p)
	#define SEM_CLEAR(p)		{}
	typedef QSemaphore			*semaphore;
/*
	#include <semaphore.h>
	#if defined(__FreeBSD__)
		#define SEM_CREATE(p,s) sem_init(&(p), 0, s)
	#else
		#define SEM_CREATE(p,s) sem_init(&(p), 1, s)
	#endif
	#define SEM_SIGNAL(p)	sem_post(&(p))
	#define SEM_WAIT(p)		sem_wait(&(p))
	#define SEM_CLOSE(p)	sem_destroy(&(p))
	typedef sem_t			semaphore;
*/
#endif // BUILD_SDL
#elif defined(WIN32)
#pragma push_macro("Delete")
#pragma push_macro("bool")
#pragma push_macro("SIZE_T")
#undef Delete
#if defined(_MSC_VER)
#undef bool
#endif
#undef SIZE_T
#include <windows.h>
#pragma pop_macro("SIZE_T")
#pragma pop_macro("bool")
#pragma pop_macro("Delete")
	#define SEM_CREATE(p,s) ((p = CreateSemaphore(NULL, (long)(s), (long)(s), NULL)) == 0)
	#define SEM_SIGNAL(p)   (!ReleaseSemaphore(p, 1, NULL))
	#define SEM_WAIT(p)     WaitForSingleObject(p, INFINITE)
	#define SEM_CLOSE(p)    (!CloseHandle(p))
	#define SEM_CHECK(p)	if (p)
	#define SEM_CLEAR(p)	p = 0
	typedef HANDLE          semaphore;
#elif defined(__APPLE_CC__)
	#include <SDL/SDL.h>
	#include <SDL/SDL_thread.h>
	#include <SDL/SDL_mutex.h>
	#define SEM_CREATE(p,s)		(!(p = (void *)SDL_CreateSemaphore(s)))
	#define SEM_SIGNAL(p)		SDL_SemPost((SDL_sem*)p)
	#define SEM_WAIT(p)			SDL_SemWait((SDL_sem*)p)
	#define SEM_CLOSE(p)		SDL_DestroySemaphore((SDL_sem*)p)
	#define SEM_CHECK(p)		if (p)
	#define SEM_CLEAR(p)		p = 0
	typedef void*				semaphore;
/*#elif defined(__APPLE__)
	//#include <Carbon/Carbon.h>
	#define SEM_CREATE(p,s) MPCreateSemaphore(s, s, &(p))
	#define SEM_SIGNAL(p)   MPSignalSemaphore(p)
	#define SEM_WAIT(p)     MPWaitOnSemaphore(p, kDurationForever)
	#define SEM_CLOSE(p)    MPDeleteSemaphore(p)
	#define SEM_CHECK(p)	if (p)
	#define SEM_CLEAR(p)		p = 0
	typedef MPSemaphoreID   semaphore;*/
#endif

namespace Seed {

/// Theora video player
class SEED_CORE_API Theora : public Thread, public Image /*, public IVideo*/
{
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
		virtual void Update(f32 delta);
		virtual void Render();

		// Thread
		virtual bool Run();

	protected:
		bool WaitFrameRate();
		void DoPlay();
		void ProcessVideoData(OggPlayVideoData *data);
		void ConfigureRendering();

	private:
		SEED_DISABLE_COPY(Theora);

	public:
		OggPlay		*pPlayer;
		u8			*pTexData;

		u32			iTime;
		u32			iDuration;
		f32			fFps;
		u32			iFpsDenom;
		u32			iFpsNum;
		f32			fDelay;
		u32			iFrameCount;
		u32			iUntilFrame;
		u64			iLastFrameTime;
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
		f32			fElapsedTime;

		bool		bLoaded;
		bool		bPaused;
		bool		bPlaying;
		bool		bFinished;
		bool		bTerminateThread;
		semaphore	sem;

		Texture		cTexture;
		//Image		cImage;
};

} // namespace

#endif // SEED_USE_THEORA

#endif // __THEORA_H__
