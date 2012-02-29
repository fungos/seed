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

void *ResourceManager::mapLoaders[ObjectUser];

ResourceManager::ResourceManager(const String &name)
	: bHasUnusedResource(false)
	, sName(name)
{
	MEMSET(mapLoaders, 0, sizeof(mapLoaders));
	MEMSET(mapResources, 0, sizeof(mapResources));
}

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			LOG(TAG "WARNING: Some resources still allocated in '%s'.", sName.c_str());
			this->Print();
			break;
		}
	}

	this->Reset();
}

void ResourceManager::Reset()
{
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			String first = mapResources[i].sKey;

			LOG(TAG "Deallocating %s.", first);
			Delete(second);
		}
	}

	MEMSET(mapResources, 0, sizeof(mapResources));
}

IResource *ResourceManager::Get(const String &filename, Seed::eObjectType resourceType)
{
	IResource *res = NULL;

	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].sKey == filename)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			res = second;
			res->IncrementReference();
			//Log(TAG "Found allocated resource, incrementing reference counter to: %d.", res->GetReferenceCount());
			return res;
		}
	}

	if (mapLoaders[resourceType] != NULL)
	{
		pResourceLoaderFunc ldr = (pResourceLoaderFunc)mapLoaders[resourceType];
		res = (ldr)(filename, this);
		if (res)
		{
			this->Add(filename, res);
			res->IncrementReference();
		}
		else
		{
			Log(TAG "WARNING: Resource file '%s' not found - couldn't load.", filename);
		}
	}

	return res;
}

void ResourceManager::GarbageCollect()
{
#if defined(DEBUG)
	u64 begin = pTimer->GetMilliseconds();
	u64 end = 0;
#endif // DEBUG

	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			String first = mapResources[i].sKey;

			IResource *res = second;
			u32 r = res->GetReferenceCount();
			if (!r)
			{
				LOG(TAG "\tdeleting %s from [%s].", first.c_str(), sName.c_str());

				Delete(res);

				mapResources[i].pValue = NULL;
				mapResources[i].sKey = "";

				bHasUnusedResource = true;
			}
		}
	}

	if (bHasUnusedResource)
	{
		bHasUnusedResource = false;
		this->GarbageCollect();
	}

#if defined(DEBUG)
	end = pTimer->GetMilliseconds();
	LOG(TAG "Garbage collection done in %d milliseconds.",  (u32)(end - begin));
	LOG(TAG "Resources inside '%s': ", sName.c_str());
	this->Print();
#endif // DEBUG
}

void ResourceManager::Unload(Seed::eObjectType resourceType)
{
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			String first = mapResources[i].sKey;

			IResource *res = second;
			if (res->GetObjectType() == resourceType)
			{
				LOG(TAG "Unloading %s %s.", res->GetObjectName(), first);
				res->Unload();
			}
		}
	}
}

void ResourceManager::Reload(Seed::eObjectType resourceType)
{
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			String first = mapResources[i].sKey;

			IResource *res = second;
			if (res->GetObjectType() == resourceType)
			{
				LOG(TAG "Reloading %s %s.", res->GetObjectName(), first);
				res->Load(res->pFilename, res->pRes);
			}
		}
	}
}

void ResourceManager::Register(Seed::eObjectType resourceType, pResourceLoaderFunc pfunc)
{
	if (mapLoaders[resourceType])
	{
		LOG(TAG "This resource loader is already registered.");
		return;
	}

	mapLoaders[resourceType] = (void *)pfunc;
}

void ResourceManager::Unregister(Seed::eObjectType resourceType)
{
	if (mapLoaders[resourceType] == NULL)
	{
		LOG(TAG "Resource loader not found.");
		return;
	}

	mapLoaders[resourceType] = NULL;
}

void ResourceManager::Add(const String &filename, IResource *res)
{
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].sKey && mapResources[i].sKe == filename)
		{
			LOG(TAG "The resource %s already is allocated in '%s'.", filename, sName.c_str());
			return;
		}
		else if (mapResources[i].sKey == "")
		{
			LOG(TAG "Adding resource to position %d: %s: %s.", i, res->GetObjectName(), filename.c_str());
			mapResources[i].sKey = filename;
			mapResources[i].pValue = (void *)res;
			return;
		}
	}

	LOG(TAG "Reached resource limit for '%s'.", sName.c_str());
}

void ResourceManager::Remove(const String &filename)
{
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].sKey && mapResources[i].sKey == filename)
		{
			LOG(TAG "Removing resource '%s'.", filename.c_str());
			mapResources[i].sKey = "";
			mapResources[i].pValue = NULL;
			return;
		}
	}

	LOG(TAG "Resource %s not found in '%s'.", filename.c_str(), sName.c_str());
}

void ResourceManager::PrintUsedMemoryByResource()
{
	u32 total = 0;
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			String first = mapResources[i].sKey;

			IResource *res = second;

			Dbg(TAG "Resource: %s/%s Memory: %d References: %d Type: %s", first, res->sFilename.c_str(), res->GetUsedMemory(), res->GetReferenceCount(), res->GetObjectName());
			total += res->GetUsedMemory();
		}
	}

	Dbg(TAG "Total: %d", total);
}

u32 ResourceManager::GetTotalUsedMemory()
{
	u32 total = 0;
	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *second = (IResource *)mapResources[i].pValue;
			IResource *res = second;
			total += res->GetUsedMemory();
		}
	}

	return total;
}

void ResourceManager::Print()
{
	u32 cnt = 0;
	Log(TAG "Listing loaded resources in '%s':",sName.c_str());

	for (int i = 0; i < 100; i++)
	{
		if (mapResources[i].pValue != NULL)
		{
			IResource *res = (IResource *)mapResources[i].pValue;
			String name = mapResources[i].sKey;

			Log(TAG "\t%s [%s] [%d]", name.c_str(), res->GetObjectName(), res->GetReferenceCount());
			cnt++;
		}
	}
	Log(TAG "%s Total: %d resources.", sName.c_str(), cnt);
}

} // namespace
