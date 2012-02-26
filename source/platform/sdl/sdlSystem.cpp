/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

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

const FilePath *System::GetUsername() const
{
	return get_user_name();
}

const FilePath *System::GetHomeFolder() const
{
	return PHYSFS_getUserDir();
}

const FilePath *System::GetApplicationDataFolder() const
{
	return PHYSFS_getBaseDir();
}

const FilePath *System::GetSaveGameFolder() const
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
