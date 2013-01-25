#include "game_pause_state.h"
#include "../gameflow.h"

GamePauseState::GamePauseState()
{
}

GamePauseState::~GamePauseState()
{
}

void GamePauseState::OnStart(IObject *data)
{
	UNUSED(data)
	Log("Entering GamePause State");

	gFlow->LoadGUI("gui/views/gamepause.rml");
}

void GamePauseState::OnUpdate(f32 dt)
{
	UNUSED(dt)
}

void GamePauseState::OnStop(IObject *data)
{
	UNUSED(data)
	Log("Exiting GamePause State");
}
