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

#include "ViewManager.h"
#include "Defines.h"
#include "Log.h"
#include "Enum.h"
#include "RendererDevice.h"
#include "renderer/Viewport.h"
#include "renderer/Renderer.h"

#define TAG		"[ViewManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(ViewManager)

ViewManager::ViewManager()
	: vViewport()
	, pCurrentViewport(nullptr)
	, bEnabled(true)
{
}

ViewManager::~ViewManager()
{
	this->Reset();
}

bool ViewManager::Initialize()
{
	IManager::Initialize();

	for (auto each: vViewport)
	{
		each->GetRenderer()->Setup(); // redundante?
	}

	return true;
}

bool ViewManager::Reset()
{
	ViewportVector().swap(vViewport);
	return true;
}

bool ViewManager::Shutdown()
{
	for (auto each: vViewport)
	{
		each->GetRenderer()->Teardown(); // redundante?
	}

	this->Reset();

	return IManager::Shutdown();
}

void ViewManager::Add(Viewport *view)
{
	vViewport += view;
	vViewport.Unique();
}

void ViewManager::Remove(Viewport *view)
{
	vViewport -= view;
}

void ViewManager::Disable()
{
	bEnabled = false;
}

void ViewManager::Enable()
{
	bEnabled = true;
}

void ViewManager::Render()
{
	if (bEnabled)
	{
		pRendererDevice->BackbufferClear();

		for (auto each: vViewport)
			each->Render();
	}

	pCurrentViewport = nullptr;
}

Renderer *ViewManager::GetCurrentRenderer() const
{
	SEED_ASSERT_MSG(pCurrentViewport, TAG "GetCurrentRenderer must be called within Render call.");
	return pCurrentViewport->GetRenderer();
}


Viewport *ViewManager::GetCurrentViewport() const
{
	SEED_ASSERT_MSG(pCurrentViewport, TAG "GetCurrentViewport must be called within Render call.");
	return pCurrentViewport;
}

Viewport *ViewManager::GetViewportAt(u32 x, u32 y)
{
	Viewport *ret = nullptr;
	if (bEnabled)
	{
		for (auto each: vViewport)
		{
			if (each->Contains(x, y))
			{
				ret = each;
				break;
			}
		}
	}

	return ret;
}

} // namespace
