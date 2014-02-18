#include "prefabs_sample.h"

PrefabsSample::PrefabsSample()
	: cPres()
	, pEmitter(nullptr)
	, pSprite(nullptr)
	, iAnimation(0)
{
}

PrefabsSample::~PrefabsSample()
{
}

bool PrefabsSample::Initialize()
{
	IGameApp::Initialize();
	return cPres.Load("prefabs_sample.config", [&](Presentation *pres, Renderer *) {
		pEmitter = (ParticleEmitter *)pres->GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Emitter");
		pSystem->AddListener(this);
		pInput->AddKeyboardListener(this);
	});
}

bool PrefabsSample::Update(Seconds dt)
{
	UNUSED(dt)
	if (!pSprite && pEmitter)
	{
		pSprite = pEmitter->GetSprite();
		if (pSprite)
			iAnimation = pSprite->GetCurrentAnimation();
	}

	return true;
}

bool PrefabsSample::Shutdown()
{
	cPres.Unload();

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void PrefabsSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

bool PrefabsSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
	else if (k == eKey::F1)
		pResourceManager->Print();
	else if (k == eKey::F3)
		pResourceManager->GarbageCollect();
	else if (k == eKey::Left)
		iAnimation--;
	else if (k == eKey::Right)
		iAnimation++;

	if (pSprite)
	{
		if (iAnimation < 0)
			iAnimation = pSprite->GetAnimationCount() - 1;
		else if (iAnimation >= (s32)pSprite->GetAnimationCount())
			iAnimation = 0;

		pSprite->SetAnimation(iAnimation);
	}

	return true;
}
