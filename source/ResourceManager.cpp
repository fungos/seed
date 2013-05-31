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
#include "ResourceManager.h"
#include "Timer.h"

#define TAG		"[ResourceManager] "

#if defined(SEED_LOG_RESOURCEMANAGER)
	#define LOG		Log
#else
	#define LOG		_hid_rmlog
	void _hid_rmlog(const char *pMessage, ...);
	void _hid_rmlog(const char *pMessage, ...)
	{
		UNUSED(pMessage);
	}
#endif

namespace Seed {

LoaderMap ResourceManager::mapLoaders;

ResourceManager::ResourceManager(const String &name)
	: sName(name)
	, mapResources()
{
}

ResourceManager::~ResourceManager()
{
	if (mapResources.size())
	{
		Log(TAG "WARNING: Some resources still allocated in '%s'.", sName.c_str());
		this->Print();
	}

	this->Reset();
}

void ResourceManager::Reset()
{
	ResourceMapIterator it = mapResources.begin();
	ResourceMapIterator end = mapResources.end();
	for (; it != end; ++it)
	{
		LOG(TAG "Deallocating %s.", (*it).first.c_str());
		Delete((*it).second);
	}

	ResourceMap().swap(mapResources);
}

IResource *ResourceManager::Get(const String &filename, Seed::eObjectType resourceType)
{
	IResource *res = NULL;
    
	if (mapResources.find(filename) == mapResources.end())
	{
		Log(TAG "Resource %s not found in '%s'.", filename.c_str(), sName.c_str());
		LoaderMapIterator it = mapLoaders.find(resourceType);
		if (it == mapLoaders.end())
		{
			Log(TAG "Resource loader for %s not found.", filename.c_str());
			return NULL;
		}

		res = ((*it).second)(filename, this);

		if (res)
		{
			this->Add(filename, res);
		}
		else
		{
			Log(TAG "WARNING: Resource file '%s' not found - couldn't load.", filename.c_str());
		}
	}
	else
	{
		res = mapResources[filename];
	}

	if (res)
		res->IncrementReference();

	return res;
}

void ResourceManager::GarbageCollect()
{
#if defined(DEBUG)
	u64 tbegin = pTimer->GetMilliseconds();
	u64 tend = 0;
#endif // DEBUG

	u32 amount = 0;
	ResourceMapIterator it = mapResources.begin();
	while (it != mapResources.end())
	{
		IResource *res = (*it).second;
		u32 r = res->GetReferenceCount();
		LOG(TAG "> %s -> %d", res->GetFilename().c_str(), r);
		if (!r)
		{
			amount++;
			mapResources.erase(it++);
			Delete(res);
		}
		else
		{
			++it;
		}
	}

#if defined(DEBUG)
	tend = pTimer->GetMilliseconds();
	LOG(TAG "Garbage collection done in %d milliseconds, cleared %d objects.",  (u32)(tend - tbegin), amount);
	LOG(TAG "Resources inside '%s': ", sName.c_str());
	this->Print();
#endif // DEBUG
}

void ResourceManager::Unload(Seed::eObjectType resourceType)
{
	ResourceMapIterator it = mapResources.begin();
	ResourceMapIterator end = mapResources.end();
	for (; it != end; ++it)
	{
		IResource *res = (*it).second;

		if (res->GetObjectType() == resourceType)
		{
			Log(TAG "Unloading %s %s.", res->GetClassName().c_str(), (*it).first.c_str());
			res->Unload();
		}
	}
}

void ResourceManager::Reload(Seed::eObjectType resourceType)
{
	ResourceMapIterator it = mapResources.begin();
	ResourceMapIterator end = mapResources.end();
	for (; it != end; ++it)
	{
		IResource *res = (*it).second;

		if (res->GetObjectType() == resourceType)
		{
			Log(TAG "Reloading %s %s.", res->GetClassName().c_str(), (*it).first.c_str());
			res->Load(res->sFilename, res->pRes);
		}
	}
}

void ResourceManager::Register(Seed::eObjectType resourceType, pResourceLoaderFunc pfunc)
{
	if (mapLoaders.find(resourceType) != mapLoaders.end())
	{
		LOG(TAG "This resource loader is already registered.");
		return;
	}

	mapLoaders[resourceType] = pfunc;
}

void ResourceManager::Unregister(Seed::eObjectType resourceType)
{
	LoaderMapIterator it = mapLoaders.find(resourceType);

	if (it == mapLoaders.end())
	{
		LOG(TAG "Resource loader not found.");
		return;
	}

	mapLoaders.erase(it);
}

void ResourceManager::Add(const String &filename, IResource *res)
{
	if (mapResources.find(filename) != mapResources.end())
	{
		LOG(TAG "The resource %s already is allocated in '%s'.", filename.c_str(), sName.c_str());
		return;
	}

	mapResources[filename] = res;
}

void ResourceManager::Remove(const String &filename)
{
	ResourceMapIterator it = mapResources.find(filename);

	if (it == mapResources.end())
	{
		LOG(TAG "Resource %s not found in '%s'.", filename.c_str(), sName.c_str());
		return;
	}

	mapResources.erase(it);
}

void ResourceManager::PrintUsedMemoryByResource()
{
	u32 total = 0;

	ResourceMapIterator it = mapResources.begin();
	ResourceMapIterator end = mapResources.end();
	for (; it != end; ++it)
	{
		IResource *res = (*it).second;

		Dbg(TAG "Resource: %s Memory: %d References: %d Type: %s", res->sFilename.c_str(), res->GetUsedMemory(), res->GetReferenceCount(), res->GetClassName().c_str());
		total += res->GetUsedMemory();
	}

	Dbg(TAG "Total: %d", total);
}

u32 ResourceManager::GetTotalUsedMemory()
{
	u32 total = 0;

	ResourceMapIterator it = mapResources.begin();
	ResourceMapIterator end = mapResources.end();
	for (; it != end; ++it)
	{
		IResource *res = (*it).second;
		total += res->GetUsedMemory();
	}

	return total;
}

void ResourceManager::Print()
{
#if defined(DEBUG) && defined(SEED_LOG_RESOURCEMANAGER)
	u32 cnt = 0;
	Log(TAG "Listing %d loaded resources in '%s':", mapResources.size(), sName.c_str());

	ResourceMapIterator it = mapResources.begin();
	ResourceMapIterator end = mapResources.end();
	for (; it != end; ++it)
	{
		IResource *res = (*it).second;
		const String name = (*it).first;

		Log(TAG "\t%s [%s] [%d]", name.c_str(), res->GetClassName().c_str(), res->GetReferenceCount());
		cnt++;
	}

	Log(TAG "%s Total: %d resources.", sName.c_str(), cnt);
#endif // DEBUG
}

} // namespace
