#include "main_menu_state.h"
#include "../gameflow.h"

MainMenuState::MainMenuState()
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::OnStart(IObject *data)
{
	UNUSED(data)
	Log("Entering MainMenu State");

	gFlow->LoadGUI("gui/views/mainmenu.rml");
}

void MainMenuState::OnUpdate(f32 dt)
{
	UNUSED(dt)
}

void MainMenuState::OnStop(IObject *data)
{
	UNUSED(data)
	Log("Exiting MainMenu State");
}
