#include "options_state.h"
#include "../gameflow.h"

OptionsState::OptionsState()
{
}

OptionsState::~OptionsState()
{
}

void OptionsState::OnStart(void *data)
{
	UNUSED(data)
	Log("Entering Options State");

	gFlow->LoadGUI("gui/views/options.rml");
}

void OptionsState::OnUpdate(f32 dt)
{
	UNUSED(dt)
}

void OptionsState::OnStop(void *data)
{
	UNUSED(data)
	Log("Exiting Options State");
}
