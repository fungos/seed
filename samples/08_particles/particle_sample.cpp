#include "particle_sample.h"

ParticleSample::ParticleSample()
	: cPres()
	, pObject(nullptr)
	, pEmitter(nullptr)
	, iAnimation(0)
{
}

ParticleSample::~ParticleSample()
{
}

bool ParticleSample::Initialize()
{
	IGameApp::Initialize();
	return cPres.Load("particle_sample.config", [&](Presentation *pres, Renderer *) {
		pEmitter = (ParticleEmitter *)pres->GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Emitter");
		pSystem->AddListener(this);
		pInput->AddKeyboardListener(this);
	});
}

bool ParticleSample::Update(Seconds dt)
{
	UNUSED(dt)
	if (!pObject && pEmitter)
	{
		pObject = pEmitter->GetSprite();
		if (pObject)
			iAnimation = pObject->GetCurrentAnimation();
	}

	return true;
}

bool ParticleSample::Shutdown()
{
	cPres.Unload();

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void ParticleSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

bool ParticleSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
	else if (k == eKey::F1)
		pResourceManager->Print();
	else if (k == eKey::F2)
		LeakReportPrint();
	else if (k == eKey::F3)
		pResourceManager->GarbageCollect();
	else if (k == eKey::Left)
		iAnimation--;
	else if (k == eKey::Right)
		iAnimation++;

	if (pObject)
	{
		if (iAnimation < 0)
			iAnimation = pObject->GetAnimationCount() - 1;
		else if (iAnimation >= (s32)pObject->GetAnimationCount())
			iAnimation = 0;

		pObject->SetAnimation(iAnimation);
	}

	return true;
}
