#include "image_sample.h"

ImageSample::ImageSample()
	: cPres()
	, pObject(nullptr)
	, pCamera(nullptr)
{
}

ImageSample::~ImageSample()
{
}

bool ImageSample::Initialize()
{
	IGameApp::Initialize();
	return cPres.Load("image_sample.config", [&](Presentation *pres, Renderer *) {
		pCamera = pres->GetViewportByName("MainView")->GetCamera();
		pObject = pres->GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Panda");

		pSystem->AddListener(this);
		pInput->AddKeyboardListener(this);
	});
}

bool ImageSample::Update(Seconds dt)
{
	UNUSED(dt)
	return true;
}

bool ImageSample::Shutdown()
{
	cPres.Unload();
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void ImageSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

bool ImageSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
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
