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

#include "Log.h"
#include "ResourceLoader.h"
#include "ResourceGroup.h"
#include "EventResourceLoader.h"
#include "interface/IEventResourceLoaderListener.h"
#include "Timer.h"

#define TAG		"[ResourceLoader] "

namespace Seed {

SEED_SINGLETON_DEFINE(ResourceLoader)

ResourceLoader::ResourceLoader()
	: vListeners()
	, vGroups()
	, bRunning(false)
	, pMutex(NULL)
{
}

ResourceLoader::~ResourceLoader()
{
}

bool ResourceLoader::Reset()
{
	IModule::Reset();
	return true;
}

bool ResourceLoader::Initialize()
{
	Log(TAG "Initializing...");
	IModule::Initialize();
	pMutex = New(Mutex());
	this->Create();
	bRunning = true;
	Log(TAG "Initialization completed.");

	return true;
}

bool ResourceLoader::Shutdown()
{
	bRunning = false;
	this->Destroy();
	Delete(pMutex);
	IModule::Shutdown();
	Log(TAG "Terminated.");

	return true;
}

bool ResourceLoader::Update(f32 dt)
{
	UNUSED(dt);

	if (!bRunning)
		return false;

	ResourceGroup *group = NULL;

	pMutex->Lock();
	if (vGroups.size() > 0)
	{
		group = (*vGroups.begin());
	}
	pMutex->Unlock();

	if (!group)
		return true;

	pMutex->Lock();
	if (group->IsLoaded())
	{
		EventResourceLoader ev;
		ev.SetGroup(group);
		SendEventGroupLoaded(&ev);

		group->Unload();

		GroupIterator p = std::find(vGroups.begin(), vGroups.end(), group);
		if (p != vGroups.end())
			vGroups.erase(p);

		if (vGroups.size() == 0)
		{
			vGroups.clear();
			Vector<ResourceGroup *>().swap(vGroups);
		}

		//glResourceManager.Print();
	}
	pMutex->Unlock();

	return true;
}

bool ResourceLoader::Run()
{
	bool ret = Thread::Run();
	if (ret)
	{
		ResourceGroup *group = NULL;

		pMutex->Lock();
		if (vGroups.size() > 0)
		{
			group = (*vGroups.begin());
		}
		pMutex->Unlock();

		if (!group)
			return true;

		if (group->IsLoaded())
			return true;

		if (group->Load())
		{
			pMutex->Lock();
			group->SetLoaded();
			pMutex->Unlock();
		}

		ret = bRunning;
	}

	pTimer->Sleep(10);
	return ret;
}

void ResourceLoader::Add(ResourceGroup *group)
{
	if (bRunning)
	{
		pMutex->Lock();
		vGroups.push_back(group);
		pMutex->Unlock();
	}
}

void ResourceLoader::AddListener(IEventResourceLoaderListener *listener)
{
	vListeners += listener;
}

void ResourceLoader::RemoveListener(IEventResourceLoaderListener *listener)
{
	SEED_ASSERT(listener);
	ListenerIterator p = std::find(vListeners.begin(), vListeners.end(), listener);

	if (p != vListeners.end())
		vListeners.erase(p);

	if (!vListeners.size())
	{
		vListeners.clear();
		ListenerVector().swap(vListeners);
	}
}

void ResourceLoader::SendEventGroupLoaded(const EventResourceLoader *ev)
{
	SEED_ASSERT(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventResourceLoaderListener *target = (*it);
		SEED_ASSERT(target);
		target->OnGroupLoaded(ev);
	}
}

void ResourceLoader::SendEventQueueEmpty(const EventResourceLoader *ev)
{
	SEED_ASSERT(ev);
	ListenerIterator it = vListeners.begin();
	ListenerIterator end = vListeners.end();

	for (; it != end; ++it)
	{
		IEventResourceLoaderListener *target = (*it);
		SEED_ASSERT(target);
		target->OnQueueEmpty(ev);
	}
}

const String ResourceLoader::GetClassName() const
{
	return "ResourceLoader";
}

} // namespace
