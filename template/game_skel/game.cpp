#include "game.h"
#include "gameflow.h"

enum
{
	kJobLoadScene
};

Game::Game(SceneNode *parent)
	: pPlayer(NULL)
	, pScene(parent)
{
}

Game::~Game()
{
}

bool Game::Initialize()
{
	gFlow->AddScene(pScene);
	pJobManager->Add(New(FileLoader("game.scene", kJobLoadScene, this)));
	return true;
}

bool Game::Update(f32 dt)
{
	UNUSED(dt)

	if (pPlayer)
	{
		pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
	}

	return true;
}

bool Game::Shutdown()
{
	gFlow->RemoveScene(pScene);
	pScene->Unload();
	pScene = NULL;

	pInput->RemoveKeyboardListener(this);

	return true;
}

void Game::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();
}

void Game::OnJobCompleted(const EventJob *ev)
{
	switch (ev->GetName())
	{
		case kJobLoadScene:
		{
			FileLoader *job = (FileLoader *)ev->GetJob();
			Reader r(job->pFile);
			pScene->Load(r);
			Log("Scene Name: %s len %d", pScene->sName.c_str(), pScene->Size());
			Delete(job);
			pPlayer = (ISceneObject *)pScene->GetChildByName("Player");
		}
		break;
	}
}

void Game::OnJobAborted(const EventJob *ev)
{
	Job *job = ev->GetJob();
	Delete(job);
}
