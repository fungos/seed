#include "state_working.h"
#include "data/agent_data.h"

StateWorking::StateWorking(AgentData *agentData)
	: pAgentData(agentData)
{
}

StateWorking::~StateWorking()
{
}

void StateWorking::OnStart(void *)
{
	Log("It's time to go work!");
}

void StateWorking::OnUpdate(f32)
{
	pAgentData->SetFatigue(pAgentData->GetFatigue() + 1);
	Log("Working ... %d", pAgentData->GetFatigue());
}

void StateWorking::OnStop(void *)
{
	Log("I'm tired, I want to go home!");
}
