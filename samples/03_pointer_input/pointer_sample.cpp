#include "pointer_sample.h"

PointerSample::PointerSample()
	: cPres()
	, pObject(nullptr)
	, pCamera(nullptr)
	, vFrom(0.0f, 0.0f, 0.0f)
	, vCurrent(0.0f, 0.0f, 0.0f)
	, vTo(0.0f, 0.0f, 0.0f)
	, fElapsed(0.0f)
	, fDir(1.0f)
	, bRotate(false)
{
}

PointerSample::~PointerSample()
{
}

bool PointerSample::Initialize()
{
	IGameApp::Initialize();
	return cPres.Load("pointer_sample.config", [&](Presentation *pres, Viewport *aborted) {
		if (!aborted)
		{
			auto vp = pres->GetViewportByName("MainView");
			pCamera = vp->GetCamera();
			pObject = vp->GetScene()->GetChildByName("Panda");

			pSystem->AddListener(this);
			pInput->AddKeyboardListener(this);
			pInput->AddPointerListener(this);
		}
	});
}

bool PointerSample::Update(Seconds dt)
{
	if (pObject)
	{
		fElapsed += dt;
		if (fElapsed > 1.0f)
			fElapsed = 1.0f;

		vCurrent = ((1.f - fElapsed) * vFrom) + (fElapsed * vTo);
		pObject->SetPosition(vCurrent);
		if (bRotate)
			pObject->SetRotation(pObject->GetRotation() + fDir);
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

bool PointerSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
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

void PointerSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	if (!pCamera)
		return;

	if (ev->GetReleased() == eInputButton::MouseLeft)
	{
		if (pObject)
			vFrom = pObject->GetPosition();

		vTo.x = f32(ev->GetX());
		vTo.y = f32(ev->GetY());
		vTo += pCamera->GetPosition();
		fElapsed = 0.0f;
	}
	else if (ev->GetReleased() == eInputButton::MouseRight)
	{
		bRotate = !bRotate;
	}
	else if (ev->GetReleased() == eInputButton::MouseUp)
	{
		fDir = 1.0f;
		bRotate = true;
	}
	else if (ev->GetReleased() == eInputButton::MouseDown)
	{
		fDir = -1.0f;
		bRotate = true;
	}
}
