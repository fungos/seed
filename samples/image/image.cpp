#include "image.h"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>

SceneNode *gScene;

enum
{
	kJobLoadScene
};

ImageSample::ImageSample()
	: pImage(NULL)
{
}

ImageSample::~ImageSample()
{
}

bool ImageSample::Initialize()
{
	/* ------- Rendering Initialization ------- */
	cRenderer.Add(&cScene);

	cViewport.SetHeight(pScreen->GetHeight());
	cViewport.SetWidth(pScreen->GetWidth());
	cViewport.SetCamera(&cCamera);
	cViewport.SetRenderer(&cRenderer);

	pViewManager->Add(&cViewport);
	pRendererManager->Add(&cRenderer);
	pSceneManager->Add(&cScene);
	gScene = &cScene;
	/* ------- Rendering Initialization ------- */

	pJobManager->Add(New(FileLoader("image_sample.scene", kJobLoadScene, this)));
	cCamera.sName = "Camera";
	cCamera.SetPosition(-400, -300);
	cCamera.Update(0.0f);

	pSystem->AddListener(this);

	pInput->AddKeyboardListener(this);
	pInput->AddPointerListener(this);

	return true;
}

bool ImageSample::Update(f32 dt)
{
	UNUSED(dt)

	if (pImage)
	{
		Vector3f diff = vTo - vCurrent;
		vCurrent = vCurrent + (dt * diff);
		vCurrent.setX((int)vCurrent.getX());
		vCurrent.setY((int)vCurrent.getY());

		Log("Position: %f, %f, %f", vCurrent.getX(), vCurrent.getY(), vCurrent.getZ());
		pImage->SetPosition(vCurrent);
	}

	return true;
}

bool ImageSample::Shutdown()
{
	pInput->RemovePointerListener(this);
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	pSceneManager->Reset();
	pRendererManager->Reset();
	pViewManager->Reset();
	gScene->Unload();

	IGameApp::Shutdown();

	return true;
}

bool ImageSample::Reset()
{
	return true;
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
}

void ImageSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	vTo.setX(ev->GetX());
	vTo.setY(ev->GetY());
	vTo += cCamera.GetPosition();
}

void ImageSample::OnJobCompleted(const EventJob *ev)
{
	switch (ev->GetName())
	{
		case kJobLoadScene:
		{
			FileLoader *job = (FileLoader *)ev->GetJob();
			Reader r(job->pFile->GetData());
			gScene->Load(r);
			Delete(job);

			pImage = (Image *)gScene->GetChildByName("Panda");
			vFrom = vCurrent = pImage->GetPosition();
		}
		break;
	}
}

void ImageSample::OnJobAborted(const EventJob *ev)
{
	Job *job = ev->GetJob();
	Delete(job);
}
