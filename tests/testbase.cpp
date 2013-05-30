#include "testbase.h"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>

SceneNode *gScene;

enum
{
	kJobLoadEmitter,
	kJobLoadAnim,
	kJobLoadScene
};

TestBase::TestBase()
{
}

TestBase::~TestBase()
{
}

bool TestBase::Initialize()
{
	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	/* ------- Rendering Initialization ------- */
	cScene.SetZ(0);
	cRenderer.SetScene(&cScene);

	cViewport.SetHeight(pScreen->GetHeight());
	cViewport.SetWidth(pScreen->GetWidth());
	cViewport.SetCamera(&cCamera);
	cViewport.SetRenderer(&cRenderer);

	pViewManager->Add(&cViewport);
	pRendererManager->Add(&cRenderer);
	pSceneManager->Add(&cScene);
	gScene = &cScene;
	/* ------- Rendering Initialization ------- */

	/* ------- libRocket ------- */
	pRocketInterface = New(Seed::RocketGui::RocketInterface());
	Rocket::Core::SetFileInterface(pRocketInterface);
	Rocket::Core::SetSystemInterface(pRocketInterface);
	Rocket::Core::SetRenderInterface(pRocketInterface);

	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();
	/* ------- libRocket ------- */

	// Initialize the texture that will be our render target
//	cRenderTarget.Load(800, 600);
//	pRendererDevice->TextureRequest(&cRenderTarget);
//	pRendererDevice->TextureRequestProcess();
	// --

//	{
//		File f("anim.sprite");
//		Reader r(f);
//		sptLogo.Load(r);
//		cScene.Add(&sptLogo);
//	}

//	{
//		File f("sample.movie");
//		Reader r(f);
//		mvSample.Load(r);
//		mvSample.SetZ(200.0f);
//		cScene.Add(&mvSample);
//	}

	pJobManager->Add(New(FileLoader("scenes/main.scene", kJobLoadScene, this)));
//	pJobManager->Add(New(FileLoader("anim.sprite", kJobLoadAnim, this)));
//	pJobManager->Add(New(FileLoader("teste.emitter", kJobLoadEmitter, this)));
//	cScene.Add(&sptLogo);
//	cScene.Add(&cEmitter);

//	{
//		File f("teste.emitter");
//		Reader r(f);
//		cEmitter.Load(r);
////		cEmitter.SetPosition(200, 0);
////		cEmitter.SetZ(10.0f);
//		cScene.Add(&cEmitter);
//	}

//	{
//		musTheme.Load("theme.ogg");
//		musTheme.SetVolume(.2f);
//		pSoundSystem->PlayMusic(&musTheme);
//	}

//	{
//		File f("sound.sfx");
//		Reader r(f);
//		sfxSound.Load(r);
//	}

//	{
//		Writer w;
//		sptLogo.Write(w);
//		w.Save("out.sprite");
//	}

//	cScene.SetPosition(100, 100);
//	sptLogo.SetParent(&cScene);

	cScene.sName = "Main";

	cCameraTex.sName = "Camera to Texture";
	cCameraTex.SetTexture(&cRenderTarget);

	cCamera.sName = "Camera";
	cCamera.SetPosition(-400, -300);
	cCamera.Update(0.0f);

//	gScene->Add(&cCamera);
//	pScreen->FadeIn();

	return true;
}

bool TestBase::Update(f32 dt)
{
	UNUSED(dt)
//	pRendererDevice->DrawRect(100, 100, 50, 50, Color(255, 0, 255, 255), true);
	return true;
}

bool TestBase::Shutdown()
{
//	{
//		Writer w;
//		gScene->Write(w);
//		w.Save("out.scene");
//	}

//	cEmitter.Unload();
//	sptLogo.Unload();
//	mvSample.Unload();

	pResourceManager->GarbageCollect();

	Rocket::Core::Shutdown();
	Delete(pRocketInterface);

	pSceneManager->Reset();
	pRendererManager->Reset();
	pViewManager->Reset();

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	gScene->Unload();
	IGameApp::Shutdown();

	return true;
}

bool TestBase::Reset()
{
	return true;
}

void TestBase::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void TestBase::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pLeakReport->Print();
	else if (k == Seed::KeyF3)
		pResourceManager->GarbageCollect();
}

void TestBase::OnJobCompleted(const EventJob *ev)
{
	switch (ev->GetName())
	{
//		case kJobLoadEmitter:
//		{
//			FileLoad *job = (FileLoad *)ev->GetJob();
//			Reader r(job->pFile);
//			cEmitter.Load(r);
//			Delete(job);
//		}
//		break;

//		case kJobLoadAnim:
//		{
//			FileLoad *job = (FileLoad *)ev->GetJob();
//			Reader r(job->pFile);
//			sptLogo.Load(r);
//			Delete(job);
//		}
//		break;

		case kJobLoadScene:
		{
			FileLoader *job = (FileLoader *)ev->GetJob();
			Reader r(job->pFile);
			gScene->Load(r);
			Delete(job);
		}
		break;
	}
}

void TestBase::OnJobAborted(const EventJob *ev)
{
	Job *job = ev->GetJob();
	Log("Aborting job");
	Delete(job);
}
