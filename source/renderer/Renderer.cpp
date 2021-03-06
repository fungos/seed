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

#include "renderer/Renderer.h"
#include "renderer/Camera.h"
#include "Log.h"
#include "SceneNode.h"
#include "RendererDevice.h"
#include "Profiler.h"
#include "Memory.h"

#include <algorithm>
#include <vector>

namespace Seed {

/// Scene object ascending predicate
struct SEED_CORE_API VisibleObjectAscendingPrioritySort
{
	bool operator()(VisibleObject const &left, VisibleObject const &right)
	{
		return (left.pObj->GetZ() < right.pObj->GetZ());
	}
};

/// Scene object descending predicate
struct SEED_CORE_API VisibleObjectDescendingPrioritySort
{
	bool operator()(VisibleObject const &left, VisibleObject const &right)
	{
		return (left.pObj->GetZ() > right.pObj->GetZ());
	}
};


Renderer::Renderer()
	: pScene(nullptr)
	, vRenderables()
	, vVisibleRenderables()
	, bEnabled(true)
{
}

Renderer::~Renderer()
{
	vRenderables.clear();
	RenderableVector().swap(vRenderables);

	vVisibleRenderables.clear();
	VisibleVector().swap(vVisibleRenderables);
}

void Renderer::PushChildNodes(SceneNode *node, SceneNodeVector &v)
{
	SEED_ASSERT(node);
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

bool Renderer::Update(Seconds dt)
{
	UNUSED(dt);
	SEED_FUNCTION_PROFILER;

	if (!this->IsEnabled() || !pScene)
		return false;

	vRenderables.clear();

	SceneNodeVector v;
	v.push_back(pScene);
	this->PushChildNodes(pScene, v);

	SceneNodeVectorIterator it = v.begin();
	SceneNodeVectorIterator end = v.end();
	for (; it != end; ++it)
	{
		SceneNode *node = (*it);
		ISceneObjectVector nv = node->vChild;

		ISceneObjectVectorIterator jit = nv.begin();
		ISceneObjectVectorIterator jend = nv.end();
		for (; jit != jend; ++jit)
		{
			ISceneObject *obj = (*jit);
			if (obj->IsVisible())
			{
				vRenderables += obj;
			}
		}
	}

	return true;
}

void Renderer::Render(Camera *camera)
{
	SEED_ASSERT(camera);
	SEED_FUNCTION_PROFILER;
	if (pScene && pRendererDevice && pRendererDevice->IsEnabled() && this->IsEnabled())
	{
		this->Culler(camera);

		pRendererDevice->SetCamera(camera);
		this->Begin();
			this->RenderObjects(vVisibleRenderables);
		this->End();
	}
}

void Renderer::Culler(Camera *camera)
{
	SEED_ASSERT(camera);
	SEED_FUNCTION_PROFILER;

	vVisibleRenderables.clear();

	VisibleObject visible;
	for (auto obj: vRenderables)
	{
//		Log("Culling: %s", obj->sName.c_str());
		SEED_ASSERT(obj);

		if (camera->Contains(obj, visible.mWorldTransform))
		{
//			Log("Passed");
			visible.pObj = obj;
			vVisibleRenderables.push_back(visible);
		}
	}

	this->Sort(vVisibleRenderables);
}

void Renderer::RenderObjects(const VisibleVector &vec) const
{
	ConstVisibleVectorIterator it = vec.begin();
	ConstVisibleVectorIterator end = vec.end();
	for (; it != end; ++it)
	{
		const VisibleObject *obj = &(*it);
//		Log("Obj: %s Z: %f", obj->pObj->sName.c_str(), obj->pObj->GetZ());
		obj->pObj->Render(obj->mWorldTransform);
	}
}

void Renderer::Sort(VisibleVector &vec)
{
	std::sort(vec.begin(), vec.end(), VisibleObjectDescendingPrioritySort());
}

void Renderer::Begin() const
{
	pRendererDevice->Begin();
}

void Renderer::End() const
{
	pRendererDevice->End();
}

Renderer *Renderer::Clone() const
{
	Renderer *r = sdNew(Renderer);
	SEED_ASSERT_MSG(r, "Could not instantiate Renderer");

	return r;
}

} // namespace
