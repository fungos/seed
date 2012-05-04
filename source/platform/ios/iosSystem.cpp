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

#if defined(BUILD_IOS)

#include "Defines.h"
#include "System.h"
#include "Log.h"
#include "FileSystem.h"
#include "platform/ios/iosoneView.h"

#define TAG "[System] "

namespace Seed { namespace iOS {

SEED_SINGLETON_DEFINE(System);

System::System()
	: iRetraceCount(0)
	, iFrameRate(Seed::FrameRateLockAt60)
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
	Log(TAG "Initialization completed.");

	pFileSystem->SetWriteableDirectory(iphGetHomePath());

	return true;
}

bool System::Shutdown()
{
	Log(TAG "Terminated.");
	Log(TAG "Terminated.");

	return true;
}

bool System::Update(f32 delta)
{
	UNUSED(delta);

	this->WaitForRetrace(this->iFrameRate);

	return true;
}

void System::Sleep()
{
	Log(TAG "WARNING: Platform doesnt support sleep mode.");
}

bool System::IsSleeping() const
{
	return false;
}

bool System::IsShuttingDown() const
{
	return false;
}

bool System::IsResetting() const
{
	return false;
}

void System::WaitForRetrace(eSystemFrameRate rate)
{
	UNUSED(rate);
	// This platform is synced by NSTimer at AppView
	this->iRetraceCount = 0;
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

void System::DisableHome()
{
}

void System::EnableHome()
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
	return false;
}

}} // namespace

#endif // BUILD_IOS