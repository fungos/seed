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

/*! \file SeedInit.cpp
	\author	Danny Angelo Carminati Grein
	\brief Seed Initialization / Shutdown
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
#include "ParticleEmitterObject.h"
#include "ParticleManager.h"
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
	if (!STRCASECMP(param, "--nosound"))
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
	ret = ret && pModuleManager->Add(pParticleManager);

	pUpdater->Add(Private::pApplication);
#if !defined(BUILD_IOS)
	pUpdater->Add(pInput);
#endif

	if (!Private::bDisableSound)
		pUpdater->Add(pSoundSystem);

	pUpdater->Add(pSystem);
	pUpdater->Add(pResourceLoader);
	pUpdater->Add(pParticleManager);
	pUpdater->Add(pRendererManager);
	pUpdater->Add(pSceneManager);

	ResourceManager::Register(Seed::ObjectTexture,			TextureResourceLoader);
	ResourceManager::Register(Seed::ObjectSound,			SoundResourceLoader);
	ResourceManager::Register(Seed::ObjectMusic,			MusicResourceLoader);
	ResourceManager::Register(Seed::ObjectParticleEmitterObject, ParticleEmitterObjectResourceLoader);

	//pSystem->InitializeHome(); FIXME home can only be initialized after the ROM package with the
	//default error font was called and this is done at app->Initialize since the memory manager is not
	//ready during Setup
	Private::bInitialized = true;

	ret = ret && pModuleManager->Add(Private::pApplication);

	pModuleManager->Print();

	return ret;
}

void Update()
{
	if (!Private::bInitialized)
		return;

	f32 newTime				= pTimer->GetMilliseconds() / 1000.0f;
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

	pParticleManager->DestroyInstance();
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
