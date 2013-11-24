#include "sprite_sample.h"

SpriteSample::SpriteSample()
	: cPres()
	, pObject(nullptr)
	, pCamera(nullptr)
	, vFrom()
	, vCurrent()
	, vTo()
	, fElapsed(0.0f)
	, fDir(1.0f)
	, bRotate(false)
	, bLoaded(false)
{
}

SpriteSample::~SpriteSample()
{
}

bool SpriteSample::Initialize()
{
	IGameApp::Initialize();
	return cPres.Load("sprite_sample.config", [&](Presentation *pres, Renderer *rend)
	{
		UNUSED(rend)
		pCamera = pres->GetViewportByName("MainView")->GetCamera();
		pObject = pres->GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Panda");

		pSystem->AddListener(this);
		pInput->AddKeyboardListener(this);
		pInput->AddPointerListener(this);

		bLoaded = true;
	});
}

bool SpriteSample::Update(Seconds dt)
{
	if (!bLoaded || !pObject)
		return true;

	fElapsed += dt;
	if (fElapsed > 1.0f)
		fElapsed = 1.0f;

	vCurrent = ((1.f - fElapsed) * vFrom) + (fElapsed * vTo);
	pObject->SetPosition(vCurrent);
	if (bRotate)
		pObject->SetRotation(pObject->GetRotation() + fDir);

	return true;
}

bool SpriteSample::Shutdown()
{
	cPres.Unload();
	pInput->RemovePointerListener(this);
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void SpriteSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void SpriteSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
	else if (k == eKey::F1)
		pResourceManager->Print();
	else if (k == eKey::F2)
		pResourceManager->GarbageCollect();
}

void SpriteSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	if (!pCamera)
		return;

	if (ev->GetReleased() == eInputButton::MouseLeft)
	{
		if (pObject)
			vFrom = pObject->GetPosition();

		vTo.setX(f32(ev->GetX()));
		vTo.setY(f32(ev->GetY()));
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
