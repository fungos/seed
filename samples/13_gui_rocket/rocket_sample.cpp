<<<<<<< HEAD:samples/rocket/rocket_sample.cpp
#include "rocket_sample.h"

#include <Rocket/Debugger.h>
#include <Rocket/Controls.h>

SceneNode *gScene;

enum
{
	kJobLoadScene
};

RocketSample::RocketSample()
	: pI(NULL)
	, pContext(NULL)
	, pDoc(NULL)
{
}

RocketSample::~RocketSample()
{
}

bool RocketSample::Initialize()
{
	/* ------- Rendering Initialization ------- */
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

	pJobManager->Add(New(FileLoader("rocket_sample.scene", kJobLoadScene, this)));
	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	return this->InitializeGUI();
}

bool RocketSample::Shutdown()
{
	this->ReleaseGUI();

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	pSceneManager->Reset();
	pRendererManager->Reset();
	pViewManager->Reset();
	gScene->Unload();

	return IGameApp::Shutdown();
}

void RocketSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void RocketSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
	else if (k == Seed::KeyF12)
		Rocket::Debugger::SetVisible(!Rocket::Debugger::IsVisible());
}

void RocketSample::OnJobCompleted(const EventJob *ev)
{
	switch (ev->GetName())
	{
		case kJobLoadScene:
		{
			FileLoader *job = (FileLoader *)ev->GetJob();
			Reader r(job->pFile);
			gScene->Load(r);
			Delete(job);

			pCamera = (Camera *)gScene->GetChildByName("MainCamera");
			cViewport.SetCamera(pCamera);

			gScene->Add(pI);
		}
		break;
	}
}

void RocketSample::OnJobAborted(const EventJob *ev)
{
	Job *job = ev->GetJob();
	Delete(job);
}

bool RocketSample::InitializeGUI()
{
	pI = New(RocketInterface());
	pI->sName = "RocketGUI";
	Rocket::Core::SetRenderInterface(pI);
	Rocket::Core::SetFileInterface(pI);
	Rocket::Core::SetSystemInterface(pI);
	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();

	pContext = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(pScreen->GetWidth(), pScreen->GetHeight()));
	if (pContext == NULL)
	{
		Rocket::Core::Shutdown();
		return false;
	}

	Rocket::Core::String fonts[4];
	fonts[0] = "Delicious-Roman.otf";
	fonts[1] = "Delicious-Italic.otf";
	fonts[2] = "Delicious-Bold.otf";
	fonts[3] = "Delicious-BoldItalic.otf";

	for (unsigned int i = 0; i < sizeof(fonts) / sizeof(Rocket::Core::String); i++)
		Rocket::Core::FontDatabase::LoadFontFace(fonts[i]);

	Rocket::Debugger::Initialise(pContext);
	pDoc = pContext->LoadDocument("rocket_menu.rml");
	if (pDoc != NULL)
		pDoc->Show();

	pI->SetCurrentContext(pContext);
	pInput->AddKeyboardListener(pI);
	pInput->AddPointerListener(pI);

	return true;
}

void RocketSample::ReleaseGUI()
{
	pInput->RemovePointerListener(pI);
	pInput->RemoveKeyboardListener(pI);

	pContext->UnloadAllDocuments();
	pDoc->RemoveReference();
	pContext->RemoveReference();

	Rocket::Core::Shutdown();

	gScene->Remove(pI);
	Delete(pI);
}
=======
#include "rocket_sample.h"

#include <Rocket/Debugger.h>
#include <Rocket/Controls.h>

RocketSample::RocketSample()
	: pScene(nullptr)
	, pI(nullptr)
	, pContext(nullptr)
	, pDoc(nullptr)
	, cPres()
	, sDocument("")
	, bLoaded(false)
{
}

RocketSample::~RocketSample()
{
}

bool RocketSample::Initialize()
{
	IGameApp::Initialize();
	return cPres.Load("rocket_sample.config", [&](Presentation *pres, Viewport *aborted) {
		if (!aborted)
		{
			pScene = pres->GetViewportByName("MainView")->GetScene();

			pSystem->AddListener(this);
			pInput->AddKeyboardListener(this);

			this->InitializeGUI();
		}
	});
}

bool RocketSample::Shutdown()
{
	this->ReleaseGUI();
	cPres.Unload();

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void RocketSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

bool RocketSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
	else if (k == eKey::F1)
		pResourceManager->Print();
	else if (k == eKey::F2)
		pResourceManager->GarbageCollect();
	else if (k == eKey::F5)
		this->ReloadGUI();
	else if (k == eKey::F12)
		Rocket::Debugger::SetVisible(!Rocket::Debugger::IsVisible());

	return true;
}

bool RocketSample::UnloadGUI()
{
	Log("Unloading GUI Document");
	if (pDoc)
	{
		pDoc->Hide();
		pContext->UnloadDocument(pDoc);
		pDoc->RemoveReference();
		pDoc = nullptr;
	}

	bLoaded = false;
	return true;
}

bool RocketSample::ReloadGUI()
{
	return this->LoadGUI(sDocument);
}

bool RocketSample::LoadGUI(const String &doc)
{
	if (this->UnloadGUI())
	{
		Log("Loading GUI Document");
		pDoc = pContext->LoadDocument(doc.c_str());
		if (pDoc != nullptr)
			pDoc->Show();

		sDocument = doc;
		bLoaded = true;
	}

	return bLoaded;
}

bool RocketSample::InitializeGUI()
{
	pI = sdNew(RocketInterface());
	pI->sName = "RocketGUI";
	Rocket::Core::SetRenderInterface(pI);
	Rocket::Core::SetFileInterface(pI);
	Rocket::Core::SetSystemInterface(pI);
	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();

	pContext = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(pScreen->GetWidth(), pScreen->GetHeight()));
	if (pContext == nullptr)
	{
		Rocket::Core::Shutdown();
		return false;
	}

	Rocket::Core::String fonts[4];
	fonts[0] = "Delicious-Roman.otf";
	fonts[1] = "Delicious-Italic.otf";
	fonts[2] = "Delicious-Bold.otf";
	fonts[3] = "Delicious-BoldItalic.otf";

	for (unsigned int i = 0; i < sizeof(fonts) / sizeof(Rocket::Core::String); i++)
		Rocket::Core::FontDatabase::LoadFontFace(fonts[i]);

	Rocket::Debugger::Initialise(pContext);
	pI->SetCurrentContext(pContext);
	pInput->AddKeyboardListener(pI);
	pInput->AddPointerListener(pI);
	pScene->Add(pI);

	return this->LoadGUI("rocket_menu.rml");
}

void RocketSample::ReleaseGUI()
{
	pInput->RemovePointerListener(pI);
	pInput->RemoveKeyboardListener(pI);

	pContext->UnloadAllDocuments();
	pDoc->RemoveReference();
	pContext->RemoveReference();

	Rocket::Core::Shutdown();
	bLoaded = false;

	pScene->Remove(pI);
	sdDelete(pI);
}
>>>>>>> develop:samples/13_gui_rocket/rocket_sample.cpp
