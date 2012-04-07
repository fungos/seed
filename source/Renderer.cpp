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

#include "Renderer.h"
#include "Log.h"
#include "SceneNode.h"
#include "RendererDevice.h"
#include "Profiler.h"

#include <algorithm>
#include <vector>

namespace Seed {

bool PredicateIsNodeAndVisible(SceneNode *obj)
{
	return (obj->IsNode() && obj->IsVisible());
}

bool PredicateIsVisible(ISceneObject *obj)
{
	return (obj->IsVisible());
}

Renderer::Renderer()
	: vScenes()
	, vRenderables()
	, vVisibleRenderables()
{
}

Renderer::~Renderer()
{
	vScenes.clear();
	SceneNodeVector().swap(vScenes);

	vRenderables.clear();
	RenderableVector().swap(vRenderables);

	vVisibleRenderables.clear();
	RenderableVector().swap(vVisibleRenderables);
}

void Renderer::PushChildNodes(SceneNode *node, SceneNodeVector &v)
{
	for (u32 i = 0; i < node->Size(); i++)
	{
		ISceneObject *obj = node->GetChildAt(i);
		if (obj->IsNode() && obj->IsVisible())
		{
			v.push_back(static_cast<SceneNode *>(obj));
			this->PushChildNodes(static_cast<SceneNode *>(obj), v);
		}
	}
}

bool Renderer::Update(f32 dt)
{
	UNUSED(dt);
	SEED_FUNCTION_PROFILER;

	if (!IModule::IsEnabled())
		return false;

	vRenderables.clear();

	SceneNodeVector v(vScenes);

	SceneNodeVectorIterator sit = v.begin();
	SceneNodeVectorIterator send = v.end();
	for (; sit != send; ++sit)
	{
		this->PushChildNodes((*sit), v);
	}

	SceneNodeVectorIterator it = v.begin();
	SceneNodeVectorIterator end = v.end();
	for (; it != end; ++it)
	{
		SceneNode *node = (*it);
		ISceneObjectVector nv = node->vChild;

		ISceneObjectVectorIterator it = nv.begin();
		ISceneObjectVectorIterator end = nv.end();
		for (; it != end; ++it)
		{
			ISceneObject *obj = (*it);
			if (obj->IsVisible())
			{
				vRenderables += obj;
			}
		}
	}

	return true;
}

void Renderer::DrawRect(f32 x, f32 y, f32 w, f32 h, uPixel color, bool fill) const
{
	UNUSED(x);
	UNUSED(y);
	UNUSED(w);
	UNUSED(h);
	UNUSED(color);
	UNUSED(fill);
	SEED_ABSTRACT_METHOD;
}

void Renderer::Render()
{
	SEED_FUNCTION_PROFILER;
	if (pRendererDevice && pRendererDevice->IsEnabled() && IModule::IsEnabled())
	{
		this->Culler();

		this->Begin();
			this->RenderObjects(vRenderables); //vVisibleRenderables);
		this->End();
	}
}

void Renderer::RenderObjects(const RenderableVector &vec) const
{
	ConstRenderableVectorIterator it = vec.begin();
	ConstRenderableVectorIterator end = vec.end();

	for (; it != end; ++it)
	{
		ISceneObject *obj = const_cast<ISceneObject *>(*it);
		SEED_ASSERT(obj);

		obj->Render();
	}
}

// FIXME: Culler(SceneNode, CullingOperation)
void Renderer::Culler()
{
	SEED_FUNCTION_PROFILER;
	this->Sort(vRenderables);
/*
	vVisibleRenderables.clear();

	ConstRenderableVectorIterator it = vRenderables.begin();
	ConstRenderableVectorIterator end = vRenderables.end();

	for (; it != end; ++it)
	{
		ISceneObject *obj = const_cast<ISceneObject *>(*it);
		SEED_ASSERT(obj);

		if (obj->IsVisible())
			vVisibleRenderables.push_back(obj);
	}

	this->Sort(vVisibleRenderables);
*/
}

void Renderer::Sort(RenderableVector &vec)
{
#if !SEED_ENABLE_DEPTH_TEST
	std::sort(vec.begin(), vec.end(), ISceneObjectAscendingPrioritySort());
#else
	UNUSED(vec)
#endif
}

void Renderer::Begin() const
{
	pRendererDevice->Begin();
}

void Renderer::End() const
{
	pRendererDevice->End();
}

void Renderer::Add(SceneNode *node)
{
	vScenes += node;
}

void Renderer::Remove(SceneNode *node)
{
	vScenes -= node;
}

const String Renderer::GetObjectName() const
{
	return "Renderer";
}

} // namespace
