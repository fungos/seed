#include "testbase.h"

ISceneNode *pScene;
ISceneNode *pSceneStatic;

TestBase::TestBase()
	: fInfoElapsedTime(0.0f)
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
	cSceneStatic.SetPriority(20000);

	cRenderer.Add(&cScene);
	cRenderer.Add(&cSceneStatic);

	cViewport.SetRenderer(&cRenderer);
	pViewManager->Add(&cViewport);
	pRendererManager->Add(&cRenderer);

	pSceneManager->Add(&cScene);
	pSceneManager->Add(&cSceneStatic);

	pScene = &cScene;
	pSceneStatic = &cSceneStatic;
	/* ------- Rendering Initialization ------- */

	Reader r(File("logo.sprite"));
	sptLogo.Load(r);
	sptLogo.SetPosition(0.0f, 0.0f);
	sptLogo.SetVisible(TRUE);
	cScene.Add(&sptLogo);
	pScreen->FadeIn();

	return true;
}

bool TestBase::Update(f32 dt)
{

	fInfoElapsedTime += dt;

	if (fInfoElapsedTime > 10.0f)
	{
		Log("Resource Memory: ");
		pResourceManager->PrintUsedMemoryByResource();

		fInfoElapsedTime = 0.0f;
	}

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

bool TestBase::HasError() const
{
	return false;
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
