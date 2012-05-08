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

#include "SceneObjectFactory.h"
#include "Log.h"
#include "interface/ISceneObject.h"

#define TAG		"[SceneObjectFactory] "

namespace Seed {

FactoryMap SceneObjectFactory::mapFactory;

SEED_SINGLETON_DEFINE(SceneObjectFactory)

SceneObjectFactory::SceneObjectFactory()
{
}

SceneObjectFactory::~SceneObjectFactory()
{
}

void SceneObjectFactory::Register(const String &objectType, pSceneObjectFactoryFunc pfunc)
{
	String type = objectType;
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);

	if (mapFactory.find(type) != mapFactory.end())
	{
		Log(TAG "This object factory is already registered.");
		return;
	}

	mapFactory[type] = pfunc;
}

void SceneObjectFactory::Unregister(const String &objectType)
{
	String type = objectType;
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);

	FactoryMapIterator it = mapFactory.find(type);

	if (it == mapFactory.end())
	{
		Log(TAG "Object factory not found.");
		return;
	}

	mapFactory.erase(it);
}

ISceneObject *SceneObjectFactory::Load(Reader &reader, ResourceManager *res) const
{
	String type = reader.ReadString("sType", "");
	SEED_ASSERT_MSG(type != "", "Object without type.");

	ISceneObject *obj = this->Create(type);
	SEED_ASSERT_MSG(obj != NULL, "Object type invalid.");

	obj->Load(reader, res);

	return obj;
}

ISceneObject *SceneObjectFactory::Create(const String &objectType) const
{
	ISceneObject *obj = NULL;
	String type = objectType;
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);

	if (mapFactory.find(type) == mapFactory.end())
		Log(TAG "Factory %s not found.", objectType.c_str());

	obj = mapFactory[type]();
	SEED_ASSERT_MSG(obj != NULL, "Couldn't create the object.");

	obj->bFromFactory = true;

	return obj;
}

} // namespace
