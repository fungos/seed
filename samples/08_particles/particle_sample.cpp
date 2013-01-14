#include "particle_sample.h"

ParticleSample::ParticleSample()
	: cPres()
	, pEmitter(NULL)
	, pSprite(NULL)
	, iAnimation(0)
{
}

ParticleSample::~ParticleSample()
{
}

bool ParticleSample::Initialize()
{
	IGameApp::Initialize();
	return cPres.Load("particle_sample.config", this);
}

bool ParticleSample::Update(f32 dt)
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

void ParticleSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pLeakReport->Print();
	else if (k == Seed::KeyF3)
		pResourceManager->GarbageCollect();
	else if (k == Seed::KeyLeft)
		iAnimation--;
	else if (k == Seed::KeyRight)
		iAnimation++;

	if (pSprite)
	{
		if (iAnimation < 0)
			iAnimation = pSprite->GetAnimationCount() - 1;
		else if (iAnimation >= (s32)pSprite->GetAnimationCount())
			iAnimation = 0;

		pSprite->SetAnimation(iAnimation);
	}
}


void ParticleSample::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pEmitter = (ParticleEmitter *)cPres.GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Emitter");

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);
}
