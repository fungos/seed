#include "state_working.h"

StateWorking::StateWorking(IObject *agentData)
    : pAgentData(agentData)
{

}

StateWorking::~StateWorking()
{
}

void StateWorking::OnStart(AgentData *)
{
    Log("It's time to go work!");
}

void StateWorking::OnUpdate(f32)
{
    dynamic_cast<AgentData *>(pAgentData)->SetFatigue(
                dynamic_cast<AgentData *>(pAgentData)->GetFatigue() + 1);

    Log("Working ... %d", dynamic_cast<AgentData *>(pAgentData)->GetFatigue());
}

void StateWorking::OnStop(AgentData *)
{

    Log("I'm tired, I want to go home!");
}
