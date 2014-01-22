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
#include "SceneObjectFactory.h"
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
#include "Manager.h"
#include "Cartridge.h"
#include "ViewManager.h"
#include "renderer/Camera.h"
#include "renderer/RendererManager.h"
#include "PrefabManager.h"
#include "SceneManager.h"
#include "RendererDevice.h"
#include "JobManager.h"
#include "Checksum.h"
#include "Profiler.h"
#include "Configuration.h"
#include "ThreadManager.h"
#include "map/GameMap.h"

#include "Sprite.h"
#include "Image.h"
#include "Movie.h"
#include "ParticleEmitter.h"

#if defined(SEED_ENABLE_OGLES2) || defined(SEED_ENABLE_OGL20)
#include "ShaderManager.h"
#include "ShaderProgram.h"
#include "Shader.h"
#endif

extern "C" {

namespace Seed {

namespace Private
{
	IGameApp	*pApplication;
	bool		bInitialized;
	int			iArgc;
	char		**pcArgv;
	bool		bDisableSound;
	bool		bDisableThread;
	bool		bDisableResourceLoader;
	String		sConfigFile;
	String		sWorkDir;
	f32			fCurrentTime;
}

ResourceManager *pResourceManager = nullptr;

#define MAX_FRAME_DELTA ((1.0f / 60.0f) * 5.0f)

#ifdef _MSC_VER
#define SEED_strcasecmp _stricmp
#else
#define SEED_strcasecmp strcasecmp
#endif

int CommandLineParameter(char **argv, int pos)
{
	const char *param = argv[pos];
	int consume = 1;

	if (!SEED_strcasecmp(param, "--no-sound"))
	{
		Private::bDisableSound = true;
	}
	else if (!SEED_strcasecmp(param, "--no-thread"))
	{
		Private::bDisableThread = true;
	}
	else if (!SEED_strcasecmp(param, "--no-resourceloader"))
	{
		Private::bDisableResourceLoader = true;
	}
	else if (!SEED_strcasecmp(param, "--config"))
	{
		Private::sConfigFile = argv[pos + 1];
		consume++;
	}
	else if (!SEED_strcasecmp(param, "--workdir"))
	{
		Private::sWorkDir = argv[pos + 1];
		consume++;
	}

	return consume;
}

void CommandLineParse(int argc, char **argv)
{
	int i = 0;
	while (i < argc)
	{
		i += Seed::CommandLineParameter(argv, i);
	}
}

void SetGameApp(IGameApp *app, int argc, char **argv, const char *config)
{
	Private::iArgc = argc;
	Private::pcArgv = argv;
	Private::pApplication = app;
	Private::sConfigFile = config;
	Private::bDisableSound = false;

#if defined(EMSCRIPTEN)
	Private::bDisableThread = true;
	Private::bDisableResourceLoader = true;
#else
	Private::bDisableThread = false;
	Private::bDisableResourceLoader = false;
#endif
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

	Info(SEED_BANNER, SEED_VERSION_MAJOR, SEED_VERSION_MIDDLE, SEED_VERSION_MINOR);

	Info("");
	Info(SEED_TAG "Build Configuration:");

	Info(SEED_TAG "\tThreading: %s", SEED_USE_THREAD ? "Yes" : "No");
	Info(SEED_TAG "\tTheora: %s", SEED_USE_THEORA ? "Yes" : "No");
	Info(SEED_TAG "\tSingleton: %s", SEED_SINGLETON_HEAP ? "Heap" : "Stack");
	Info(SEED_TAG "\tMusic Buffer: %d", SEED_MUSIC_STREAMING_BUFFER_SIZE);
	Info(SEED_TAG "\tReader: %s", SEED_USE_JSON ? "JSON" : "Binary");
	Info(SEED_TAG "Initializing...");

	bool ret = true;

	pChecksum = Checksum::GetInstance();

	ret = ret && pManager->Add(pFileSystem);
	pConfiguration->Load(Private::sConfigFile);
	if (Private::sWorkDir != "")
		pConfiguration->SetWorkingDirectory(Private::sWorkDir); // cli has priority
	pFileSystem->Prepare();
	pScreen->EnableCursor(pConfiguration->IsCursorEnabled());

	Info(SEED_TAG "Options: ");
	Info(SEED_TAG "\tSound: %s", Private::bDisableSound ? "No" : "Yes");
	Info(SEED_TAG "\tThread: %s", Private::bDisableThread ? "No" : "Yes");
	Info(SEED_TAG "\tResourceLoader: %s", Private::bDisableResourceLoader ? "No" : "Yes");

	ret = ret && pManager->Add(pSystem);
	ret = ret && pManager->Add(pCartridge);
	ret = ret && pManager->Add(pScreen);
	ret = ret && pManager->Add(pRendererDevice);
	ret = ret && pManager->Add(pViewManager);
	ret = ret && pManager->Add(pRendererManager);
	pPrefabManager->GetInstance();

#if defined(SEED_ENABLE_OGLES2) || defined(SEED_ENABLE_OGL20)
	ret = ret && pManager->Add(pShaderManager);
#endif

	if (!Private::bDisableSound)
		ret = ret && pManager->Add(pSoundSystem);

#if (SEED_USE_THREAD == 1)
	if (!Private::bDisableThread || !Private::bDisableResourceLoader)
		ret = ret && pManager->Add(pResourceLoader);
#else
	ret = ret && pManager->Add(pThreadManager);
#endif

	ret = ret && pManager->Add(pJobManager);
	ret = ret && pManager->Add(pInput);

	pUpdater->Add(Private::pApplication);

#if !defined(BUILD_IOS)
	pUpdater->Add(pInput);
#endif

#if (SEED_USE_THREAD == 1)
	if (!Private::bDisableThread || !Private::bDisableResourceLoader)
		pUpdater->Add(pResourceLoader);
#else
	pUpdater->Add(pThreadManager);
#endif

	if (!Private::bDisableSound)
		pUpdater->Add(pSoundSystem);

	pUpdater->Add(pSystem);
	pUpdater->Add(pJobManager);
	pUpdater->Add(pRendererManager);
	pUpdater->Add(pSceneManager);

	ResourceManager::Register(ITexture::GetTypeId(), TextureResourceLoader);
	ResourceManager::Register(ISound::GetTypeId(), SoundResourceLoader);
	ResourceManager::Register(IMusic::GetTypeId(), MusicResourceLoader);

	SceneObjectFactory::Register("Sprite", FactorySprite);
	SceneObjectFactory::Register("Movie", FactoryMovie);
	SceneObjectFactory::Register("ParticleEmitter", FactoryParticleEmitter);
	SceneObjectFactory::Register("Image", FactoryImage);
	SceneObjectFactory::Register("SoundSource", FactorySoundSource);
	SceneObjectFactory::Register("Camera", FactoryCamera);
	SceneObjectFactory::Register("SceneNode", FactorySceneNode);
	SceneObjectFactory::Register("GameMap", FactoryGameMap);

	Private::bInitialized = true;

	ret = ret && pManager->Add(Private::pApplication);
	pManager->Print();

	return ret;
}

void Update()
{
	if (!Private::bInitialized)
		return;

	const f32 maxRate		= 5.0f;
	Seconds newTime			= pTimer->GetSeconds();
	Seconds dt				= newTime - Private::fCurrentTime;
	Private::fCurrentTime	= newTime;
	Seconds maxFrameDt		= (Seconds(1) / pConfiguration->GetFrameRate()) * maxRate;

	if (dt > maxFrameDt)
		dt = maxFrameDt;

	pUpdater->Run(dt);

#if !defined(BUILD_IOS)
	Seed::Render();
#endif
}

void Render()
{
	pScreen->Update();
	pViewManager->Render();
}

void Shutdown()
{
	if (!Private::bInitialized)
		return;

	Info(SEED_TAG "Shutting down subsystems...");
	pPrefabManager->GetInstance()->Reset();
	pManager->Shutdown();

	pPrefabManager->DestroyInstance();
	pSceneObjectFactory->DestroyInstance();
	pSceneManager->DestroyInstance();
	pJobManager->DestroyInstance();
	pInput->DestroyInstance();

#if (SEED_USE_THREAD == 1)
	pResourceLoader->DestroyInstance();
#else
	pThreadManager->DestroyInstance();
#endif

	pSoundSystem->DestroyInstance();
	pRendererManager->DestroyInstance();
	pViewManager->DestroyInstance();
	pScreen->DestroyInstance();
	pCartridge->DestroyInstance();
	pSystem->DestroyInstance();
	pFileSystem->DestroyInstance();

	ProfilerReportPrint;
	ProfilerTerminate;
	LeakReportPrint();

	Private::bInitialized = false;
	Private::pApplication = nullptr;
}

} // namespace

} // extern C
