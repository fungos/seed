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

#if defined(BUILD_IOS)

#include "Defines.h"
#include "Screen.h"
#include "Log.h"
#include "RendererDevice.h"
#include "platform/ios/iosoneView.h"

#define FADE_OUT_COLOR  0xff
#define FADE_OUT_SOLID  0xff
#define FADE_OUT_TRANS	0x00

#if defined(DEBUG)
#define FADE_INCREMENT	0x10
#else
#define FADE_INCREMENT	0x10
#endif // DEBUG

#define TAG "[Screen] "

namespace Seed { namespace iOS {

SEED_SINGLETON_DEFINE(Screen);

Screen::Screen()
	: IScreen()
	, iFadeStatus(0)
	, fadeType(FADE_IN)
	, iModeHeight(0)
	, iModeWidth(0)
{
	this->Reset();
}

Screen::~Screen()
{
	this->Reset();
}

bool Screen::Reset()
{
	bFading    	= false;
	iFadeStatus 	= FADE_OUT_TRANS;

	return true;
}

bool Screen::Initialize()
{
	Log(TAG "Initializing...");

	nMode = pConfiguration->GetVideoMode();
	if (nMode == Seed::Video_iPad)
	{
		iWidth = iModeWidth = 1024;
		iHeight = iModeHeight = 768;
	}

	this->Reset();


	Log(TAG "Initialization completed.");

	return true;
}

bool Screen::Shutdown()
{
	Log(TAG "Terminating...");

	bool r = this->Reset();

	Log(TAG "Terminated.");

	return r;
}

void Screen::Update()
{
	//this->SwapSurfaces();
}

void Screen::FadeOut()
{
	if (bFading)
		return;

	bFading		= true;
	fadeType	= FADE_OUT;
	iFadeStatus	= FADE_OUT_TRANS;
}

void Screen::FadeIn()
{
	if (bFading)
		return;

	bFading		= true;
	fadeType	= FADE_IN;
	iFadeStatus	= FADE_OUT_SOLID;
}

void Screen::CancelFade()
{
	if (!bFading)
		return;

	bFading		= false;
	iFadeStatus	= FADE_OUT_TRANS;
}

void Screen::SwapSurfaces()
{
	//glFlush();
}

void Screen::Resize(int w, int h)
{
	if (w > h)
	{
		//iModeWidth = h;
	   // iModeHeight = w;
		iModeWidth = w;
		iModeHeight = h;
		nMode = Seed::Video_iOSLandscape;
	}
	else
	{
		iModeWidth = w;
		iModeHeight = h;
		nMode = Seed::Video_iOSPortrait;
	}

	iHeight = iModeHeight;
	iWidth = iModeWidth;
	fAspectRatio = (f32)iHeight / (f32)iWidth;

	Info(TAG "Video resolution is %dx%d.", iModeWidth, iModeHeight);
}

void Screen::ApplyFade()
{
	if (bFading == false)
		return;

	if (fadeType == FADE_IN)
	{
		iFadeStatus -= FADE_INCREMENT;

		if (iFadeStatus <= FADE_OUT_TRANS)
		{
			bFading = false;
			iFadeStatus = FADE_OUT_TRANS;
		}
	}
	else
	{
		iFadeStatus += FADE_INCREMENT;

		if (iFadeStatus >= FADE_OUT_SOLID)
		{
			bFading = false;
			iFadeStatus = FADE_OUT_SOLID;
		}
	}

	u8 c = static_cast<u8>(iFadeStatus & 0xff);
	pRendererDevice->BackbufferFill(PIXEL_COLOR(0u, 0u, 0u, c));
}

}} // namespace

#endif // BUILD_IOS
