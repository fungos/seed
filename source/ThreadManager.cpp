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

#if (SEED_USE_THREAD == 0)

#include "ThreadManager.h"
#include "Defines.h"
#include "Log.h"
#include "Enum.h"
#include "interface/IThread.h"

#define TAG		"[ThreadManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(ThreadManager)

ThreadManager::ThreadManager()
	: vThread()
	, bEnabled(true)
{
}

ThreadManager::~ThreadManager()
{
	this->Reset();
}

bool ThreadManager::Initialize()
{
	return IManager::Initialize();
}

bool ThreadManager::Reset()
{
	IThreadVector().swap(vThread);
	return true;
}

bool ThreadManager::Shutdown()
{
	this->Reset();
	return IManager::Shutdown();
}

bool ThreadManager::Update(f32 dt)
{
	UNUSED(dt)
	if (bEnabled)
	{
		IThreadVector completed;

		for (auto each: vThread)
		{
			if (!each->Run())
			{
				completed += each;
			}
		}

		for (auto each: completed)
			vThread -= each;
	}

	return true;
}

void ThreadManager::Add(IThread *obj)
{
	vThread += obj;
}

void ThreadManager::Remove(IThread *obj)
{
	vThread -= obj;
}

void ThreadManager::Disable()
{
	bEnabled = false;
}

void ThreadManager::Enable()
{
	bEnabled = true;
}

} // namespace

#endif // SEED_USE_THREAD
