#include "skel_sample.h"

SkelSample::SkelSample()
{
}

SkelSample::~SkelSample()
{
}

bool SkelSample::Initialize()
{
	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	return IGameApp::Initialize();
}

bool SkelSample::Update(Seconds dt)
{
	UNUSED(dt)
	return true;
}

bool SkelSample::Shutdown()
{
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void SkelSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

bool SkelSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
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
