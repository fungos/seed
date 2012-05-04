#include "testbase.h"

SceneNode *pScene;

TestBase::TestBase()
{
	cConfig.SetApplicationTitle("tests");
	cConfig.SetPublisherName("seed");
	cConfig.SetVideoMode(Seed::Video_800x600);
//	cConfig.bDebugSprite = true;
}

TestBase::~TestBase()
{
}

bool TestBase::Initialize()
{
	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	/* ------- Rendering Initialization ------- */
	cScene.SetPriority(0);
	cRenderer.Add(&cScene);
	cViewport.SetRenderer(&cRenderer);
	cViewport.SetHeight(pScreen->GetHeight());
	cViewport.SetWidth(pScreen->GetWidth());
	pViewManager->Add(&cViewport);
	pRendererManager->Add(&cRenderer);
	pSceneManager->Add(&cScene);
	pScene = &cScene;
	/* ------- Rendering Initialization ------- */

	cScene.sName = "Main";

	{
		File f("teste.emitter");
		Reader r(f);
		cEmitter.Load(r);
		cEmitter.SetPosition(200, 100);
		cEmitter.Play();
		cEmitter.SetPriority(10.0f);
		cScene.Add(&cEmitter);
	}

	{
		File f("anim.sprite");
		Reader r(f);
		sptLogo.Load(r);
		sptLogo.SetPosition(400, 300);
		sptLogo.SetPriority(100.0f);
		cScene.Add(&sptLogo);
	}

//	{
//		Writer w;
//		sptLogo.Write(w);
//		w.Save("out.sprite");
//	}

	{
		File f("sample.movie");
		Reader r(f);
		mvSample.Load(r);
		mvSample.SetPriority(200.0f);
		cScene.Add(&mvSample);
	}

//	{
//		Writer w;
//		mvSample.Write(w);
//		w.Save("out.movie");
//	}

//	cScene.SetPosition(100, 100);
//	sptLogo.SetParent(&cScene);

//	musTheme.Load("theme.ogg");
//	musTheme.SetVolume(.2f);
//	pSoundSystem->PlayMusic(&musTheme);

//	sfxSound.Load("sfx.ogg");
//	sfxSound.SetLoop(true);
//	sfxSound.SetVolume(1.0f);
//	pSoundSystem->Add(&sfxSound);

	sfxSound.Play();
	pScreen->FadeIn();

	return true;
}

bool TestBase::Update(f32 dt)
{
	UNUSED(dt)
	//pRendererDevice->DrawRect(100, 100, 50, 50, Color(255, 0, 255, 255), true);
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
