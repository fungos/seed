#include "shader_sample.h"

SceneNode *gScene;

enum
{
	kJobLoadScene
};

ShaderSample::ShaderSample()
	: cPres()
	, pObject(nullptr)
	, pProgram(nullptr)
	, pVertexShader(nullptr)
	, pPixelShader(nullptr)
{
}

ShaderSample::~ShaderSample()
{
}

bool ShaderSample::Initialize()
{
	/* ------- Rendering Initialization ------- */
	pProgram = sdNew(OGL20ShaderProgram("Simple"));
	pVertexShader = sdNew(OGL20Shader(eShaderType::Vertex));
	pPixelShader = sdNew(OGL20Shader(eShaderType::Pixel));

	IGameApp::Initialize();
	return cPres.Load("shader_sample.config", [&](Presentation *pres, Renderer *) {
		pObject = pres->GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Panda");
		pSystem->AddListener(this);
		pInput->AddKeyboardListener(this);

		pShaderManager->Add("Simple", pProgram);
		pShaderManager->AttachShader("SimpleVertex", pVertexShader);
		pShaderManager->AttachShader("SimpleFragment", pPixelShader);
		pShaderManager->LoadShaderSource("SimpleVertex", "simple.vs", pResourceManager);
		pShaderManager->LoadShaderSource("SimpleFragment", "simple.fs", pResourceManager);

		pShaderManager->CompileShader("SimpleVertex");
		pShaderManager->CompileShader("SimpleFragment");
		pShaderManager->AttachShaderToProgram("Simple", "SimpleVertex");
		pShaderManager->AttachShaderToProgram("Simple", "SimpleFragment");

		pShaderManager->BindAttribute("Simple", 0, "vPosition");
		pShaderManager->BindAttribute("Simple", 1, "vTexCoord");
		pShaderManager->BindAttribute("Simple", 2, "vColor");
		pShaderManager->LinkShaderProgram("Simple");

		pShaderManager->SetTexture("Simple", 0, "mTexture");
		pShaderManager->Validate("Simple");
		pShaderManager->GetShaderProgram("Simple")->Use();
	});
}

bool ShaderSample::Update(Seconds dt)
{
	UNUSED(dt)
	return true;
}

bool ShaderSample::Shutdown()
{
	cPres.Unload();

	pShaderManager->Reset();
	
	sdDelete(pPixelShader);
	sdDelete(pVertexShader);
	sdDelete(pProgram);

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
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
