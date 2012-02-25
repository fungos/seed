/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file ResourceManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief Resource Manager
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
	: bHasUnusedResource(FALSE)
	, sName(name)
	, mapResources()
{
}

ResourceManager::~ResourceManager()
{
	if (mapResources.size())
	{
		LOG(TAG "WARNING: Some resources still allocated in '%s'.", sName.c_str());
		this->Print();
	}

	this->Reset();
}

void ResourceManager::Reset()
{
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

	for (; it != itEnd; ++it)
	{
		LOG(TAG "Deallocating %s.", (*it).first.c_str());
		Delete((*it).second);
	}

	mapResources.clear();
	ResourceMap().swap(mapResources);
}

IResource *ResourceManager::Get(const String &filename, Seed::eObjectType resourceType)
{
	IResource *res = NULL;

	if (mapResources.find(filename) == mapResources.end())
	{
		LOG(TAG "Resource %s not found in '%s'.", filename.c_str(), sName.c_str());
		LoaderIterator it = mapLoaders.find(resourceType);
		if (it == mapLoaders.end())
		{
			LOG(TAG "Resource loader for %s not found.", filename.c_str());
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
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

#if defined(DEBUG)
	u64 begin = pTimer->GetMilliseconds();
	u64 end = 0;
#endif // DEBUG

	for (; it != itEnd;)
	{
		IResource *res = (*it).second;

		u32 r = res->GetReferenceCount();
		if (!r)
		{
			mapResources.erase(it++);
			Delete(res);

			bHasUnusedResource = TRUE;
		}
		else
		{
			++it;
		}
	}

	if (bHasUnusedResource)
	{
		bHasUnusedResource = FALSE;
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
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

	for (; it != itEnd; ++it)
	{
		IResource *res = (*it).second;

		if (res->GetObjectType() == resourceType)
		{
			LOG(TAG "Unloading %s %s.", res->GetObjectName(), (*it).first.c_str());
			res->Unload();
		}
	}
}

void ResourceManager::Reload(Seed::eObjectType resourceType)
{
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

	for (; it != itEnd; ++it)
	{
		IResource *res = (*it).second;

		if (res->GetObjectType() == resourceType)
		{
			LOG(TAG "Reloading %s %s.", res->GetObjectName(), (*it).first.c_str());
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
	LoaderIterator it = mapLoaders.find(resourceType);

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
	ResourceIterator it = mapResources.find(filename);

	if (it == mapResources.end())
	{
		LOG(TAG "Resource %s not found in '%s'.", filename.c_str(), sName.c_str());
		return;
	}

	mapResources.erase(it);
}

void ResourceManager::PrintUsedMemoryByResource()
{
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

	u32 total = 0;
	for (; it != itEnd; ++it)
	{
		IResource *res = (*it).second;

		Dbg(TAG "Resource: %s Memory: %d References: %d Type: %s", res->sFilename.c_str(), res->GetUsedMemory(), res->GetReferenceCount(), res->GetObjectName());
		total += res->GetUsedMemory();
	}

	Dbg(TAG "Total: %d", total);
}

u32 ResourceManager::GetTotalUsedMemory()
{
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

	u32 total = 0;
	for (; it != itEnd; ++it)
	{
		IResource *res = (*it).second;

		total += res->GetUsedMemory();
	}

	return total;
}

void ResourceManager::Print()
{
#if defined(DEBUG) && defined(SEED_LOG_RESOURCEMANAGER)
	ResourceIterator it = mapResources.begin();
	ResourceIterator itEnd = mapResources.end();

	u32 cnt = 0;
	Log(TAG "Listing %d loaded resources in '%s':", mapResources.size(), sName.c_str());
	for (; it != itEnd; ++it)
	{
		IResource *res = (*it).second;
		const String name = (*it).first;

		Log(TAG "\t%s [%s] [%d]", name.c_str(), res->GetObjectName(), res->GetReferenceCount());
		cnt++;
	}

	Log(TAG "%s Total: %d resources.", sName.c_str(), cnt);
#endif // DEBUG
}

} // namespace
