#include "game_state.h"
#include "../gameflow.h"
#include "../scenes/game_scene.h"

GameState::GameState()
	: cScene()
	, pGame(NULL)
	, bDoStop(false)
{
}

GameState::~GameState()
{
}

void GameState::OnStart(IObject *data)
{
	UNUSED(data)
	Log("Entering Game State");
	bDoStop = false;

	gFlow->LoadGUI("gui/views/game.rml");
	pGame = New(GameScene(&cScene));
	pGame->Initialize();
	cScene.SetVisible(true);
}

void GameState::OnUpdate(f32 dt)
{
	pGame->Update(dt);
}

void GameState::OnStop(IObject *data)
{
	UNUSED(data)
	Log("Exiting Game State");
	bDoStop = true;
	cScene.SetVisible(false);
}

/*
We need this because we will mess with the SceneGraph from an unexpected event (ie. user input)
and the Renderer may be traversing the graph, so we need be sure that the destruction will happen
inside an update.
*/
void GameState::LateStop()
{
	if (bDoStop && pGame)
	{
		pGame->Shutdown();
		Delete(pGame);
	}
}
