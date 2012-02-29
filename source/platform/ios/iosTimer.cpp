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
#include "Timer.h"
#include "Log.h"

#include <time.h>
#include <sys/time.h>

#define TAG "[Timer] "

namespace Seed { namespace iOS {

SEED_SINGLETON_DEFINE(Timer);

Timer::Timer()
	: fStart(0)
{
}

Timer::~Timer()
{
}

bool Timer::Initialize()
{
	Log(TAG "Initializing...");
	this->Reset();
	Log(TAG "Initialization completed.");

	return true;
}

bool Timer::Reset()
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	fStart = (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL);

	//fStart = CFAbsoluteTimeGetCurrent();

	return true;
}

bool Timer::Shutdown()
{
	return this->Reset();
}

u64 Timer::GetMilliseconds() const
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	u64 ms = (tv.tv_usec / 1000ULL);
	u64 x =  (tv.tv_sec * 1000ULL);
	u64 ret = x + ms - fStart;
	return ret;

	//return CFAbsoluteTimeGetCurrent() - fStart;
}

void Timer::Sleep(u32 ms) const
{
	UNUSED(ms);
}

}} // namespace

#endif // BUILD_IOS
