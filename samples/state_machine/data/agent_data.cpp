#include "agent_data.h"

AgentData::AgentData()
    : fatigue(40)
{
}

AgentData::~AgentData()
{
}

u32 AgentData::GetFatigue()
{
    return fatigue;
}

void AgentData::SetFatigue(const u32 fatigue)
{
    this->fatigue = fatigue;
}

const String AgentData::GetClassName() const
{
    return "AgentData";
}

int AgentData::GetObjectType() const
{
    return Seed::TypeUser;
}
