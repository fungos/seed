/*
* Copyright (c) 2013, Seed Developers
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

#ifndef __SDL2_SYSTEM_H__
#define __SDL2_SYSTEM_H__

#if defined(BUILD_SDL2)

#include "interface/ISystem.h"
#include "Singleton.h"

#define SYSTEM_RETRACE_HISTORY_MAX	64

namespace Seed { namespace SDL2 {

/// SDL System Module
class SEED_CORE_API System : public ISystem
{
	SEED_DECLARE_SINGLETON(System)
	SEED_DECLARE_MANAGER(System)
	SEED_DISABLE_COPY(System)
	public:
		// ISystem
		virtual void GoToMenu() override;
		virtual void Sleep() override;
		virtual void OnHomeCalled();
		virtual void WaitForRetrace() override;
		virtual void HangUp() override;
		virtual void GoToDataManager() override;

		virtual bool IsShuttingDown() const override;
		virtual bool IsResetting() const override;
		virtual bool IsSleeping() const override;

		virtual void DisableHome() override;
		virtual void EnableHome() override;
		virtual bool IsHomeEnabled() const override;
		virtual bool IsHomeRunning() const override;
		virtual bool InitializeHome() override;

		virtual void EnableDefaultCursor(bool b) override;

		virtual const char *GetUsername() const override;
		virtual const char *GetHomeFolder() const override;
		virtual const char *GetApplicationDataFolder() const override;
		virtual const char *GetSaveGameFolder() const override;

		// IModule
		virtual bool Initialize() override;
		virtual bool Shutdown() override;
		virtual bool Reset() override;

		// IUpdatable
		virtual bool Update(f32 dt) override;

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
};

#define pSystem Seed::SDL2::System::GetInstance()

}} // namespace

#else // BUILD_SDL2
	#error "Include 'System.h' instead 'platform/sdl/sdlSystem.h' directly."
#endif // BUILD_SDL2
#endif // __SDL2_SYSTEM_H__
