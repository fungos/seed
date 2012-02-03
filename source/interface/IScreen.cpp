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

/*! \file IScreen.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Screen class interface
*/

#include "interface/IScreen.h"
#include "Log.h"

namespace Seed {

IScreen::IScreen()
	: bFading(FALSE)
	, nMode(Seed::Video_AutoDetect)
	, fAspectRatio(static_cast<f32>(PLATFORM_MAX_SCREEN_HEIGHT)/static_cast<f32>(PLATFORM_MAX_SCREEN_WIDTH))
	, iHeight(PLATFORM_MAX_SCREEN_HEIGHT)
	, iWidth(PLATFORM_MAX_SCREEN_WIDTH)
{
}

IScreen::~IScreen()
{
}

bool IScreen::IsFading() const
{
	return bFading;
}

void IScreen::SetMode(eVideoMode mode)
{
	nMode = mode;
}

eVideoMode IScreen::GetMode() const
{
	return nMode;
}

void IScreen::ToggleFullscreen()
{
	SEED_ABSTRACT_METHOD;
}

bool IScreen::HasWindowedMode() const
{
	SEED_ABSTRACT_METHOD;
	return FALSE;
}

bool IScreen::IsFullscreen() const
{
	SEED_ABSTRACT_METHOD;
	return TRUE;
}

void IScreen::Update()
{
	SEED_ABSTRACT_METHOD;
}

u32 IScreen::GetHeight() const
{
	return iHeight;
}

u32 IScreen::GetWidth() const
{
	return iWidth;
}

f32 IScreen::GetAspectRatio() const
{
	return fAspectRatio;
}

void IScreen::FadeIn()
{
}

void IScreen::FadeOut()
{
}

bool IScreen::IsRequired() const
{
	return TRUE;
}

const char *IScreen::GetObjectName() const
{
	return "Screen";
}

} // namespace

