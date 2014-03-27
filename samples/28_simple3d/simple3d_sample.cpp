#include "simple3d_sample.h"

SceneNode *gScene;

enum
{
	kJobLoadScene
};

Simple3DSample::Simple3DSample()
	: cPres()
	, pObject(nullptr)
	, fElapsedTime(0.0f)
	, bInitialized(false)
{
}

Simple3DSample::~Simple3DSample()
{
}

bool Simple3DSample::Initialize()
{
	/* ------- Rendering Initialization ------- */
	IGameApp::Initialize();
	return cPres.Load("simple3d_sample.config", [&](Presentation *pres, Viewport *aborted) {
		if (!aborted)
		{
			pObject = pres->GetViewportByName("MainView")->GetScene()->GetChildByName("Panda");
			pSystem->AddListener(this);
			pInput->AddKeyboardListener(this);

			bInitialized = true;
		}
	});
}

bool Simple3DSample::Update(Seconds dt)
{
	if (!bInitialized)
		return true;

	fElapsedTime += dt;

	return true;
}

bool Simple3DSample::Shutdown()
{
	cPres.Unload();

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void Simple3DSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

bool Simple3DSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
	else if (k == eKey::F1)
		pResourceManager->Print();
	else if (k == eKey::F2)
		pResourceManager->GarbageCollect();

	return true;
}
