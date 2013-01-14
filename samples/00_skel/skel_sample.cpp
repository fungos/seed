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

bool SkelSample::Update(f32 dt)
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

void SkelSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
}
