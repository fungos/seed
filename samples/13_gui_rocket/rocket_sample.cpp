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
	return cPres.Load("rocket_sample.config", [&](Presentation *pres, Renderer *) {
		pScene = pres->GetRendererByName("MainRenderer")->GetScene();

		pSystem->AddListener(this);
		pInput->AddKeyboardListener(this);

		this->InitializeGUI();
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

void RocketSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
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
