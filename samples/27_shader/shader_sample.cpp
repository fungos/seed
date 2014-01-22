#include "shader_sample.h"

SceneNode *gScene;

enum
{
	kJobLoadScene
};

ShaderSample::ShaderSample()
	: pCamera(nullptr)
	, cScene()
	, cViewport()
	, cRenderer()
{
}

ShaderSample::~ShaderSample()
{
}

bool ShaderSample::Initialize()
{
	/* ------- Rendering Initialization ------- */
	pShaderManager->Add("Simple", sdNew(OGL20ShaderProgram("Simple")));

	pShaderManager->AttachShader("SimpleVertex", sdNew(OGL20Shader(eShaderType::Vertex)));
	pShaderManager->AttachShader("SimpleFragment", sdNew(OGL20Shader(eShaderType::Pixel)));
	pShaderManager->LoadShaderSource("SimpleVertex", "simple.vs", pResourceManager);
	pShaderManager->LoadShaderSource("SimpleFragment", "simple.fs", pResourceManager);

	pShaderManager->CompileShader("SimpleVertex");
	pShaderManager->CompileShader("SimpleFragment");
	pShaderManager->AttachShaderToProgram("Simple", "SimpleVertex");
	pShaderManager->AttachShaderToProgram("Simple", "SimpleFragment");

	pShaderManager->BindAttribute("Simple", 0, "vPosition");

	pShaderManager->LinkShaderProgram("Simple");

	pShaderManager->GetShaderProgram("Simple")->Use();

	cRenderer.SetScene(&cScene);

	cViewport.sName = "MainView";
	cRenderer.sName = "MainRenderer";

	cViewport.SetHeight(pScreen->GetHeight());
	cViewport.SetWidth(pScreen->GetWidth());
	cViewport.SetRenderer(&cRenderer);

	pViewManager->Add(&cViewport);
	pRendererManager->Add(&cRenderer);
	pSceneManager->Add(&cScene);

	gScene = &cScene;
	/* ------- Rendering Initialization ------- */

	auto cb = [&](Job *self) {
		auto job = static_cast<FileLoader *>(self);

		if (self->GetState() == eJobState::Completed)
		{
			Reader r(job->pFile);
			gScene->Load(r);

			pCamera = (Camera *)gScene->GetChildByName("MainCamera");
			cViewport.SetCamera(pCamera);
		}
		else if (job->GetState() == eJobState::Aborted)
		{
			// ...
		}
		sdDelete(self);
	};

	pJobManager->Add(sdNew(FileLoader("shader_sample.scene", cb)));
	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	return true;
}

bool ShaderSample::Update(Seconds dt)
{
	UNUSED(dt)
	return true;
}

bool ShaderSample::Shutdown()
{
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	pSceneManager->Reset();
	pRendererManager->Reset();
	pViewManager->Reset();
	gScene->Unload();

	IGameApp::Shutdown();

	return true;
}

void ShaderSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void ShaderSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
	else if (k == eKey::F1)
		pResourceManager->Print();
	else if (k == eKey::F2)
		pResourceManager->GarbageCollect();
}
