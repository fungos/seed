#include "state_sleeping.h"
#include "data/agent_data.h"

StateSleeping::StateSleeping(AgentData *agentData)
	: pAgentData(agentData)
{
}

StateSleeping::~StateSleeping()
{
}

void StateSleeping::OnStart(void *)
{
	Log("I'm sleepy ...");
}

void StateSleeping::OnUpdate(f32)
{
	pAgentData->SetFatigue(pAgentData->GetFatigue() - 1);
	Log("ZzzZZZzzz ... %d", pAgentData->GetFatigue());
}

void StateSleeping::OnStop(void *)
{
	Log("TRIIIIIIIIIMMMMMMMMMM ...");
}
