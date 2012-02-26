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

#include "ViewManager.h"
#include "Defines.h"
#include "Log.h"
#include "Enum.h"
#include "Viewport.h"
#include "RendererDevice.h"
#include "Renderer.h"

#define TAG		"[ViewManager] "

namespace Seed {

SEED_SINGLETON_DEFINE(ViewManager)

ViewManager::ViewManager()
	: vViewport()
	, pCurrentViewport(NULL)
	, bEnabled(true)
{
}

ViewManager::~ViewManager()
{
	this->Reset();
}

bool ViewManager::Initialize()
{
	IModule::Initialize();

	ForEach(ViewportVector, vViewport,
	{
		(*it)->GetRenderer()->Initialize();
	});

	return true;
}

bool ViewManager::Reset()
{
	ViewportVector().swap(vViewport);
	return true;
}

bool ViewManager::Shutdown()
{
	ForEach(ViewportVector, vViewport,
	{
		(*it)->GetRenderer()->Shutdown();
	});

	this->Reset();

	return IModule::Shutdown();
}

void ViewManager::Add(Viewport *view)
{
	vViewport += view;
}

void ViewManager::Remove(Viewport *view)
{
	vViewport -= view;
}

void ViewManager::Disable()
{
	bEnabled = false;
}

void ViewManager::Enable()
{
	bEnabled = true;
}

void ViewManager::Render()
{
	if (bEnabled)
	{
		pRendererDevice->BackbufferClear();
		ForEach(ViewportVector, vViewport,
		{
			pCurrentViewport = (*it);
			pCurrentViewport->Render();
		});
	}

	pCurrentViewport = NULL;
}

Renderer *ViewManager::GetCurrentRenderer() const
{
	ASSERT_MSG(pCurrentViewport, TAG "GetCurrentRenderer must be called within Render call.");
	return pCurrentViewport->GetRenderer();
}


Viewport *ViewManager::GetCurrentViewport() const
{
	ASSERT_MSG(pCurrentViewport, TAG "GetCurrentViewport must be called within Render call.");
	return pCurrentViewport;
}

Viewport *ViewManager::GetViewportAt(f32 x, f32 y)
{
	Viewport *ret = NULL;
	if (bEnabled)
	{
		ForEach(ViewportVector, vViewport,
		{
			if ((*it)->Contains(x, y))
			{
				ret = (*it);
				break;
			}
		});
	}

	return ret;
}


const char *ViewManager::GetObjectName() const
{
	return "ViewManager";
}

int ViewManager::GetObjectType() const
{
	return Seed::ObjectViewManager;
}

} // namespace
