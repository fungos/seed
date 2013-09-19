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

#include "JobManager.h"
#include "Defines.h"
#include "Log.h"
#include "Enum.h"
#include "Job.h"
#include "EventJob.h"
#include "interface/IEventJobListener.h"

#define TAG		"[JobManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(JobManager)

JobManager::JobManager()
{
}

JobManager::~JobManager()
{
}

bool JobManager::Initialize()
{
	pMutex = New(Mutex);
	return IManager::Initialize();
}

bool JobManager::Reset()
{
	while (vQueue.size())
	{
		auto job = vQueue.front();
		vQueue.pop();

		job->Abort();
	}

	for (auto job: vRunning)
		job->Abort();

	JobVector().swap(vRunning);

	return true;
}

bool JobManager::Shutdown()
{
	while (vQueue.size())
	{
		auto job = vQueue.front();
		vQueue.pop();

		job->Abort();
	}

	for (auto job: vRunning)
		job->Destroy();

	JobVector().swap(vRunning);
	Delete(pMutex);

	return IManager::Shutdown();
}

void JobManager::StartThreads()
{
	if (vQueue.empty())
		return;

	pMutex->Lock();
	while (vRunning.Size() < iMaxThreads && !vQueue.empty())
	{
		auto job = vQueue.front();
		vQueue.pop();

		vRunning += job;
		job->Create();
	}
	pMutex->Unlock();
}

bool JobManager::Update(f32 dt)
{
	UNUSED(dt)
	if (bEnabled)
	{
		this->StartThreads();
		auto list = vRunning;

		for (auto job: list)
		{
			switch (job->GetState())
			{
				case eJobState::Completed:
				case eJobState::Aborted:
				{
					vRunning -= job;
					job->OnFinished();
				}
				break;

				default:
				break;
			}
		}

		this->StartThreads();
	}

	return true;
}

Job *JobManager::Add(Job *job)
{
	pMutex->Lock();
	vQueue.push(job);
	pMutex->Unlock();
	return job;
}

void JobManager::Disable()
{
	bEnabled = false;
}

void JobManager::Enable()
{
	bEnabled = true;
}

} // namespace
