#include "renderer_sample.h"

SceneNode *gScene;

enum
{
	kJobLoadScene
};

RendererSample::RendererSample()
	: pImage(nullptr)
	, pCamera(nullptr)
	, cScene()
	, cViewport()
	, cRenderer()
	, vFrom()
	, vCurrent()
	, vTo()
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

	auto cb = [&](Job *self) {
		auto job = static_cast<FileLoader *>(self);

		if (self->GetState() == eJobState::Completed)
		{
			Reader r(job->pFile);
			gScene->Load(r);

			pCamera = (Camera *)gScene->GetChildByName("MainCamera");
			cViewport.SetCamera(pCamera);

			pImage = (ISceneObject *)gScene->GetChildByName("Panda");
			if (pImage)
				vFrom = vCurrent = pImage->GetPosition();
		}
		else if (job->GetState() == eJobState::Aborted)
		{
			// ...
		}
		Delete(self);
	};

	pJobManager->Add(New(FileLoader("renderer_sample.scene", cb)));
	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);
	pInput->AddPointerListener(this);

	return true;
}

bool RendererSample::Update(Seconds dt)
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
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
	else if (k == eKey::F1)
		pResourceManager->Print();
	else if (k == eKey::F2)
		pResourceManager->GarbageCollect();
}

void RendererSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	if (ev->GetReleased() == eInputButton::Left)
	{
		if (pImage)
			vFrom = pImage->GetPosition();

		vTo.setX(ev->GetX());
		vTo.setY(ev->GetY());
		vTo += pCamera->GetPosition();
		fElapsed = 0.0f;
	}
	else if (ev->GetReleased() == eInputButton::Right)
	{
		bRotate = !bRotate;
	}
	else if (ev->GetReleased() == eInputButton::Up)
	{
		fDir = 1.0f;
		bRotate = true;
	}
	else if (ev->GetReleased() == eInputButton::Down)
	{
		fDir = -1.0f;
		bRotate = true;
	}
}
