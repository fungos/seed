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

#include "Defines.h"
#include "SeedInit.h"
#include "ResourceManager.h"
#include "ResourceLoader.h"
#include "Texture.h"
#include "Timer.h"
#include "FileSystem.h"
#include "System.h"
#include "Music.h"
#include "Sound.h"
#include "SoundSystem.h"
#include "Screen.h"
#include "Log.h"
#include "Input.h"
#include "Updater.h"
#include "ModuleManager.h"
#include "Cartridge.h"
#include "ViewManager.h"
#include "RendererManager.h"
#include "SceneManager.h"
#include "RendererDevice.h"
#include "Checksum.h"
#include "Profiler.h"

extern "C" {

namespace Seed {

namespace Private
{
	IGameApp	*pApplication;// 	= NULL;
	bool		bInitialized;// 	= false;
	int			iArgc;//			= 0;
	const char	**pcArgv;//		= NULL;
	bool		bDisableSound;//	= false;
	f32			fCurrentTime;//	= 0.0f;
}

ResourceManager *pResourceManager = NULL;
Configuration *pConfiguration = NULL;

#define MAX_FRAME_DELTA ((1.0f / 60.0f) * 5.0f)

void CommandLineParameter(const char *param)
{
	if (!strcasecmp(param, "--nosound"))
	{
		Private::bDisableSound = true;
	}
}

void CommandLineParse(int argc, const char **argv)
{
	int i = 0;
	while (i < argc)
	{
		const char *param = argv[i];
		Seed::CommandLineParameter(param);
		i++;
	}
}

void SetGameApp(IGameApp *app, int argc, const char **argv)
{
	Private::iArgc = argc;
	Private::pcArgv = argv;
	Private::pApplication = app;
	pConfiguration  = app->GetConfiguration();
	pResourceManager = app->GetResourceManager();

	Seed::CommandLineParse(argc, argv);
}

void WriteOut(const char *msg)
{
	if (Private::pApplication)
		Private::pApplication->WriteOut(msg);
}

void WriteErr(const char *msg)
{
	if (Private::pApplication)
		Private::pApplication->WriteErr(msg);
}

void GetVersion(u32 *major, u32 *middle, u32 *minor)
{
	if (major)
		*major = SEED_VERSION_MAJOR;

	if (middle)
		*middle = SEED_VERSION_MAJOR;

	if (minor)
		*minor = SEED_VERSION_MAJOR;
}

bool Initialize()
{
	if (!Private::pApplication)
	{
		fprintf(stderr, "ERROR: You should set a GameApp by calling SetGameApp(IGameApp *app, int argc, char **argv)!\n");
		HALT;
	}

	Info(SEED_MESSAGE, SEED_VERSION_MAJOR, SEED_VERSION_MIDDLE, SEED_VERSION_MINOR);

	Info("");
	Info(SEED_TAG "Build Configuration:");

	Info(SEED_TAG "\tTheora: %s", SEED_USE_THEORA ? "Yes" : "No");
	Info(SEED_TAG "\tSingleton: %s", SEED_SINGLETON_HEAP ? "Heap" : "Stack");
	Info(SEED_TAG "\tMusic Buffer: %d", SEED_MUSIC_STREAMING_BUFFER_SIZE);
	Info(SEED_TAG "Initializing...");

	bool ret = true;

	pChecksum = Checksum::GetInstance();

	ret = ret && pModuleManager->Add(pSystem);
	ret = ret && pModuleManager->Add(pTimer);
	ret = ret && pModuleManager->Add(pFileSystem);
	ret = ret && pModuleManager->Add(pCartridge);
	ret = ret && pModuleManager->Add(pScreen);
	ret = ret && pModuleManager->Add(pRendererDevice);
	ret = ret && pModuleManager->Add(pViewManager);
	ret = ret && pModuleManager->Add(pRendererManager);

	if (!Private::bDisableSound)
		ret = ret && pModuleManager->Add(pSoundSystem);

	ret = ret && pModuleManager->Add(pResourceLoader);
	ret = ret && pModuleManager->Add(pInput);

	pUpdater->Add(Private::pApplication);
#if !defined(BUILD_IOS)
	pUpdater->Add(pInput);
#endif

	if (!Private::bDisableSound)
		pUpdater->Add(pSoundSystem);

	pUpdater->Add(pSystem);
	pUpdater->Add(pResourceLoader);
	pUpdater->Add(pRendererManager);
	pUpdater->Add(pSceneManager);

	ResourceManager::Register(Seed::ObjectTexture,			TextureResourceLoader);
	ResourceManager::Register(Seed::ObjectSound,			SoundResourceLoader);
	ResourceManager::Register(Seed::ObjectMusic,			MusicResourceLoader);

	Private::bInitialized = true;

	ret = ret && pModuleManager->Add(Private::pApplication);
	pModuleManager->Print();

	return ret;
}

void Update()
{
	if (!Private::bInitialized)
		return;

	f32 newTime				= (f32)pTimer->GetMilliseconds() / 1000.0f;
	f32 dt					= newTime - Private::fCurrentTime;
	Private::fCurrentTime	= newTime;
	f32 frameDelta			= (1.0f / pConfiguration->GetFrameRate()) * 5.0f;

	if (dt > frameDelta)
		dt = frameDelta;

	pUpdater->Run(dt);

	Seed::Render();
}

void Render()
{
	pScreen->Update();
	// FIXME: Viewport Render and Screen Update must be generic
#if !defined(BUILD_QT)
	pViewManager->Render();
#endif
}

void Shutdown()
{
	if (!Private::bInitialized)
		return;

	Info(SEED_TAG "Shutting down subsystems...");
	pModuleManager->Shutdown();

	pSceneManager->DestroyInstance();
	pInput->DestroyInstance();
	pResourceLoader->DestroyInstance();
	pSoundSystem->DestroyInstance();
	pRendererManager->DestroyInstance();
	pViewManager->DestroyInstance();
	pScreen->DestroyInstance();
	pCartridge->DestroyInstance();
	pFileSystem->DestroyInstance();
	pTimer->DestroyInstance();
	pSystem->DestroyInstance();

	ProfilerReportPrint;
	ProfilerTerminate;
	LeakReportPrint;

	Private::bInitialized = false;
	Private::pApplication = NULL;
}

}

} // namespace
