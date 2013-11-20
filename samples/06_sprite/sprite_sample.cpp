#include "sprite_sample.h"

SpriteSample::SpriteSample()
	: cPres()
	, pSprite(NULL)
	, pCamera(NULL)
	, vFrom()
	, vCurrent()
	, vTo()
	, fElapsed(0.0f)
	, fDir(1.0f)
	, bRotate(false)
{
}

SpriteSample::~SpriteSample()
{
}

bool SpriteSample::Initialize()
{
	IGameApp::Initialize();
	return cPres.Load("sprite_sample.config", this);
}

bool SpriteSample::Update(f32 dt)
{
	//#warning FIXME - Find a way to guarantee that all jobs finished before finishing scene loading (SceneObjectFactory.cpp)
	if (!pSprite && cPres.GetRendererByName("MainRenderer")->GetScene())
		pSprite = (Sprite *)cPres.GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Panda");

	if (pSprite)
	{
		fElapsed += dt;
		if (fElapsed > 1.0f)
			fElapsed = 1.0f;

		vCurrent = ((1.f - fElapsed) * vFrom) + (fElapsed * vTo);
		pSprite->SetPosition(vCurrent);
		if (bRotate)
			pSprite->SetRotation(pSprite->GetRotation() + fDir);
	}

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

	if (ev->GetReleased() == eInputButton::Left)
	{
		if (pSprite)
			vFrom = pSprite->GetPosition();

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

void SpriteSample::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pCamera = cPres.GetViewportByName("MainView")->GetCamera();
	pSprite = (Sprite *)cPres.GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Panda");

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);
	pInput->AddPointerListener(this);
}
