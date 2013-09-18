#include "image_sample.h"

ImageSample::ImageSample()
	: cPres()
	, pImage(NULL)
	, pCamera(NULL)
{
}

ImageSample::~ImageSample()
{
}

bool ImageSample::Initialize()
{
	IGameApp::Initialize();
	return cPres.Load("image_sample.config", this);
}

bool ImageSample::Update(f32 dt)
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

void ImageSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
}

void ImageSample::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pCamera = cPres.GetViewportByName("MainView")->GetCamera();
	pImage = (Image *)cPres.GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Panda");

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);
}
