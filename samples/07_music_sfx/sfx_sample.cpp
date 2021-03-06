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
	return cPres.Load("sfx_sample.config", [&](Presentation *, Viewport *aborted) {
		if (!aborted)
		{
			pSystem->AddListener(this);
			pInput->AddKeyboardListener(this);
			pInput->AddPointerListener(this);

			musTheme.Load("theme.ogg");
			musTheme.SetVolume(.2f);
			pSoundSystem->PlayMusic(&musTheme);
		}
	});
}

bool SfxSample::Update(Seconds dt)
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

bool SfxSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
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

void SfxSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	UNUSED(ev)
}
