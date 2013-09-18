#include "mainmenu_state.h"
#include "../gameflow.h"

MainMenuState::MainMenuState()
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::OnStart(void *data)
{
	UNUSED(data)
	Log("Entering MainMenu State");

	gFlow->LoadGUI("gui/views/mainmenu.rml");
}

void MainMenuState::OnUpdate(f32 dt)
{
	UNUSED(dt)
}

void MainMenuState::OnStop(void *data)
{
	UNUSED(data)
	Log("Exiting MainMenu State");
}
