#include "pres_sample.h"

PresentationSample::PresentationSample()
	: cPres()
{
}

PresentationSample::~PresentationSample()
{
}

bool PresentationSample::Initialize()
{
	return cPres.Load("pres2.config", this);
}

bool PresentationSample::Update(f32 dt)
{
	UNUSED(dt)
	return true;
}

bool PresentationSample::Shutdown()
{
	cPres.Unload();

	pInput->RemovePointerListener(this);
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void PresentationSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void PresentationSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
}

void PresentationSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	if (ev->GetReleased() == Seed::ButtonLeft)
	{
		Viewport *vp = pViewManager->GetViewportAt(ev->GetX(), ev->GetY());
		Log("Click at viewport %s", vp->sName.c_str());
		Log("Click position %d,%d position inside viewport is at %d,%d", ev->GetX(), ev->GetY(), ev->GetX() - vp->GetX(), ev->GetY() - vp->GetY());
	}
}

void PresentationSample::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);
	pInput->AddPointerListener(this);
}
