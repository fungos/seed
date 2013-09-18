#include "agent_data.h"

AgentData::AgentData()
	: iFatigue(40)
{
}

AgentData::~AgentData()
{
}

u32 AgentData::GetFatigue()
{
	return iFatigue;
}

void AgentData::SetFatigue(const u32 fatigue)
{
	iFatigue = fatigue;
}

const String AgentData::GetClassName() const
{
	return "AgentData";
}

int AgentData::GetObjectType() const
{
	return TYPE_AGENTDATA;
}
