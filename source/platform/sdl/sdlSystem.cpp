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

#if defined(BUILD_SDL)

#include "Defines.h"
#include "System.h"
#include "Log.h"
#include "Timer.h"
#include "EventSystem.h"
#include "SeedInit.h"
#include "physfs/physfs.h"
#include "platform/pc/platform.h"

#define TAG "[System] "

#define MAX_FRAMESKIP_THRESHOLD 10

namespace Seed { namespace SDL {

SEED_SINGLETON_DEFINE(System)

System::System()
	: iRetraceCount(0)
	, iFrameRate(FrameRateLockAt60)
	, bShutdown(false)
	, bSleeping(false)
	, bDefaultCursorEnabled(false)
	, fElapsedTime(0.0f)
	, iLastFrameTime(0)
	, iFpsTime(0)
{
}

System::~System()
{
}

bool System::Reset()
{
	return true;
}

bool System::Initialize()
{
	Log(TAG "Initializing...");
	print_system_info();

	iFrameRate = pConfiguration->GetFrameRate();

	if (!pConfiguration->GetCanHaveMultipleInstances() && !system_check_multiple_instance())
	{
		exit(1);
	}

	if (SDL_Init(SDL_INIT_TIMER) < 0 ) // SDL_INIT_VIDEO
	{
		Info(TAG "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	//atexit(SDL_Quit);
	//freopen("CON", "w", stdout);
	//freopen("CON", "w", stderr);

	Log(TAG "Initialization completed.");

	return true;
}

bool System::Shutdown()
{
	this->bShutdown = true;
	Log(TAG "Terminating...");

	return true;
}

bool System::Update(f32 dt)
{
	UNUSED(dt);

	u8 state = SDL_GetAppState();
	if ((state & SDL_APPACTIVE) != SDL_APPACTIVE || (state & SDL_APPINPUTFOCUS) != SDL_APPINPUTFOCUS)
	{
		if (!this->bSleeping)
		{
			this->bSleeping = true;

			EventSystem ev;
			this->SendEventSleep(&ev);
		}
	}
	else
	{
		if (this->bSleeping)
		{
			this->bSleeping = false;

			EventSystem ev;
			this->SendEventSleep(&ev);
		}
	}

	//this->WaitForRetrace(this->iFrameRate);
	return true;
}

void System::Sleep()
{
	Log(TAG "WARNING: Platform doesnt support sleep mode.");
}

bool System::IsSleeping() const
{
	return this->bSleeping;
}

bool System::IsShuttingDown() const
{
	return this->bShutdown;
}

bool System::IsResetting() const
{
	return false;
}

void System::WaitForRetrace(eSystemFrameRate rate)
{
	++this->iRetraceCount;

	if (!this->iLastFrameTime)
		this->iLastFrameTime = pTimer->GetMilliseconds();

	f32 fFrameMaxTime			= 1000.0f / (f32)rate;

	do
	{
		//hold fps
		u64 iTime				= pTimer->GetMilliseconds();
		u64 iFrameTime			= iTime - iLastFrameTime;
		this->iFpsTime			+= iFrameTime;
		this->fElapsedTime		+= iFrameTime;
		this->iLastFrameTime	= iTime;
	} while (this->fElapsedTime < fFrameMaxTime);

	this->fElapsedTime -= fFrameMaxTime;

	//Raptor: test fix for when WM_PAINT stops comming for a long time due to the
	//user moving the window, for instance. Tries to avoid the retrace trying to
	//catch up with the lost frame time
	if ((this->fElapsedTime / fFrameMaxTime) > MAX_FRAMESKIP_THRESHOLD)
		this->fElapsedTime = 0;

	if (this->iFpsTime > 1000)
	{
		Dbg("FPS: %d", this->iRetraceCount);

		arRetraceCount[iRetraceIndex++] = this->iRetraceCount;
		if (iRetraceIndex >= SYSTEM_RETRACE_HISTORY_MAX)
			iRetraceIndex = 0;

		this->iFpsTime -= 1000;
		this->iRetraceCount = 0;
	}
}

const char *System::GetUsername() const
{
	return get_user_name();
}

const char *System::GetHomeFolder() const
{
	return PHYSFS_getUserDir();
}

const char *System::GetApplicationDataFolder() const
{
	return PHYSFS_getBaseDir();
}

const char *System::GetSaveGameFolder() const
{
	return PHYSFS_getUserDir();
}

void System::GoToMenu()
{
}

void System::OnHomeCalled()
{
}

void System::GoToDataManager()
{
}

void System::HangUp()
{
}

bool System::IsHomeEnabled() const
{
	return false;
}

bool System::IsHomeRunning() const
{
	return false;
}

bool System::InitializeHome()
{
	return true;
}

void System::EnableHome()
{
}

void System::DisableHome()
{
}

void System::EnableDefaultCursor(bool b)
{
	ISystem::EnableDefaultCursor(b);

	SDL_ShowCursor(b);
}

}} // namespace

#endif // BUILD_SDL
