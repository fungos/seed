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

/*! \file RendererManager.cpp
	\author	Everton Fernando Patitucci da Silva
	\brief Renderer Manager
*/

#include "RendererManager.h"
#include "Defines.h"
#include "Log.h"
#include "Enum.h"
#include "Renderer.h"

#define TAG		"[RendererManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(RendererManager)

RendererManager::RendererManager()
	: arRenderer()
	, bEnabled(true)
{
	arRenderer.Truncate();
}

RendererManager::~RendererManager()
{
	this->Reset();
}

bool RendererManager::Initialize()
{
	IModule::Initialize();

	for (u32 i = 0; i < arRenderer.Size(); i++)
	{
		arRenderer[i]->Initialize();
	}

	return true;
}

bool RendererManager::Reset()
{
	arRenderer.Truncate();
	return true;
}

bool RendererManager::Shutdown()
{
	for (u32 i = 0; i < arRenderer.Size(); i++)
	{
		arRenderer[i]->Shutdown();
	}

	return IModule::Shutdown();
}

bool RendererManager::Update(f32 dt)
{
	bool ret = true;

	if (bEnabled)
	{
		for (u32 i = 0; i < arRenderer.Size(); i++)
		{
			ret &= arRenderer[i]->Update(dt);
		}
	}

	return ret;
}

void RendererManager::Add(Renderer *renderer)
{
	ASSERT_NULL(renderer);

	bool found = false;
	for (u32 i = 0; i < arRenderer.Size(); i++)
	{
		if (arRenderer[i] == renderer)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		arRenderer.Add();
		arRenderer[arRenderer.Size() - 1] = renderer;
	}
}

void RendererManager::Remove(Renderer *renderer)
{
	ASSERT_NULL(renderer);
	arRenderer.Remove(renderer);
}

void RendererManager::Disable()
{
	bEnabled = false;
	for (u32 i = 0; i < arRenderer.Size(); i++)
	{
		arRenderer[i]->Disable();
	}
}

void RendererManager::Enable()
{
	bEnabled = true;
	for (u32 i = 0; i < arRenderer.Size(); i++)
	{
		arRenderer[i]->Enable();
	}
}

const char *RendererManager::GetObjectName() const
{
	return "RendererManager";
}

int RendererManager::GetObjectType() const
{
	return Seed::ObjectRendererManager;
}

} // namespace
