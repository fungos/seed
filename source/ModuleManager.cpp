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

#include "ModuleManager.h"
#include "interface/IModule.h"

#define TAG "[ModuleManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(ModuleManager)

ModuleManager::ModuleManager()
	: vModule()
{
}

ModuleManager::~ModuleManager()
{
	IModuleVector().swap(vModule);
}

bool ModuleManager::Add(IModule *obj)
{
	vModule += obj;
	bool ret = obj->Initialize();

	if (!ret)
	{
		if (obj->IsRequired())
		{
			Info(TAG "CRITICAL: Module '%s' failed to initialize.", obj->GetObjectName().c_str());
		}
		else
		{
			Info(TAG "WARNING: Module '%s' failed to initalize.", obj->GetObjectName().c_str());
			ret = true; // we can continue as this module isn't critical.
		}
	}

	return ret;
}

bool ModuleManager::Remove(IModule *obj)
{
	vModule -= obj;
	return obj->Shutdown();
}

void ModuleManager::Disable(const char *moduleName)
{
	UNUSED(moduleName);
}

void ModuleManager::Enable(const char *moduleName)
{
	UNUSED(moduleName);
}

bool ModuleManager::IsEnabled(const char *moduleName)
{
	UNUSED(moduleName);
	return true;
}

bool ModuleManager::Initialize()
{
	bool ret = true;

	IModuleVectorIterator it = vModule.begin();
	IModuleVectorIterator end = vModule.end();
	for (; it != end; ++it)
	{
		IModule *obj = (*it);
		ret = ret && (obj->Initialize() || !obj->IsRequired());
	}

	return ret;
}

bool ModuleManager::Reset()
{
	bool ret = true;

	IModuleVectorIterator it = vModule.begin();
	IModuleVectorIterator end = vModule.end();
	for (; it != end; ++it)
	{
		IModule *obj = (*it);
		ret = ret && (obj->Reset() || !obj->IsRequired());
	}

	return ret;
}

bool ModuleManager::Shutdown()
{
	bool ret = true;

	u32 len = vModule.Size() - 1;
	for (s32 i = len; i >= 0; i--)
	{
		IModule *obj = vModule[i];
		ret = ret && (obj->Shutdown() || !obj->IsRequired());
	}

	return ret;
}

void ModuleManager::Print()
{
	Info(TAG "Listing current modules:");

	IModuleVectorIterator it = vModule.begin();
	IModuleVectorIterator end = vModule.end();
	for (; it != end; ++it)
	{
		IModule *obj = (*it);
		Info(TAG "\tModule: %s.", obj->GetObjectName().c_str());
	}
}

} // namespace
