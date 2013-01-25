#include "game_scene.h"
#include "../gameflow.h"

enum
{
	kJobLoadScene
};

GameScene::GameScene(SceneNode *parent)
	: pPlayer(NULL)
	, pScene(parent)
{
}

GameScene::~GameScene()
{
}

bool GameScene::Initialize()
{
	gFlow->AddScene(pScene);
	pJobManager->Add(New(FileLoader("scenes/game.scene", kJobLoadScene, this)));
	return true;
}

bool GameScene::Update(f32 dt)
{
	UNUSED(dt)

	if (pPlayer)
	{
		pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
	}

	return true;
}

bool GameScene::Shutdown()
{
	gFlow->RemoveScene(pScene);
	pScene->Unload();
	pScene = NULL;

	pInput->RemoveKeyboardListener(this);

	return true;
}

void GameScene::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();
}

void GameScene::OnJobCompleted(const EventJob *ev)
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

void GameScene::OnJobAborted(const EventJob *ev)
{
	Job *job = ev->GetJob();
	Delete(job);
}
