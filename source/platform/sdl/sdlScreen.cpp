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

#if defined(BUILD_SDL)

#include "Screen.h"
#include "Log.h"
#include "RendererDevice.h"
#include "SeedInit.h"
#include "Configuration.h"

#if defined(WIN32)
#define USER_DEFAULT_SCREEN_DPI	96
#include <SDL/SDL_syswm.h>
#endif

#define TAG "[Screen] "

namespace Seed { namespace SDL {

SEED_SINGLETON_DEFINE(Screen)

Screen::Screen()
	: iHandle(0)
	, surfaceSize(0)
	, pSurface(NULL)
	, bFullScreen(false)
	, iFadeStatus(0)
	, fadeType(FADE_IN)
	, iBPP(32)
	, iFlags(0)
	, videoInfo(NULL)
{
}

Screen::~Screen()
{
}

bool Screen::Reset()
{
#if defined(__linux__)
	this->PrepareMode();
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
	videoInfo = const_cast<SDL_VideoInfo *>(SDL_GetVideoInfo());
	if (videoInfo)
	{
		Info(TAG "SDL Video Info:");
		Info(TAG "\tHardware surface available...: %s", videoInfo->hw_available ? "yes" : "no");
		Info(TAG "\tWindow manager available.....: %s", videoInfo->wm_available ? "yes" : "no");
		Info(TAG "\tHardware blit accelerated....: %s", videoInfo->blit_hw ? "yes" : "no");
		Info(TAG "\tHardware colorkey blit.......: %s", videoInfo->blit_hw_CC ? "yes" : "no");
		Info(TAG "\tHardware alpha blit..........: %s", videoInfo->blit_hw_A ? "yes" : "no");
		Info(TAG "\tSoftware to hardware blit....: %s", videoInfo->blit_sw ? "yes" : "no");
		Info(TAG "\tSoftware to hardware colorkey: %s", videoInfo->blit_sw_CC ? "yes" : "no");
		Info(TAG "\tSoftware to hardware alpha...: %s", videoInfo->blit_sw_A ? "yes" : "no");
		Info(TAG "\tColor fill accelerated.......: %s", videoInfo->blit_fill ? "yes" : "no");
		Info(TAG "\tDisplay pixel format.........: ");
		Info(TAG "\t\tBytes per pixel............: %d", videoInfo->vfmt->BytesPerPixel);
#if !defined(EMSCRIPTEN)
		Info(TAG "\t\tColorkey...................: %x", videoInfo->vfmt->colorkey);
		Info(TAG "\t\talpha......................: %d", videoInfo->vfmt->alpha);
#endif
		Info(TAG "\t\tRGBA loss..................: %d %d %d %d", videoInfo->vfmt->Rloss, videoInfo->vfmt->Gloss, videoInfo->vfmt->Bloss, videoInfo->vfmt->Aloss);
		Info(TAG "\tBest resolution..............: %dx%d", videoInfo->current_w, videoInfo->current_h);
		Info(TAG "\tTotal video memory available.: %d", videoInfo->video_mem);
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
		if (videoInfo)
		{
			iWidth = videoInfo->current_w;
			iHeight = videoInfo->current_h;
			iBPP = videoInfo->vfmt->BitsPerPixel;
		}
		else
		{
			Log(TAG "Error: Failed to auto detect video mode.");
			return;
		}
	}

	fAspectRatio = (f32)iHeight / (f32)iWidth;
	iFlags = SDL_DOUBLEBUF | SDL_HWSURFACE;

	if (bFullScreen)
		iFlags |= SDL_FULLSCREEN;
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

	this->Prepare();
	if (!videoInfo)
	{
		Log(TAG "ERROR: You must set up a video mode!");
		return false;
	}

	if (!this->InitializeVideo())
		return false;

	Info(TAG "Video resolution is %dx%dx%d.", iWidth, iHeight, iBPP);

#if defined(DEBUG)
	SDL_ShowCursor(1);
#else
	SDL_ShowCursor(0);
#endif // DEBUG

	Log(TAG "Initialization completed.");
	return true;
}

bool Screen::InitializeVideo()
{
	bool ret = true;

	if (pSurface)
	{
		SDL_FreeSurface(pSurface);
		pSurface = NULL;
	}

#if defined(__APPLE_CC__) || defined(__linux__)
	this->SetupOpenGL();
#else
	eRendererDeviceType type = pConfiguration->GetRendererDeviceType();
	if (type == Seed::RendererDeviceOpenGL1x || type == Seed::RendererDeviceOpenGL2x ||
		type == Seed::RendererDeviceOpenGL3x || type == Seed::RendererDeviceOpenGL4x)
	{
		this->SetupOpenGL();
	}
	else
	{
#if !defined(SEED_ENABLE_D3D8) && !defined(SEED_ENABLE_D3D9) && !defined(SEED_ENABLE_D3D10) && !defined(SEED_ENABLE_D3D11)
		this->SetupOpenGL();
#endif
	}
#endif

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

	SDL_WM_SetCaption(pConfiguration->GetApplicationTitle().c_str(), pConfiguration->GetApplicationTitle().c_str());
	pSurface = SDL_SetVideoMode(iWidth, iHeight, iBPP, iFlags);
	if (!pSurface)
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
		{
			SDL_Surface *icon = SDL_LoadBMP("icon.bmp");
			if (icon)
			{
				Uint32 colorkey = SDL_MapRGB(icon->format, 255, 0, 255);
				SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);
				SDL_WM_SetIcon(icon, NULL);
			}
		}
	}

	return ret;
}

bool Screen::Shutdown()
{
	Log(TAG "Terminating...");

	iFadeStatus = 0;
	fadeType	= FADE_IN;
	iHeight		= 0;
	iWidth		= 0;
	iBPP		= 32;
	iFlags		= 0;
	videoInfo	= NULL;

	if (pSurface)
		SDL_FreeSurface(pSurface);
	pSurface = NULL;

	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	Log(TAG "Terminated.");

	return true;
}

void Screen::Update()
{
	this->SwapSurfaces();
	pRendererDevice->Update();
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
	if (iFlags & SDL_OPENGL)
		SDL_GL_SwapBuffers();
}

void Screen::SetupOpenGL()
{
	iFlags |= SDL_OPENGL;

	// http://sdl.beuc.net/sdl.wiki/SDL_GLattr
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
#if !defined(EMSCRIPTEN)
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
#endif

	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
}

void Screen::ToggleFullscreen()
{
	// destroy opengl textures
	// change video mode
	// reconfigure opengl context
	// reload textures
	bFullScreen = !bFullScreen;
#if defined(__linux__)
	SDL_WM_ToggleFullScreen(pSurface);
#else
	iFlags ^= SDL_FULLSCREEN;

	pResourceManager->Unload(Seed::TypeTexture);
	pRendererDevice->Shutdown();
	this->InitializeVideo();
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

#endif // BUILD_SDL
