#include "renderer_sample.h"

SceneNode *gScene;

enum
{
	kJobLoadScene
};

RendererSample::RendererSample()
	: pImage(NULL)
	, fElapsed(0.0f)
	, fDir(1.0f)
	, bRotate(false)
{
}

RendererSample::~RendererSample()
{
}

bool RendererSample::Initialize()
{
	/* ------- Rendering Initialization ------- */
	cRenderer.SetScene(&cScene);

	cViewport.sName = "MainView";
	cRenderer.sName = "MainRenderer";

	cViewport.SetHeight(pScreen->GetHeight());
	cViewport.SetWidth(pScreen->GetWidth());
	cViewport.SetRenderer(&cRenderer);

	pViewManager->Add(&cViewport);
	pRendererManager->Add(&cRenderer);
	pSceneManager->Add(&cScene);
	gScene = &cScene;
	/* ------- Rendering Initialization ------- */

	pJobManager->Add(New(FileLoader("renderer_sample.scene", kJobLoadScene, this)));
	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);
	pInput->AddPointerListener(this);

	return true;
}

bool RendererSample::Update(f32 dt)
{
	if (pImage)
	{
		fElapsed += dt;
		if (fElapsed > 1.0f)
			fElapsed = 1.0f;

		vCurrent = ((1.f - fElapsed) * vFrom) + (fElapsed * vTo);
		pImage->SetPosition(vCurrent);
		if (bRotate)
			pImage->SetRotation(pImage->GetRotation() + fDir);
	}

	return true;
}

bool RendererSample::Shutdown()
{
	pInput->RemovePointerListener(this);
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	pSceneManager->Reset();
	pRendererManager->Reset();
	pViewManager->Reset();
	gScene->Unload();

	IGameApp::Shutdown();

	return true;
}

void RendererSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void RendererSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
}

void RendererSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	if (ev->GetReleased() == Seed::ButtonLeft)
	{
		if (pImage)
			vFrom = pImage->GetPosition();

		vTo.setX(ev->GetX());
		vTo.setY(ev->GetY());
		vTo += pCamera->GetPosition();
		fElapsed = 0.0f;
	}
	else if (ev->GetReleased() == Seed::ButtonRight)
	{
		bRotate = !bRotate;
	}
	else if (ev->GetReleased() == Seed::ButtonUp)
	{
		fDir = 1.0f;
		bRotate = true;
	}
	else if (ev->GetReleased() == Seed::ButtonDown)
	{
		fDir = -1.0f;
		bRotate = true;
	}
}

void RendererSample::OnJobCompleted(const EventJob *ev)
{
	switch (ev->GetName())
	{
		case kJobLoadScene:
		{
			FileLoader *job = (FileLoader *)ev->GetJob();
			Reader r(job->pFile);
			gScene->Load(r);
			Delete(job);

			pCamera = (Camera *)gScene->GetChildByName("MainCamera");
			cViewport.SetCamera(pCamera);

			pImage = (ISceneObject *)gScene->GetChildByName("Panda");
			if (pImage)
				vFrom = vCurrent = pImage->GetPosition();
		}
		break;
	}
}

void RendererSample::OnJobAborted(const EventJob *ev)
{
	Job *job = ev->GetJob();
	Delete(job);
}
