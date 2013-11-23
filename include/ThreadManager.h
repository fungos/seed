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

#ifndef __THREADMANAGER_H__
#define __THREADMANAGER_H__

#if (SEED_USE_THREAD == 0)

#include "interface/IManager.h"
#include "interface/IUpdatable.h"
#include "Singleton.h"
#include "Container.h"

namespace Seed {

class Thread;

/// Thread Manager is used when no threads are enabled to run threads in serial
class SEED_CORE_API ThreadManager : public IManager, public IUpdatable
{
	SEED_DECLARE_SINGLETON(ThreadManager)
	SEED_DECLARE_MANAGER(ThreadManager)
	SEED_DECLARE_CONTAINER(Vector, Thread)
	SEED_DISABLE_COPY(ThreadManager)

	public:
		void Add(Thread *thread);
		void Remove(Thread *thread);

		// IManager
		virtual bool Initialize() override;
		virtual bool Reset() override;
		virtual bool Shutdown() override;

		virtual void Disable() override;
		virtual void Enable() override;

		// IUpdatable
		virtual bool Update(Seconds dt) override;

	private:
		ThreadVector vThread;
		bool bEnabled : 1;
};

#define pThreadManager ThreadManager::GetInstance()

} // namespace

#endif // SEED_USE_THREAD

#endif // __THREADMANAGER_H__
