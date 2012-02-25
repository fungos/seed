#include "testbase.h"

ISceneNode *pScene;

TestBase::TestBase()
{
	cConfig.SetApplicationTitle("tests");
	cConfig.SetPublisherName("seed");
	cConfig.SetVideoMode(Seed::Video_800x600);
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

	Reader r(File("logo.sprite"));
	sptLogo.Load(r);
	cScene.Add(&sptLogo);
	pScreen->FadeIn();
	return true;
}

bool TestBase::Update(f32 dt)
{
	return true;
}

bool TestBase::Shutdown()
{
	sptLogo.Unload();

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
	pSystem->Shutdown();
}

void TestBase::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
}
