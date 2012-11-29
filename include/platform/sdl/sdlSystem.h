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

#ifndef __SDL_SYSTEM_H__
#define __SDL_SYSTEM_H__

#if defined(BUILD_SDL)

#include "interface/ISystem.h"
#include "Singleton.h"

#define SYSTEM_RETRACE_HISTORY_MAX	64

namespace Seed { namespace SDL {

/// SDL System Module
class SEED_CORE_API System : public ISystem
{
	SEED_SINGLETON_DECLARE(System)
	public:
		// ISystem
		virtual void GoToMenu();
		virtual void Sleep();
		virtual void OnHomeCalled();
		virtual void WaitForRetrace();
		virtual void HangUp();
		virtual void GoToDataManager();

		virtual bool IsShuttingDown() const;
		virtual bool IsResetting() const;
		virtual bool IsSleeping() const;

		virtual void DisableHome();
		virtual void EnableHome();
		virtual bool IsHomeEnabled() const;
		virtual bool IsHomeRunning() const;
		virtual bool InitializeHome();

		virtual void EnableDefaultCursor(bool b);

		virtual const char *GetUsername() const;
		virtual const char *GetHomeFolder() const;
		virtual const char *GetApplicationDataFolder() const;
		virtual const char *GetSaveGameFolder() const;

		// IModule
		virtual bool Initialize();
		virtual bool Shutdown();
		virtual bool Reset();

		// IUpdatable
		virtual bool Update(f32 dt);

	private:
		SEED_DISABLE_COPY(System);

	private:
		u64		iLastFrameTime;
		u64		iFpsTime;
		f32		fElapsedTime;
		u32		iRetraceCount;
		u32		iFrameRate;
		u32		iRetraceIndex;
		u32		arRetraceCount[SYSTEM_RETRACE_HISTORY_MAX];
		bool	bShutdown;
		bool	bSleeping;
		bool	bDefaultCursorEnabled;
};

#define pSystem Seed::SDL::System::GetInstance()

}} // namespace

#else // BUILD_SDL
	#error "Include 'System.h' instead 'platform/sdl/sdlSystem.h' directly."
#endif // BUILD_SDL
#endif // __SDL_SYSTEM_H__
