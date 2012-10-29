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

#if defined(BUILD_IOS)

#include "Defines.h"
#include "Screen.h"
#include "Log.h"
#include "RendererDevice.h"

#include <UIKit/UIKit.h>
#include <CoreGraphics/CoreGraphics.h>
//#include "platform/ios/iosoneView.h"

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

    CGRect screenBounds = [[UIScreen mainScreen] bounds];
	iWidth = iModeWidth = screenBounds.size.width;
	iHeight = iModeHeight = screenBounds.size.height;

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
		iModeWidth = w;
		iModeHeight = h;
		//nMode = Seed::Video_iOSLandscape;
	}
	else
	{
		iModeWidth = w;
		iModeHeight = h;
		//nMode = Seed::Video_iOSPortrait;
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
	pRendererDevice->BackbufferFill(Color(0u, 0u, 0u, c));
}

}} // namespace

#endif // BUILD_IOS
