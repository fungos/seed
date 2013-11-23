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

#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defines.h"
#include <thread>

namespace Seed {

class SEED_CORE_API Timer
{
	public:
		Timer()
		{
			mStart = Clock::now();
		}

		~Timer()
		{
		}

		Timer(const Timer &t)
			: mStart(t.mStart)
		{
		}

		Timer(Timer &&t)
			: mStart(std::move(t.mStart))
		{
			mStart = TimePoint{};
		}

		Timer &operator=(const Timer &t)
		{
			mStart = t.mStart;
			return *this;
		}

		Timer &operator=(Timer &&t)
		{
			mStart = std::move(t.mStart);
			return *this;
		}

		Milliseconds GetMilliseconds() const
		{
			auto now = Clock::now();
			return std::chrono::duration_cast<std::chrono::duration<Milliseconds, std::milli>>(now - mStart).count();
		}

		Seconds GetSeconds() const
		{
			auto now = Clock::now();
			return std::chrono::duration_cast<std::chrono::duration<Seconds>>(now - mStart).count();
		}

		void Sleep(u32 ms) const
		{
			this->Sleep(Milliseconds(ms));
		}

		void Sleep(Duration ms) const
		{
			std::this_thread::sleep_for(ms);
		}

		void Reset()
		{
			mStart = Clock::now();
		}

	public:
		TimePoint mStart;
};

} // namespace

#endif // __TIMER_H__
