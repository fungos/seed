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

#if defined(BUILD_GLFW)

#include "Screen.h"
#include "Log.h"
#include "RendererDevice.h"
#include "SeedInit.h"
#include "Configuration.h"

#ifdef WIN32
#pragma push_macro("Delete")
#pragma push_macro("Free")
#undef Free
#undef Delete
#include <io.h>
#include <windows.h>
#include <winuser.h>
#include <tchar.h>
#pragma pop_macro("Free")
#pragma pop_macro("Delete")
#endif

#define TAG "[Screen] "

namespace Seed { namespace GLFW {

SEED_SINGLETON_DEFINE(Screen)

Screen::Screen()
	: iHandle(0)
	, bFullScreen(false)
	, iBPP(32)
	, iFlags(0)
{
}

Screen::~Screen()
{
}

bool Screen::Reset()
{
#if defined(__linux__)
	this->InitializeVideo();
#else
	pResourceManager->Unload(Seed::TypeTexture);
	pRendererDevice->Shutdown();
	this->Shutdown();
	this->Initialize();
	pRendererDevice->Initialize();
	pResourceManager->Reload(Seed::TypeTexture);
#endif

	return true;
}

void Screen::Prepare()
{
	GLFWvidmode list[64];
	GLFWvidmode desk;
	glfwGetDesktopMode(&desk);
	int amount = glfwGetVideoModes(list, 64);

	Info(TAG "Video Modes:");
	Info(TAG "\t%dx%d (%d%d%d) [Desktop]", desk.Width, desk.Height, desk.RedBits, desk.GreenBits, desk.BlueBits);
	for (int i = 0; i < amount; i++)
	{
		Info(TAG "\t%dx%d (%d%d%d)", list[i].Width, list[i].Height, list[i].RedBits, list[i].GreenBits, list[i].BlueBits);
	}

	u32 reqW = pConfiguration->GetResolutionWidth();
	u32 reqH = pConfiguration->GetResolutionHeight();
	if (reqW && reqH)
	{
		iWidth = reqW;
		iHeight = reqH;
	}
	else
	{
		iWidth = desk.Width;
		iHeight = desk.Height;
		iBPP = desk.RedBits + desk.GreenBits + desk.BlueBits;
	}

	fAspectRatio = (f32)iHeight / (f32)iWidth;
	iFlags = 0;

	if (bFullScreen)
		iFlags |= GLFW_FULLSCREEN;
	else
		iFlags |= GLFW_WINDOW;
}

bool Screen::Initialize()
{
	Log(TAG "Initializing...");

	bFullScreen = pConfiguration->GetFullScreen();

	bFading = false;
	iFadeStatus = 16;
	this->Prepare();
	if (!this->InitializeVideo())
		return false;

	Info(TAG "Video resolution is %dx%dx%d.", iWidth, iHeight, iBPP);
	Log(TAG "Initialization completed.");
	return true;
}

bool Screen::InitializeVideo()
{
	int depthbits = 8;
	int stencilbits = 8;

	if (!glfwOpenWindow(iWidth, iHeight, 8, 8, 8, 8, depthbits, stencilbits, iFlags))
	{
		Log(TAG "ERROR: Failed to initialize screen.");
		Log(TAG "Initialization failed.");
		return false;
	}

	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	glfwSwapInterval(1); // v-sync on

#if defined(WIN32)
	int dpiX = 0, dpiY = 0;
	HDC hdc = GetDC(NULL);
	if (hdc)
	{
		dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
		dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
		ReleaseDC(NULL, hdc);
	}

	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);
	int scaleX = MulDiv(cxScreen, 96, dpiX);
	int scaleY = MulDiv(cyScreen, 96, dpiY);

	Info(TAG "Desktop DPI: %dx%d", dpiX, dpiY);
	Info(TAG "Desktop Resolution: %dx%d", cxScreen, cyScreen);
	Info(TAG "Desktop DPI scaled resolution: %dx%d", scaleX, scaleY);

	if (cxScreen <= 1024 && cyScreen <= 768 && iWidth > 800 && iHeight > 600)
	{
		iWidth = 800;
		iHeight = 600;
	}
#endif

	glfwSetWindowTitle(pConfiguration->GetApplicationTitle().c_str());

	return true;
}

bool Screen::Shutdown()
{
	Log(TAG "Terminating...");

	iFadeStatus = 0;
	nFadeType	= kFadeIn;
	iHeight		= 0;
	iWidth		= 0;
	iBPP		= 32;
	iFlags		= 0;

	glfwCloseWindow();

	Log(TAG "Terminated.");

	return true;
}

void Screen::Update()
{
	this->SwapSurfaces();
	pRendererDevice->Update();
}

void Screen::SwapSurfaces()
{
	glfwSwapBuffers();
}

void Screen::EnableCursor(bool b)
{
	pConfiguration->EnableCursor(b);
	if (b)
		glfwEnable(GLFW_MOUSE_CURSOR);
	else
		glfwDisable(GLFW_MOUSE_CURSOR);
}

void Screen::ToggleCursor()
{
	pConfiguration->EnableCursor(!pConfiguration->IsCursorEnabled());
	if (pConfiguration->IsCursorEnabled())
		glfwEnable(GLFW_MOUSE_CURSOR);
	else
		glfwDisable(GLFW_MOUSE_CURSOR);
}

void Screen::ToggleFullscreen()
{
	// destroy opengl textures
	// change video mode
	// reconfigure opengl context
	// reload textures

	bFullScreen = !bFullScreen;
	iFlags ^= GLFW_FULLSCREEN;
	iFlags ^= GLFW_WINDOW;
	pConfiguration->SetFullScreen(bFullScreen);

	pResourceManager->Unload(Seed::TypeTexture);
	pRendererDevice->Shutdown();
	pScreen->Shutdown();
	pScreen->Initialize();
	this->EnableCursor(pConfiguration->IsCursorEnabled());
	pRendererDevice->Initialize();
	pResourceManager->Reload(Seed::TypeTexture);

#if defined(WIN32)
	if (!bFullScreen)
	{
		RECT rcTmp,rectWindow;
		GetClientRect(GetActiveWindow(), &rcTmp);
		rectWindow.left = 0;
		rectWindow.top = 0;
		rectWindow.right = rcTmp.right;
		rectWindow.bottom = rcTmp.bottom;

		SetWindowPos(GetActiveWindow(), HWND_TOP, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom, SWP_SHOWWINDOW);
	}
#endif
}

bool Screen::HasWindowedMode() const
{
	return true;
}

bool Screen::IsFullscreen() const
{
	return bFullScreen;
}

}} // namespace

#endif // BUILD_GLFW
