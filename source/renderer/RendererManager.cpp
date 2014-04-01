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

#include "Defines.h"
#include "Log.h"
#include "Enum.h"
#include "Memory.h"
#include "renderer/RendererManager.h"
#include "renderer/Renderer.h"

#define TAG		"[RendererManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(RendererManager)

class FindRendererByName
{
	public:
		FindRendererByName(const String &name)
			: sName(name)
		{}

		bool operator()(const Renderer *r) const
		{
			return (r->sName == sName);
		}

	private:
		String sName;
};


RendererManager::RendererManager()
	: vTemplates()
	, vRenderer()
	, bEnabled(true)
{
}

RendererManager::~RendererManager()
{
	this->Reset();
}

void RendererManager::Add(Renderer *renderer)
{
	vRenderer += renderer;
	vRenderer.Unique();
}

void RendererManager::Remove(Renderer *renderer)
{
	vRenderer -= renderer;
}

bool RendererManager::Load(Reader &reader, ResourceManager *res)
{
	UNUSED(res);

	auto rends = reader.SelectArray("aRenderer");
	SEED_CHECK_RETURN(rends, false, "At least one renderer is required, 'aRenderer' is empty or inexistent");

	for (u32 i = 0; i < rends; i++)
	{
		reader.SelectNext();

		String n = reader.ReadString("sName", "");
		SEED_CHECK_RETURN(!n.empty(), false, "Renderer requires a name - sName");
/*
		String s = reader.ReadString("aPasses", "");
		SEED_CHECK_RETURN(!s.empty(), false, "An array of passes 'aPasses' is required in Renderer '%s'", n.c_str());
*/
		Renderer *r = sdNew(Renderer);
		SEED_ASSERT_MSG(r, "Could not instantiate Renderer");
		r->sName = n;

		Log(TAG "Renderer %s created.", n.c_str());
		vTemplates += r;
	}
	reader.UnselectArray();
	return true;
}

bool RendererManager::Write(Writer &writer)
{
	UNUSED(writer)
	WARNING(IMPL - MapLayerTiled::Write(...))
			return false;
}

const Renderer *RendererManager::Get(const String &name) const
{
	auto it = std::find_if(vTemplates.begin(), vTemplates.end(), FindRendererByName(name));
	SEED_CHECK_RETURN(it != vTemplates.end(), nullptr, "Could not find Renderer '%s'", name);
	return (*it);
}

bool RendererManager::Initialize()
{
	IManager::Initialize();

	for (auto each: vRenderer)
		each->Setup();

	return true;
}

bool RendererManager::Reset()
{
	for (auto each: vTemplates)
		sdDelete(each);
	RendererVector().swap(vTemplates);

	RendererVector().swap(vRenderer);
	return true;
}

bool RendererManager::Shutdown()
{
	for (auto each: vRenderer)
		each->Teardown();

	for (auto each: vTemplates)
		sdDelete(each);
	RendererVector().swap(vTemplates);

	return IManager::Shutdown();
}

bool RendererManager::Update(Seconds dt)
{
	bool ret = true;

	if (bEnabled)
	{
		for (auto each: vRenderer)
			ret &= each->Update(dt);
	}

	return ret;
}

void RendererManager::Disable()
{
	bEnabled = false;

	for (auto each: vRenderer)
		each->Disable();
}

void RendererManager::Enable()
{
	bEnabled = true;

	for (auto each: vRenderer)
		each->Enable();
}

} // namespace
