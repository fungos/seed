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

	cCameraTex.sName = "Camera to Texture";
	cCameraTex.SetTexture(&cRenderTarget);

	cCamera.sName = "Camera";
	//pScene->Add(&cCamera);

	cScene.sName = "Main";

	{
//		File f("anim.sprite");
//		Reader r(f);
//		sptLogo.Load(r);
//		sptLogo.sName = "ANIM";

//		sptLogo.SetPivot(0.0f, 0.0f);
//		sptLogo.SetPosition(401, 100);
//		sptLogo.SetRotation(45.f);

//		cCamera.SetPosition(-400, -300);

//		sptLogo.SetPriority(100.0f);
//		cScene.Add(&sptLogo);
	}

	cCamera.Update(0.0f);

//	{
//		Writer w;
//		sptLogo.Write(w);
//		w.Save("out.sprite");
//	}

	{
		File f("sample.movie");
		Reader r(f);
		mvSample.Load(r);
		mvSample.SetZ(200.0f);
		cScene.Add(&mvSample);
	}

//	{
//		File f("teste.emitter");
//		Reader r(f);
//		cEmitter.Load(r);
//		cEmitter.SetPosition(-400, -300);
//		cEmitter.SetPriority(10.0f);
//		cScene.Add(&cEmitter);
//	}

//	cScene.SetPosition(100, 100);
//	sptLogo.SetParent(&cScene);

	musTheme.Load("theme.ogg");
	musTheme.SetVolume(.2f);
	pSoundSystem->PlayMusic(&musTheme);

//	{
//		File f("sound.sfx");
//		Reader r(f);
//		sfxSound.Load(r);
//	}

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
