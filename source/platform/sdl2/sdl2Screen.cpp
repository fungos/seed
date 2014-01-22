/*
* Copyright (c) 2013, Seed Developers
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

#if defined(BUILD_SDL2)

#include "Screen.h"
#include "Log.h"
#include "RendererDevice.h"
#include "SeedInit.h"
#include "Configuration.h"
#include "Texture.h"

#if defined(WIN32)
#define USER_DEFAULT_SCREEN_DPI	96
#include <WinUser.h>
#include <SDL/SDL_syswm.h>
#endif

#define TAG "[Screen] "

namespace Seed { namespace SDL2 {

SEED_SINGLETON_DEFINE(Screen)

Screen::Screen()
	: iHandle(0)
	, surfaceSize(0)
	, pWindow(NULL)
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
	pResourceManager->Unload(ITexture::GetTypeId());
	pRendererDevice->Shutdown();
	this->Shutdown();
	this->Initialize();
	pRendererDevice->Initialize();
	pResourceManager->Reload(ITexture::GetTypeId());
#endif

	return true;
}

bool Screen::Prepare()
{
	u32 reqW = pConfiguration->GetResolutionWidth();
	u32 reqH = pConfiguration->GetResolutionHeight();
	if (reqW && reqH)
	{
		iWidth = reqW;
		iHeight = reqH;
	}

	fAspectRatio = (f32)iHeight / (f32)iWidth;
	return true;
}

bool Screen::Initialize()
{
	Log(TAG "Initializing...");

	bFullScreen = pConfiguration->GetFullScreen();

	bFading = false;
	iFadeStatus = 16;
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		Log(TAG "ERROR: Failed to initialize screen.");
		Log(TAG "Initialization failed.");
		return false;
	}

	if (!this->Prepare())
	{
		Log(TAG "ERROR: You must set up a video mode!");
		return false;
	}

	if (!this->InitializeVideo())
		return false;

	Info(TAG "Video resolution is %dx%dx%d.", iWidth, iHeight, iBPP);
	Log(TAG "Initialization completed.");
	return true;
}

bool Screen::InitializeVideo()
{
	bool ret = true;

	if (pWindow)
	{
		SDL_DestroyWindow(pWindow);
		pWindow = NULL;
	}

	this->SetupOpenGL();

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
	int scaleX = MulDiv(cxScreen, USER_DEFAULT_SCREEN_DPI, dpiX);
	int scaleY = MulDiv(cyScreen, USER_DEFAULT_SCREEN_DPI, dpiY);

	Info(TAG "Desktop DPI: %dx%d", dpiX, dpiY);
	Info(TAG "Desktop Resolution: %dx%d", cxScreen, cyScreen);
	Info(TAG "Desktop DPI scaled resolution: %dx%d", scaleX, scaleY);

	if (cxScreen <= 1024 && cyScreen <= 768 && iWidth > 800 && iHeight > 600)
	{
		iWidth = 800;
		iHeight = 600;
	}
#endif

	pWindow = SDL_CreateWindow(pConfiguration->GetApplicationTitle().c_str(),
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								iWidth, iHeight,
								SDL_WINDOW_OPENGL);

	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!pWindow)
	{
		Log(TAG "Could not set video mode: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
#if defined(WIN32)
		bool icon = false;

		/*
		If there is a icon.ico file in the SAME directory of the executable (can't be workdir) we will use it, otherwise we will look for
		<workdir>/data/icon.bmp and use it instead.
		*/
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		if (SDL_GetWMInfo(&info) > 0)
		{
			HWND hWnd = info.window;
			iHandle = (u32)hWnd;

			const HANDLE bigIcon = ::LoadImageA(NULL, "icon.ico", IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE);
			if (bigIcon)
			{
				icon = true;
				::SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)bigIcon);
			}

			const HANDLE lilIcon = ::LoadImageA(NULL, "icon.ico", IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);
			if (bigIcon)
			{
				icon = true;
				::SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)lilIcon);
			}
		}
		if (!icon)
#endif
	}

	return ret;
}

bool Screen::Shutdown()
{
	Log(TAG "Terminating...");

	iFadeStatus = 0;
	nFadeType	= eFade::In;
	bFading		= false;
	iHeight		= 0;
	iWidth		= 0;
	iBPP		= 32;
	iFlags		= 0;

	if (pWindow)
	{
		SDL_DestroyWindow(pWindow);
	}

	pWindow = NULL;

	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	Log(TAG "Terminated.");

	return true;
}

void Screen::Update()
{
	SDL_PumpEvents();
	this->SwapSurfaces();
	pRendererDevice->Update();
}

void Screen::SwapSurfaces()
{
	SDL_GL_SwapWindow(pWindow);
}

void Screen::SetupOpenGL()
{
	// http://sdl.beuc.net/sdl.wiki/SDL_GLattr
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
#if !defined(EMSCRIPTEN)
	//SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
#endif

	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
}


void Screen::EnableCursor(bool b)
{
	pConfiguration->EnableCursor(b);
	SDL_ShowCursor(b);
}

void Screen::ToggleCursor()
{
	bool b = !pConfiguration->IsCursorEnabled();
	pConfiguration->EnableCursor(b);
	SDL_ShowCursor(b);
}

void Screen::ToggleFullscreen()
{
	bFullScreen = !bFullScreen;

	if (!bFullScreen)
	{
		// Switch to WINDOWED mode
		if (SDL_SetWindowFullscreen(pWindow, SDL_FALSE) < 0)
		{
			Info(TAG "CRITICAL: Setting windowed failed: '%s'.", SDL_GetError());
		}
	}
	else
	{
		// Switch to FULLSCREEN mode
		if (SDL_SetWindowFullscreen(pWindow, SDL_TRUE) < 0)
		{
			Info(TAG "CRITICAL: Setting fullscreen failed: '%s'.", SDL_GetError());
		}
	}
}

bool Screen::HasWindowedMode() const
{
	return true;
}

bool Screen::IsFullscreen() const
{
	return bFullScreen;
}

SDL_Renderer* Screen::GetRenderer()
{
	return pRenderer;
}

}} // namespace

#endif // BUILD_SDL2
