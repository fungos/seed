#include "testbaseogles2.h"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>

SceneNode *gScene;

enum
{
	kJobLoadEmitter,
	kJobLoadAnim,
	kJobLoadScene
};

TestBaseOGLES2::TestBaseOGLES2()
{
}

TestBaseOGLES2::~TestBaseOGLES2()
{
}

bool TestBaseOGLES2::Initialize()
{
	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	/* ------- Rendering Initialization ------- */
    
    pShaderManager->Add("Simple", New(GLSLES120ShaderProgram("Simple")));
    
    pShaderManager->AttachShader("SimpleVertex", New(GLSLES120Shader(ShaderTypeVertex)));
    pShaderManager->AttachShader("SimpleFragment", New(GLSLES120Shader(ShaderTypeFragment)));
    pShaderManager->LoadShaderSource("SimpleVertex", "shaders/simple.vs", pResourceManager);
    pShaderManager->LoadShaderSource("SimpleFragment", "shaders/simple.fs", pResourceManager);
    
    pShaderManager->CompileShader("SimpleVertex");
    pShaderManager->CompileShader("SimpleFragment");
    pShaderManager->AttachShaderToProgram("Simple", "SimpleVertex");
    pShaderManager->AttachShaderToProgram("Simple", "SimpleFragment");
    
    pShaderManager->BindAttribute("Simple", 0, "vPosition");    
   
    pShaderManager->LinkShaderProgram("Simple");
    
	cScene.SetZ(0);
	cRenderer.SetScene(&cScene);

	cViewport.SetHeight(pScreen->GetHeight());
	cViewport.SetWidth(pScreen->GetWidth());
	cViewport.SetCamera(&cCamera);
	cViewport.SetRenderer(&cRenderer);

	pViewManager->Add(&cViewport);
	pRendererManager->Add(&cRenderer);
	pSceneManager->Add(&cScene);
	gScene = &cScene;
	/* ------- Rendering Initialization ------- */

	pJobManager->Add(New(FileLoader("scenes/main.scene", kJobLoadScene, this)));
	cScene.sName = "Main";

	cCameraTex.sName = "Camera to Texture";
	cCameraTex.SetTexture(&cRenderTarget);

	cCamera.sName = "Camera";
	cCamera.SetPosition(-400, -300);
	cCamera.Update(0.0f);

//	gScene->Add(&cCamera);
//	pScreen->FadeIn();

	return true;
}

bool TestBaseOGLES2::Update(f32 dt)
{
	UNUSED(dt)
	//pRendererDevice->DrawRect(100, 100, 50, 50, Color(255, 0, 255, 255), true);
    

       
	return true;
}

bool TestBaseOGLES2::Shutdown()
{
	pResourceManager->GarbageCollect();

	pSceneManager->Reset();
	pRendererManager->Reset();
	pViewManager->Reset();

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	gScene->Unload();
	IGameApp::Shutdown();

	return true;
}

bool TestBaseOGLES2::Reset()
{
	return true;
}

void TestBaseOGLES2::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void TestBaseOGLES2::OnInputKeyboardRelease(const EventInputKeyboard *ev)
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
}

void TestBaseOGLES2::OnJobCompleted(const EventJob *ev)
{
	switch (ev->GetName())
	{
		case kJobLoadScene:
		{
			/*FileLoader *job = (FileLoader *)ev->GetJob();
			Reader r(job->pFile);
			gScene->Load(r);
			Delete(job);*/
		}
		break;
	}
}

void TestBaseOGLES2::OnJobAborted(const EventJob *ev)
{
	Job *job = ev->GetJob();
	Log("Aborting job");
	Delete(job);
}
