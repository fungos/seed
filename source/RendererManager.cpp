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

#include "RendererManager.h"
#include "Defines.h"
#include "Log.h"
#include "Enum.h"
#include "Renderer.h"

#define TAG		"[RendererManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(RendererManager)

RendererManager::RendererManager()
	: vRenderer()
	, bEnabled(true)
{
}

RendererManager::~RendererManager()
{
	this->Reset();
}

bool RendererManager::Initialize()
{
	IModule::Initialize();

	ForEach(RendererVector, vRenderer,
	{
		(*it)->Initialize();
	});

	return true;
}

bool RendererManager::Reset()
{
	RendererVector().swap(vRenderer);
	return true;
}

bool RendererManager::Shutdown()
{
	ForEach(RendererVector, vRenderer,
	{
		(*it)->Shutdown();
	});

	return IModule::Shutdown();
}

bool RendererManager::Update(f32 dt)
{
	bool ret = true;

	if (bEnabled)
	{
		ForEach(RendererVector, vRenderer,
		{
			ret &= (*it)->Update(dt);
		});
	}

	return ret;
}

void RendererManager::Add(Renderer *renderer)
{
	vRenderer += renderer;
}

void RendererManager::Remove(Renderer *renderer)
{
	vRenderer -= renderer;
}

void RendererManager::Disable()
{
	bEnabled = false;
	ForEach(RendererVector, vRenderer,
	{
		(*it)->Disable();
	});
}

void RendererManager::Enable()
{
	bEnabled = true;
	ForEach(RendererVector, vRenderer,
	{
		(*it)->Enable();
	});
}

const char *RendererManager::GetObjectName() const
{
	return "RendererManager";
}

int RendererManager::GetObjectType() const
{
	return Seed::ObjectRendererManager;
}

} // namespace
