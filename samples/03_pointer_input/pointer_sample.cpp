#include "pointer_sample.h"

PointerSample::PointerSample()
	: cPres()
	, pImage(NULL)
	, pCamera(NULL)
	, fElapsed(0.0f)
	, fDir(1.0f)
	, bRotate(false)
	, vFrom()
	, vCurrent()
	, vTo()
{
}

PointerSample::~PointerSample()
{
}

bool PointerSample::Initialize()
{
	IGameApp::Initialize();
	return cPres.Load("pointer_sample.config", this);
}

bool PointerSample::Update(f32 dt)
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

bool PointerSample::Shutdown()
{
	cPres.Unload();
	pInput->RemovePointerListener(this);
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void PointerSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void PointerSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
}

void PointerSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	if (!pCamera)
		return;

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

void PointerSample::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pCamera = cPres.GetViewportByName("MainView")->GetCamera();
	pImage = (Image *)cPres.GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Panda");

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);
	pInput->AddPointerListener(this);
}
