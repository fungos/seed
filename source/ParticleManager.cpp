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

#include "ParticleManager.h"
#include "ParticleEmitter.h"

namespace Seed {

SEED_SINGLETON_DEFINE(ParticleManager)

ParticleManager::ParticleManager()
	: vEmitter()
	, bPaused(false)
	, bStopped(false)
{
}

ParticleManager::~ParticleManager()
{
	this->Reset();
}

bool ParticleManager::Initialize()
{
	IModule::Initialize();

	return true;
}

bool ParticleManager::Reset()
{
	this->Kill();

	return true; // abstract IModule::Reset();
}

bool ParticleManager::Shutdown()
{
	this->Reset();
	return IModule::Shutdown();;
}

void ParticleManager::Simulate(u32 iNumLoops)
{
	for (u32 i = 0; i < iNumLoops; i++)
	{
		this->Update(0.01f);
	}
}

bool ParticleManager::Update(f32 dt)
{
	UNUSED(dt);
	/*if (bEnabled && !(bPaused || bStopped))
	{
		for (u32 i = 0; i < vEmitter.Size(); i++)
		{
			ParticleEmitter *p = vEmitter[i];
			ASSERT_NULL(p);

			if (!p->IsPaused())
				p->Update(dt);
		}
	}
	*/
	return true;
}

void ParticleManager::Kill()
{
	ForEach(ParticleEmitterVector, vEmitter,
	{
		(*it)->Kill();
	});

	ParticleEmitterVector().swap(vEmitter);
}

void ParticleManager::Stop()
{
	ForEach(ParticleEmitterVector, vEmitter,
	{
		(*it)->Stop();
	});

	bPaused = false;
	bStopped = true;
}

bool ParticleManager::IsStopped() const
{
	return bStopped;
}

void ParticleManager::Pause()
{
	ForEach(ParticleEmitterVector, vEmitter,
	{
		(*it)->Pause();
	});

	bPaused = true;
}

bool ParticleManager::IsPaused() const
{
	return bPaused;
}

void ParticleManager::Play()
{
	ForEach(ParticleEmitterVector, vEmitter,
	{
		(*it)->Play();
	});

	bPaused = false;
	bStopped = false;
}

bool ParticleManager::IsPlaying() const
{
	return !(bPaused || bStopped);
}

void ParticleManager::Disable()
{
	IModule::Disable();
	ForEach(ParticleEmitterVector, vEmitter,
	{
		(*it)->Disable();
	});
}

void ParticleManager::Enable()
{
	IModule::Enable();
	ForEach(ParticleEmitterVector, vEmitter,
	{
		(*it)->Enable();
	});
}

void ParticleManager::Add(ParticleEmitter *emitter)
{
	vEmitter +=  emitter;
}

void ParticleManager::Remove(ParticleEmitter *emitter)
{
	vEmitter -= emitter;
}

const char *ParticleManager::GetObjectName() const
{
	return "ParticleEmitter";
}

int ParticleManager::GetObjectType() const
{
	return Seed::ObjectParticleManager;
}

} // namespace
