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
	return cPres.Load("pres1.config", this);
}

bool PresentationSample::Update(f32 dt)
{
	UNUSED(dt)
	return true;
}

bool PresentationSample::Shutdown()
{
	cPres.Unload();

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
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
	else if (k == eKey::F1)
		pResourceManager->Print();
	else if (k == eKey::F2)
		pResourceManager->GarbageCollect();
}

void PresentationSample::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);
}
