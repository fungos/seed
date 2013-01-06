#include "state_sleeping.h"

StateSleeping::StateSleeping(IObject *agentData)
    : pAgentData(agentData)
{

}

StateSleeping::~StateSleeping()
{

}

void StateSleeping::OnStart(IObject *)
{
    Log("I'm sleepy ...");
}

void StateSleeping::OnUpdate(f32)
{
    dynamic_cast<AgentData *>(pAgentData)->SetFatigue(
                dynamic_cast<AgentData *>(pAgentData)->GetFatigue() - 1);

    Log("ZzzZZZzzz ... %d", dynamic_cast<AgentData *>(pAgentData)->GetFatigue());
}

void StateSleeping::OnStop(IObject *)
{
    Log("TRIIIIIIIIIMMMMMMMMMM ...");
}
