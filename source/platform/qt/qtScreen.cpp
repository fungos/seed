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

#if defined(BUILD_QT)

#include "Defines.h"
#include "Screen.h"
#include "System.h"
#include "Log.h"

/*
##if !defined(GL_MULTISAMPLE)
#define GL_MULTISAMPLE  0x809D
#endif // GL_MULTISAMPLE
*/

#define TAG "[Screen] "

namespace Seed { namespace QT {

SEED_SINGLETON_DEFINE(Screen)

Scene *Screen::pScene = NULL;

Screen::Screen()
{
	this->Reset();
}

Screen::~Screen()
{
	this->Reset();
}

bool Screen::Reset()
{
	IModule::Reset();
	iFadeStatus = 0;

	return true;
}

void Screen::PrepareMode()
{
	iWidth = 800;
	iHeight = 600;
	iFlags = 0;
	iBPP = 32;

	switch (nMode)
	{
		case Video_480x272:
		{
			iWidth = 480;
			iHeight = 272;
		}
		break;

		case Video_iPhonePortrait:
		{
			iWidth = 320;
			iHeight = 480;
		}
		break;

		case Video_iPhoneLandscape:
		case Video_iPhoneLandscapeGoofy:
		case Video_480x320:
		{
			iWidth = 480;
			iHeight = 320;
		}
		break;

		case Video_NintendoWii:
		case Video_640x480:
		{
			iWidth = 640;
			iHeight = 480;
		}
		break;

		case Video_AutoDetect:
		case Video_800x600:
		{
			iWidth = 800;
			iHeight = 600;
		}
		break;

		case Video_1024x768:
		{
			iWidth = 1024;
			iHeight = 768;
		}
		break;

		default:
			Info(TAG "Invalid video mode, using 800x600.");
		break;
	}
}

bool Screen::Initialize()
{
	Log(TAG "Initializing...");
	IModule::Initialize();
	this->PrepareMode();

	if (!pScene)
		pScene = New(Scene(iWidth, iHeight));

	this->Reset();
	this->CreateHardwareSurfaces();

	Log(TAG "Video resolution is %dx%dx%d.", iWidth, iHeight, iBPP);
	Log(TAG "Initialization completed.");

	bFullScreen = false;
	bFading = false;

	return true;
}

bool Screen::Shutdown()
{
	Log(TAG "Terminating...");
	bool r = this->Reset();

	Delete(pScene);
	pScene = NULL;
	IModule::Shutdown();
	Log(TAG "Terminated.");

	return r;
}

void Screen::Update()
{
	//IScreen::Update(); // abstract
	this->pScene->update();
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
	bFading		= false;
	iFadeStatus	= FADE_OUT_TRANS;
}

void Screen::SwapSurfaces()
{
}

void Screen::CreateHardwareSurfaces()
{
}

void Screen::DestroyHardwareSurfaces()
{
}

u32 Screen::GetHeight() const
{
	return this->iHeight;
}

u32 Screen::GetWidth() const
{
	return this->iWidth;
}

void Screen::ToggleFullscreen()
{
	// destroy opengl textures
	// change video mode
	// reconfigure opengl context
	// reload textures
}

void Screen::ApplyFade()
{
	if (this->bFading == false)
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

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_QUADS);
		glVertex3i(-1, -1, -1);
		glVertex3i(1, -1, -1);
		glVertex3i(1, 1, -1);
		glVertex3i(-1, 1, -1);
	glEnd();

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	u8 c = static_cast<u8>(iFadeStatus & 0xff);

	const GLfloat vertices[] =
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor4ub(255, 255, 0, c);
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
}

void *Screen::GetSurface() const
{
	return this->pScene;
}

}} // namespace

#endif // BUILD_QT