#include "tile_sample.h"

SceneNode *gScene;

TileSample::TileSample()
	: pPlayer(NULL)
	, pCamera(NULL)
	, cPres()
	, vDir()
	, fSpeed(5.0f)
	, bLoaded(false)
{
}

TileSample::~TileSample()
{
}

bool TileSample::Initialize()
{
	return cPres.Load("tile_sample.config", this);
}

bool TileSample::Update(f32 dt)
{
	UNUSED(dt)

	if (bLoaded)
	{
		pPlayer->AddPosition(vDir);
	}

	return true;
}

bool TileSample::Shutdown()
{
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	cPres.Unload();
	return IGameApp::Shutdown();
}

void TileSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void TileSample::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	u32 v = ev->GetKey().GetValue();

	switch (v)
	{
		case Seed::KeyW:
			vDir.setY(-fSpeed);
		break;
		case Seed::KeyS:
			vDir.setY(fSpeed);
		break;
		case Seed::KeyA:
			vDir.setX(-fSpeed);
		break;
		case Seed::KeyD:
			vDir.setX(fSpeed);
		break;
		default: return;
	}
}

void TileSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();

	u32 v = ev->GetKey().GetValue();
	switch (v)
	{
		case Seed::KeyW:
			vDir.setY(0.0f);
		break;
		case Seed::KeyS:
			vDir.setY(0.0f);
		break;
		case Seed::KeyA:
			vDir.setX(0.0f);
		break;
		case Seed::KeyD:
			vDir.setX(0.0f);
		break;
		default: return;
	}
}

void TileSample::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	gScene = cPres.GetRendererByName("MainRenderer")->GetScene();
	pCamera = cPres.GetViewportByName("MainView")->GetCamera();

	pPlayer = (ISceneObject *)gScene->GetChildByName("Player");
	pCamera->SetParent(pPlayer);
	vDir = Vector3f(0.0f, 0.0f, 0.0f);

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	bLoaded = true;
}
