#include "testbase.h"

SceneNode *pScene;

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
	cRenderer.Add(&cScene);

	cViewport.SetHeight(pScreen->GetHeight());
	cViewport.SetWidth(pScreen->GetWidth());
	cViewport.SetCamera(&cCamera);
	cViewport.SetRenderer(&cRenderer);

	pViewManager->Add(&cViewport);
	pRendererManager->Add(&cRenderer);
	pSceneManager->Add(&cScene);
	pScene = &cScene;
	/* ------- Rendering Initialization ------- */

	// Initialize the texture that will be our render target
	cRenderTarget.Load(800, 600);
	pRendererDevice->TextureRequest(&cRenderTarget);
	pRendererDevice->TextureRequestProcess();
	// --

	{
		File f("anim.sprite");
		Reader r(f);
		sptLogo.Load(r);
		cScene.Add(&sptLogo);
	}

	{
		File f("sample.movie");
		Reader r(f);
		mvSample.Load(r);
		mvSample.SetZ(200.0f);
		cScene.Add(&mvSample);
	}

	{
		File f("teste.emitter");
		Reader r(f);
		cEmitter.Load(r);
		cEmitter.SetPosition(200, 0);
		cEmitter.SetZ(10.0f);
		cScene.Add(&cEmitter);
	}

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

	//pScene->Add(&cCamera);
	pScreen->FadeIn();

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
	cEmitter.Unload();
	sptLogo.Unload();
	mvSample.Unload();

	pSceneManager->Reset();
	pRendererManager->Reset();
	pViewManager->Reset();

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

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
}
