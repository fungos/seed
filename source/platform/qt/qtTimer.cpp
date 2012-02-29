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

#if defined(BUILD_QT)

#include <QWaitCondition>

#include "Defines.h"
#include "Timer.h"
#include "Log.h"

#define TAG "[Timer] "

namespace Seed { namespace QT {

SEED_SINGLETON_DEFINE(Timer)

Timer::Timer()
{
}

Timer::~Timer()
{
}

bool Timer::Initialize()
{
	Log(TAG "Initializing...");
	IModule::Initialize();
	this->Reset();
	Log(TAG "Initialization completed.");

	return true;
}

bool Timer::Reset()
{
	IModule::Reset();
	t.start();

	return true;
}

bool Timer::Shutdown()
{
	this->Reset();
	IModule::Shutdown();

	return true;
}

u64 Timer::GetMilliseconds() const
{
	return static_cast<u64>(t.elapsed());
}

void Timer::Sleep(u32 ms) const
{
	UNUSED(ms);
	//QWaitCondition sleep;
	//sleep.wait(ms);
}

}} // namespace

#endif // BUILD_QT
