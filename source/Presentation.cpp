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

#include "Presentation.h"
#include "renderer/RendererManager.h"
#include "renderer/Renderer.h"
#include "renderer/Viewport.h"
#include "Enum.h"
#include "ViewManager.h"
#include "SceneNode.h"
#include "SceneManager.h"
#include "JobManager.h"
#include "Screen.h"
#include "PrefabManager.h"
#include "Memory.h"

#define TAG "[Presentation] "

namespace Seed {

class FindViewportByName
{
	public:
		FindViewportByName(const String &name)
			: sName(name)
		{}

		bool operator()(const Viewport *v) const
		{
			return (v->sName == sName);
		}

	private:
		String sName;
};

enum
{
	kPresentationSceneLoaded
};

class SceneFileLoader : public FileLoader
{
	SEED_DISABLE_COPY(SceneFileLoader)

	friend class Presentation;
	public:
		SceneFileLoader(u32 unique, const String &filename, JobCallback fun)
			: FileLoader(filename, fun)
			, iId(unique)
		{
		}

		virtual ~SceneFileLoader() {}

		void SetPresentation(Presentation *parent)
		{
			pParent = parent;
		}

		void SetResourceManager(ResourceManager *res)
		{
			pRes = res;
		}

		void SetViewport(Viewport *viewport)
		{
			pViewport = viewport;
		}

	protected:
		Presentation *pParent = nullptr;
		ResourceManager *pRes = nullptr;
		Viewport *pViewport = nullptr;
		SceneNode *pScene = nullptr;
		u32 iId = 0;
};

class PrefabFileLoader : public FileLoader
{
	SEED_DISABLE_COPY(PrefabFileLoader)

	friend class Presentation;
	public:
		PrefabFileLoader(u32 unique, const String &filename, JobCallback fun)
			: FileLoader(filename, fun)
			, iId(unique)
		{
		}

		virtual ~PrefabFileLoader() {}

		void SetResourceManager(ResourceManager *res)
		{
			pRes = res;
		}

	protected:
		ResourceManager *pRes = nullptr;
		u32 iId = 0;
};

Presentation::Presentation()
	: pRes(nullptr)
	, pFinishedScenes(nullptr)
{
}

Presentation::~Presentation()
{
	this->Unload();
}

bool Presentation::Load(const String &filename, Callback cb, ResourceManager *res)
{
	fnCallback = cb;

	// FIXME: ASYNC
	File f(filename);
	Reader r(&f);
	return this->Load(r, res);
}

bool Presentation::Load(Reader &reader, ResourceManager *res)
{
	pRes = res;

	if (!this->Unload())
		return false;

	sName = reader.ReadString("sName", "presentation");
	SEED_CHECK_RETURN(pRendererManager->Load(reader, res), false, "Could not load renderers");

	auto vps = reader.SelectArray("aViewport");
	SEED_CHECK_RETURN(vps, false, "At least one viewport is required,'aViewport'' is empty or inexistent");
	if (vps)
	{
		for (u32 i = 0; i < vps; i++)
		{
			reader.SelectNext();

			String n = reader.ReadString("sName", "");
			SEED_CHECK_RETURN(!n.empty(), false, "Viewport requires a name - sName");

			u32 x = reader.ReadU32("iX", 0);
			u32 y = reader.ReadU32("iY", 0);
			u32 w = reader.ReadU32("iWidth", pScreen->GetWidth());
			u32 h = reader.ReadU32("iHeight", pScreen->GetHeight());

			String c = reader.ReadString("sCamera", "");
			SEED_CHECK_RETURN(!c.empty(), false, "Viewport '%s' requires a camera 'sCamera' name reference", n.c_str());

			String s = reader.ReadString("sScene", "");
			SEED_CHECK_RETURN(!s.empty(), false, "Viewport '%s' requires a scene 'sScene'", n.c_str());

			String r = reader.ReadString("sRenderer", "");
			SEED_CHECK_RETURN(!r.empty(), false, "Viewport '%s' requires a renderer 'sRenderer' name reference", n.c_str());

			auto rend = pRendererManager->Get(r);
			SEED_CHECK_RETURN(rend, false, "Could not find a renderer named %s for viewport %s", r.c_str(), n.c_str());

			Renderer *clone = rend->Clone();
			SEED_ASSERT_MSG(clone, "Could not clone Renderer");
			vRenderer += clone;

			Viewport *vp = sdNew(Viewport);
			SEED_ASSERT_MSG(vp, "Could not instantiate Viewport");

			vp->SetArea(Rect4u(x, y, w, h));
			vp->SetRenderer(clone);
			vp->sName = n;
			vp->sCameraNameToAttach = c;
			vp->sSceneToAttach = s;

			Log(TAG "Viewport %s (at %dx%d size: %dx%d) created.", n.c_str(), x, y, w, h);
			vViewport += vp;
			pViewManager->Add(vp);
		}
		reader.UnselectArray();
	}

	pFinishedScenes = (bool *)sdAlloc(sizeof(bool) * vRenderer.Size());
	SEED_ASSERT_MSG(pFinishedScenes, "Could instantiate pFinishedScenes");
	memset(pFinishedScenes,  0, sizeof(bool) * vRenderer.Size());

	String prefabs = reader.ReadString("sPrefabs", "");
	if (!prefabs.empty())
	{
		this->PrefabsPhase(prefabs);
	}
	else
	{
		this->ScenesPhase();
	}

	return true;
}

bool Presentation::Write(Writer &writer)
{
	UNUSED(writer)
	WARNING(IMPL - MapLayerTiled::Write(...))
	return true;
}

bool Presentation::Unload()
{
	sdFree(pFinishedScenes);

	for (auto obj: vViewport)
	{
		Log(TAG "Destroying viewport %s.", obj->sName.c_str());
		pViewManager->Remove(obj);
		sdDelete(obj);
	}
	vViewport.clear();
	ViewportVector().swap(vViewport);

	for (auto obj: vRenderer)
	{
		Log(TAG "Destroying renderer %s.", obj->sName.c_str());
		pRendererManager->Remove(obj);
		sdDelete(obj);
	}
	vRenderer.clear();
	RendererVector().swap(vRenderer);

	for (auto obj: vScenes)
	{
		Log(TAG "Destroying scene %s.", obj->sName.c_str());
		sdDelete(obj);
	}
	vScenes.clear();
	SceneNodeVector().swap(vScenes);

	return true;
}

Renderer *Presentation::GetRendererByName(const String &name)
{
	RendererVectorIterator it = std::find_if(vRenderer.begin(), vRenderer.end(), FindRendererByName(name));
	if (it == vRenderer.end())
		return nullptr;

	return (*it);
}

Viewport *Presentation::GetViewportByName(const String &name)
{
	ViewportVectorIterator it = std::find_if(vViewport.begin(), vViewport.end(), FindViewportByName(name));
	if (it == vViewport.end())
		return nullptr;

	return (*it);
}

void Presentation::SceneLoaded(SceneFileLoader *ldr)
{
	SEED_ASSERT(ldr);
	SEED_ASSERT(ldr->pScene);
	SEED_ASSERT(pFinishedScenes);
	for (auto obj: vViewport)
	{
		if (obj == ldr->pViewport)
		{
			Camera *cam = (Camera *)ldr->pScene->GetChildByName(obj->sCameraNameToAttach);
			obj->SetCamera(cam);

			// So we do not add dupes in vScenes
			if (!pFinishedScenes[ldr->iId])
			{
				Log(TAG "Scene %s finished loading.", ldr->pScene->sName.c_str());
				vScenes += ldr->pScene;
				pFinishedScenes[ldr->iId] = true;
			}
		}
	}

	for (u32 i = 0; i < vRenderer.Size(); ++i)
	{
		if (!pFinishedScenes[i])
			return;
	}

	for (auto obj: vScenes)
	{
		Log(TAG "Adding scene %s to the scene manager.", obj->sName.c_str());
		pSceneManager->Add(obj);
	}

	if (fnCallback)
		fnCallback(this, nullptr);
}

void Presentation::SceneAborted(SceneFileLoader *ldr)
{
	if (fnCallback)
		fnCallback(this, ldr->pViewport);
}


void Presentation::PrefabLoaded(PrefabFileLoader *ldr)
{
	SEED_ASSERT(ldr);
	Log(TAG "Prefab file '%s' finished loading.", ldr->sFilename.c_str());
	this->GotoScenePhase();
}

void Presentation::PrefabAborted(PrefabFileLoader *ldr)
{
	SEED_ASSERT(ldr);
	Log(TAG "Prefab file '%s' failed loading.", ldr->sFilename.c_str());
	this->GotoScenePhase();
}

void Presentation::GotoScenePhase()
{
	this->ScenesPhase();
}

void Presentation::PrefabsPhase(const String &prefabs)
{
	auto cb = [&](Job *self) {
		auto job = static_cast<PrefabFileLoader *>(self);

		if (job->GetState() == eJobState::Completed)
		{
			Reader r(job->pFile);
			pPrefabManager->Load(r, job->pRes);

			this->PrefabLoaded(job);
		}
		else if (job->GetState() == eJobState::Aborted)
		{
			this->PrefabAborted(job);
		}
		sdDelete(self);
	};

	auto ldr = sdNew(PrefabFileLoader(0, prefabs, cb));
	SEED_ASSERT_MSG(ldr, "Could instantiate PrefabFileLoader");
	ldr->SetResourceManager(pRes);

	Log(TAG "Scheduling prefab loading job: %s.", prefabs.c_str());
	pJobManager->Add(ldr);
}

void Presentation::ScenesPhase()
{
	// After all json parsing, we can start the scene loading jobs
	// so we guarantee that we have all our reference names
	auto i = u32{0};
	for (auto obj: vViewport)
	{
		auto cb = [&](Job *self) {
			auto job = static_cast<SceneFileLoader *>(self);
			if (job->GetState() == eJobState::Completed)
			{
				auto scene = sdNew(SceneNode);
				SEED_ASSERT_MSG(scene, "Could instantiate SceneNode");
				scene->bMarkForDeletion = true;

				Reader r(job->pFile);
				scene->Load(r, job->pRes);

				job->pViewport->SetScene(scene);
				job->pScene = scene;
				pRendererManager->Add(job->pViewport->pRenderer);
				job->pParent->SceneLoaded(job);
			}
			else if (job->GetState() == eJobState::Aborted)
			{
				job->pParent->SceneAborted(job);
			}
			sdDelete(self);
		};

		auto ldr = sdNew(SceneFileLoader(i, obj->sSceneToAttach, cb));
		SEED_ASSERT_MSG(ldr, "Could instantiate SceneFileLoader");
		ldr->SetResourceManager(pRes);
		ldr->SetViewport(obj);
		ldr->SetPresentation(this);

		Log(TAG "Scheduling scene job for scene %s.", obj->sSceneToAttach.c_str());
		pJobManager->Add(ldr);

		++i;
	}
}

} // namespace

