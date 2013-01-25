#include "credits_state.h"
#include "../gameflow.h"

CreditsState::CreditsState()
{
}

CreditsState::~CreditsState()
{
}

void CreditsState::OnStart(IObject *data)
{
	UNUSED(data)
	Log("Entering Credits State");

	gFlow->LoadGUI("gui/views/credits.rml");
}

void CreditsState::OnUpdate(f32 dt)
{
	UNUSED(dt)
}

void CreditsState::OnStop(IObject *data)
{
	UNUSED(data)
	Log("Exiting Credits State");
}
