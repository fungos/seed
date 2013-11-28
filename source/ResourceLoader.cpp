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
#include "System.h"
#include "Memory.h"

#define TAG		"[ResourceLoader] "

namespace Seed {

SEED_SINGLETON_DEFINE(ResourceLoader)

ResourceLoader::ResourceLoader()
	: vListeners()
	, vGroups()
{
}

ResourceLoader::~ResourceLoader()
{
}

bool ResourceLoader::Reset()
{
	IManager::Reset();
	return true;
}

bool ResourceLoader::Initialize()
{
	Log(TAG "Initializing...");
	IManager::Initialize();
	this->Create();
	Log(TAG "Initialization completed.");

	return true;
}

bool ResourceLoader::Shutdown()
{
	this->Destroy();
	IManager::Shutdown();
	Log(TAG "Terminated.");

	return true;
}

bool ResourceLoader::Update(Seconds dt)
{
	UNUSED(dt);

	if (!this->IsRunning())
		return false;

	ResourceGroup *group = nullptr;

	ScopedMutexLock lock(cMutex);
	if (vGroups.size() > 0)
	{
		group = (*vGroups.begin());
	}

	if (!group)
		return true;

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

	return true;
}

bool ResourceLoader::Run()
{
	if (this->IsRunning())
	{
		ResourceGroup *group = nullptr;

		ScopedMutexLock lock(cMutex);
		if (vGroups.size() > 0)
		{
			group = (*vGroups.begin());
		}

		if (!group)
			return true;

		if (group->IsLoaded())
			return true;

		if (group->Load())
			group->SetLoaded();
	}

	pTimer->Sleep(Milliseconds(10));
	return this->IsRunning();
}

void ResourceLoader::Add(ResourceGroup *group)
{
	if (this->IsRunning())
	{
		ScopedMutexLock lock(cMutex);
		vGroups.push_back(group);
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
	for (auto target: vListeners)
		target->OnGroupLoaded(ev);
}

void ResourceLoader::SendEventQueueEmpty(const EventResourceLoader *ev)
{
	for (auto target: vListeners)
		target->OnQueueEmpty(ev);
}

} // namespace
