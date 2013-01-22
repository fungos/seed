#include "gameflow.h"

#include <Rocket/Debugger.h>
#include <Rocket/Controls.h>

GameFlow *gFlow = NULL;

GameFlow::GameFlow()
	: pScene(NULL)
	, pRocket(NULL)
	, pContext(NULL)
	, pDoc(NULL)
{
	gFlow = this;
}

GameFlow::~GameFlow()
{
	gFlow = NULL;
}

bool GameFlow::Initialize()
{
	bool init = cPres.Load("game.config", this);

	// Create the transitions
	cMenuToGame.Initialize(&cMenu, &cOnGame, &cGame);
	cMenuToOptions.Initialize(&cMenu, &cOnOptions, &cOptions);
	cMenuToCredits.Initialize(&cMenu, &cOnCredits, &cCredits);
	cOptionsToMenu.Initialize(&cOptions, &cOnMenu, &cMenu);
	cCreditsToMenu.Initialize(&cCredits, &cOnMenu, &cMenu);
	cGameToMenu.Initialize(&cGame, &cOnMenu, &cMenu);

	// Create the State Machine.
	cFlow.RegisterTransition(&cMenuToGame);
	cFlow.RegisterTransition(&cMenuToOptions);
	cFlow.RegisterTransition(&cMenuToCredits);
	cFlow.RegisterTransition(&cOptionsToMenu);
	cFlow.RegisterTransition(&cCreditsToMenu);
	cFlow.RegisterTransition(&cGameToMenu);

	IGameApp::Initialize();

	return init;
}

bool GameFlow::Update(f32 dt)
{
	cFlow.Update(dt);

	return true;
}

bool GameFlow::Shutdown()
{
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	this->ReleaseGUI();
	cPres.Unload();

	return IGameApp::Shutdown();
}

void GameFlow::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void GameFlow::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
	else if (k == Seed::KeyF5)
		this->ReloadGUI();
	else if (k == Seed::KeyF10)
		Rocket::Debugger::SetVisible(!Rocket::Debugger::IsVisible());
}

void GameFlow::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	pScene = cPres.GetRendererByName("MainRenderer")->GetScene();

	this->InitializeGUI();
	cFlow.Initialize(&cMenu);
}


bool GameFlow::UnloadGUI()
{
	Log("Unloading GUI Document");
	if (pDoc)
	{
		pDoc->Hide();
		pContext->UnloadDocument(pDoc);
		pDoc->RemoveReference();
		pDoc = NULL;
	}

	return true;
}

bool GameFlow::ReloadGUI()
{
	return this->LoadGUI(sDocument);
}

bool GameFlow::LoadGUI(const String &doc)
{
	if (this->UnloadGUI())
	{
		Log("Loading GUI Document");
		pDoc = pContext->LoadDocument(doc.c_str());
		if (pDoc != NULL)
		{
			Rocket::Core::Element *title = pDoc->GetElementById("title");
			if (title != NULL)
				title->SetInnerRML(pDoc->GetTitle());

			pDoc->Focus();
			pDoc->Show();
		}

		sDocument = doc;
	}

	return true;
}

bool GameFlow::InitializeGUI()
{
	pRocket = New(RocketInterface());
	pRocket->sName = "GUI";
	Rocket::Core::SetRenderInterface(pRocket);
	Rocket::Core::SetFileInterface(pRocket);
	Rocket::Core::SetSystemInterface(pRocket);
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
	pRocket->SetCurrentContext(pContext);
	pInput->AddKeyboardListener(pRocket);
	pInput->AddPointerListener(pRocket);
	pScene->Add(pRocket);

	return true;
}

void GameFlow::ReleaseGUI()
{
	pInput->RemovePointerListener(pRocket);
	pInput->RemoveKeyboardListener(pRocket);

	pContext->UnloadAllDocuments();
	pDoc->RemoveReference();
	pContext->RemoveReference();

	Rocket::Core::Shutdown();

	pScene->Remove(pRocket);
	Delete(pRocket);
}

// MainMenu
MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::OnStart(IObject *data)
{
	UNUSED(data)
	Log("Entering MainMenu State");

	gFlow->LoadGUI("mainmenu.rml");
}

void MainMenu::OnUpdate(f32 dt)
{
	UNUSED(dt)
}

void MainMenu::OnStop(IObject *data)
{
	UNUSED(data)
	Log("Exiting MainMenu State");

	gFlow->UnloadGUI();
}


// Credits
Credits::Credits()
{
}

Credits::~Credits()
{
}

void Credits::OnStart(IObject *data)
{
	UNUSED(data)
	Log("Entering Credits State");

	gFlow->LoadGUI("credits.rml");
}

void Credits::OnUpdate(f32 dt)
{
	UNUSED(dt)
}

void Credits::OnStop(IObject *data)
{
	UNUSED(data)
	Log("Exiting Credits State");

	gFlow->UnloadGUI();
}
