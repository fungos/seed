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
#include "Enum.h"
#include "Viewport.h"
#include "ViewManager.h"
#include "Renderer.h"
#include "RendererManager.h"
#include "SceneNode.h"
#include "SceneManager.h"
#include "JobManager.h"
#include "EventJob.h"
#include "Screen.h"
#include "interface/IEventJobListener.h"
#include "interface/IEventPresentationListener.h"
#include "EventPresentation.h"

#define TAG "[Presentation] "

namespace Seed {

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

class RendererSceneLoader : public IEventJobListener
{
	friend class Presentation;
	public:
		RendererSceneLoader(Presentation *parent, ResourceManager *res, Renderer *renderer, u32 myId)
			: pParent(parent)
			, pRes(res)
			, pRenderer(renderer)
			, iId(myId)
		{
		}

		// IEventJobListener
		virtual void OnJobCompleted(const EventJob *ev)
		{
			switch (ev->GetName())
			{
				case kPresentationSceneLoaded:
				{
					FileLoader *job = (FileLoader *)ev->GetJob();
					Reader r(job->pFile);
					SceneNode *scene = New(SceneNode());
					scene->Load(r, pRes);
					Delete(job);

					pRenderer->SetScene(scene);
					pScene = scene;
					pParent->SceneLoaded(this);
				}
				break;
			}
		}

		virtual void OnJobAborted(const EventJob *ev)
		{
			Job *job = ev->GetJob();
			Delete(job);
		}

		Presentation *pParent;
		ResourceManager *pRes;
		Renderer *pRenderer;
		SceneNode *pScene;
		u32 iId;
};

Presentation::Presentation()
	: pListener(NULL)
	, pRes(NULL)
	, pFinished(NULL)
{
}

Presentation::~Presentation()
{
	this->Unload();
}

bool Presentation::Load(const String &filename, IEventPresentationListener *listener, ResourceManager *res)
{
	SEED_ASSERT(listener);
	pListener = listener;

	File f(filename);
	Reader r(&f);
	return this->Load(r, res);
}

bool Presentation::Load(Reader &reader, ResourceManager *res)
{
	pRes = res;
	bool ret = false;
	if (this->Unload())
	{
		sName = reader.ReadString("sName", "presentation");
		u32 rends = reader.SelectArray("aRenderer");
		SEED_ASSERT_MSG(rends, "At least one renderer is required.");
		if (rends)
		{
			for (u32 i = 0; i < rends; i++)
			{
				reader.SelectNext();

				String n = reader.ReadString("sName", "");
				SEED_ASSERT_MSG(!n.empty(), "Renderer requires a name - sName");

				String s = reader.ReadString("sScene", "");
				SEED_ASSERT_MSG(!s.empty(), "A scene file is required - sScene");

				Renderer *r = New(Renderer());
				r->sName = n;
				r->sSceneToAttach = s;

				Log(TAG "Renderer %s created.", n.c_str());
				vRenderer += r;
				pRendererManager->Add(r);
			}
			reader.UnselectArray();
		}

		u32 vps = reader.SelectArray("aViewport");
		SEED_ASSERT_MSG(vps, "At least one viewport is required.");
		if (vps)
		{
			for (u32 i = 0; i < vps; i++)
			{
				reader.SelectNext();

				String n = reader.ReadString("sName", "");
				SEED_ASSERT_MSG(!n.empty(), "Viewport requires a name - sName");

				u32 x = reader.ReadU32("iX", 0);
				u32 y = reader.ReadU32("iY", 0);
				u32 w = reader.ReadU32("iWidth", pScreen->GetWidth());
				u32 h = reader.ReadU32("iHeight", pScreen->GetHeight());

				String c = reader.ReadString("sCamera", "");
				SEED_ASSERT_MSG(!c.empty(), "Viewport requires a camera name reference - sCamera");

				String r = reader.ReadString("sRenderer", "");
				SEED_ASSERT_MSG(!r.empty(), "Viewport requires a renderer name reference - sRenderer");

				Renderer *rend = this->GetRendererByName(r);
				if (!rend)
				{
					Log(TAG "Could not find a renderer named %s for viewport %s", r.c_str(), n.c_str());
					SEED_ASSERT_MSG(!rend, "Renderer not found.");
				}

				Viewport *vp = New(Viewport());
				vp->SetArea(Rect4u(x, y, w, h));
				vp->SetRenderer(rend);
				vp->sName = n;
				vp->sCameraNameToAttach = c;

				Log(TAG "Viewport %s (at %dx%d size: %dx%d) created.", n.c_str(), x, y, w, h);
				vViewport += vp;
				pViewManager->Add(vp);
			}
			reader.UnselectArray();
		}

		pFinished = (bool *)Alloc(sizeof(bool) * vRenderer.Size());
		memset(pFinished, 0, sizeof(bool) * vRenderer.Size());

		// After all json parsing, we can start the scene loading jobs
		// so we guarantee that we have all our reference names
		RendererVectorIterator it = vRenderer.begin();
		RendererVectorIterator end = vRenderer.end();
		for (int  i = 0; it != end; ++it, ++i)
		{
			Renderer *obj = (*it);
			RendererSceneLoader *ldr = New(RendererSceneLoader(this, pRes, obj, i));
			Log(TAG "Scheduling scene job for scene %s.", obj->sSceneToAttach.c_str());
			pJobManager->Add(New(FileLoader(obj->sSceneToAttach, kPresentationSceneLoaded, ldr)));
		}

		ret = true;
	}

	return ret;
}

bool Presentation::Write(Writer &writer)
{
	UNUSED(writer)
	return true;
}

bool Presentation::Unload()
{
	sFree(pFinished);

	{
		ViewportVectorIterator it = vViewport.begin();
		ViewportVectorIterator end = vViewport.end();
		for (; it != end; ++it)
		{
			Viewport *obj = (*it);
			Log(TAG "Destroying viewport %s.", obj->sName.c_str());
			pViewManager->Remove(obj);
			Delete(obj);
		}
		vViewport.clear();
		ViewportVector().swap(vViewport);
	}

	{
		RendererVectorIterator it = vRenderer.begin();
		RendererVectorIterator end = vRenderer.end();
		for (; it != end; ++it)
		{
			Renderer *obj = (*it);
			Log(TAG "Destroying renderer %s.", obj->sName.c_str());
			pRendererManager->Remove(obj);
			Delete(obj);
		}
		vRenderer.clear();
		RendererVector().swap(vRenderer);
	}

	{
		SceneNodeVectorIterator it = vScenes.begin();
		SceneNodeVectorIterator end = vScenes.end();
		for (; it != end; ++it)
		{
			SceneNode *obj = (*it);
			Log(TAG "Destroying scene %s.", obj->sName.c_str());
			Delete(obj);
		}
		vScenes.clear();
		SceneNodeVector().swap(vScenes);
	}

	return true;
}

Renderer *Presentation::GetRendererByName(const String &name)
{
	RendererVectorIterator it = std::find_if(vRenderer.begin(), vRenderer.end(), FindRendererByName(name));
	return (*it);
}

Viewport *Presentation::GetViewportByName(const String &name)
{
	ViewportVectorIterator it = std::find_if(vViewport.begin(), vViewport.end(), FindViewportByName(name));
	return (*it);
}

void Presentation::SceneLoaded(RendererSceneLoader *ldr)
{
	{
		ViewportVectorIterator it = vViewport.begin();
		ViewportVectorIterator end = vViewport.end();
		for (; it != end; ++it)
		{
			Viewport *obj = (*it);
			if (obj->GetRenderer() == ldr->pRenderer)
			{
				Log(TAG "Scene %s finished loading.", ldr->pScene->sName.c_str());
				vScenes += ldr->pScene;
				Camera *cam = (Camera *)ldr->pScene->GetChildByName(obj->sCameraNameToAttach);
				obj->SetCamera(cam);

				pFinished[ldr->iId] = true;
			}
		}
	}

	vScenes.Unique();
	Delete(ldr);

	for (u32 i = 0; i < vRenderer.Size(); ++i)
	{
		if (!pFinished[i])
			return;
	}

	{
		SceneNodeVectorIterator it = vScenes.begin();
		SceneNodeVectorIterator end = vScenes.end();
		for (; it != end; ++it)
		{
			SceneNode *obj = (*it);
			Log(TAG "Adding scene %s to the scene manager.", obj->sName.c_str());
			pSceneManager->Add(obj);
		}

		if (pListener)
		{
			EventPresentation ev(this, NULL);
			pListener->OnPresentationLoaded(&ev);
		}
	}
}

void Presentation::SceneAborted(RendererSceneLoader *ldr)
{
	if (pListener)
	{
		EventPresentation ev(this, ldr->pRenderer);
		pListener->OnPresentationAborted(&ev);
	}
}

const String Presentation::GetClassName() const
{
	return "Presentation";
}

int Presentation::GetObjectType() const
{
	return Seed::TypeAnimation;
}

} // namespace

