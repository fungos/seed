#include "sfx_sample.h"

SfxSample::SfxSample()
	: cPres()
	, musTheme()
{
}

SfxSample::~SfxSample()
{
}

bool SfxSample::Initialize()
{
	IGameApp::Initialize();
	return cPres.Load("sfx_sample.config", this);
}

bool SfxSample::Update(f32 dt)
{
	UNUSED(dt)
	return true;
}

bool SfxSample::Shutdown()
{
	musTheme.Unload();
	cPres.Unload();
	pInput->RemovePointerListener(this);
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void SfxSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void SfxSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
}

void SfxSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	UNUSED(ev)
}

void SfxSample::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);
	pInput->AddPointerListener(this);

	musTheme.Load("theme.ogg");
	musTheme.SetVolume(.2f);
	pSoundSystem->PlayMusic(&musTheme);
}
