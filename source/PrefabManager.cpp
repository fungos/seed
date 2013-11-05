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
#include "PrefabManager.h"
#include "Timer.h"
#include "interface/ISceneObject.h"
#include "interface/IDataObject.h"
#include "SceneObjectFactory.h"
#include "Memory.h"

#define TAG		"[PrefabManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(PrefabManager)

PrefabManager::PrefabManager()
	: mapPrefabs()
{
}

PrefabManager::~PrefabManager()
{
	this->Reset();
}

void PrefabManager::Reset()
{
	cLock.Lock();
	for (auto each: mapPrefabs)
	{
		Log(TAG "Deallocating %s.", each.first.c_str());
		IObject *obj = reinterpret_cast<IObject *>(each.second);

		pLeakReport->Debug(obj);
		sdDelete(obj);
		pLeakReport->Debug(obj);
	}

	PrefabMap().swap(mapPrefabs);
	cLock.Unlock();
}

IDataObject *PrefabManager::Get(const String &name)
{
	cLock.Lock();
	if (mapPrefabs.find(name) == mapPrefabs.end())
	{
		Log(TAG "Prefab %s not found.", name.c_str());
		cLock.Unlock();

		return nullptr;
	}

	auto r = mapPrefabs[name];
	cLock.Unlock();

	return r;
}

void PrefabManager::Add(IDataObject *obj)
{
	cLock.Lock();
	if (mapPrefabs.find(obj->sName) != mapPrefabs.end())
	{
		Log(TAG "WARNING: The prefab %s already is allocated.", obj->sName.c_str());
		cLock.Unlock();

		return;
	}

	auto k = obj->sName;
	mapPrefabs[k] = obj;
	cLock.Unlock();
}

void PrefabManager::Remove(IDataObject *obj)
{
	cLock.Lock();
	Remove(obj->sName);
	cLock.Unlock();
}

void PrefabManager::Remove(const String &name)
{
	cLock.Lock();
	PrefabMapIterator it = mapPrefabs.find(name);

	if (it == mapPrefabs.end())
	{
		Log(TAG "Prefab %s not found.", name.c_str());
		cLock.Unlock();

		return;
	}

	mapPrefabs.erase(it);
	cLock.Unlock();
}

void PrefabManager::Print()
{
	cLock.Lock();

	auto cnt = u32{0};
	Log(TAG "Listing %d loaded prefabs:", mapPrefabs.size());

	for (auto each: mapPrefabs)
	{
		auto obj = each.second;
		const auto name = each.first;

		Log(TAG "\t%s [%s]", name.c_str(), obj->GetTypeName());
		cnt++;
	}
	cLock.Unlock();

	Log(TAG "Total: %d prefabs.", cnt);
}

void PrefabManager::Load(Reader &reader, ResourceManager *res)
{
	auto objs = reader.SelectArray("aObjects");
	if (objs)
	{
		for (decltype(objs) i = 0; i < objs; i++)
		{
			reader.SelectNext();
			auto obj = pSceneObjectFactory->Load(reader, res, true);
			this->Add(static_cast<IDataObject *>(obj));
		}
		reader.UnselectArray();
	}
}

} // namespace
