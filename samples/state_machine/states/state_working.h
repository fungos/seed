#ifndef _STATE_WORKING_H
#define _STATE_WORKING_H

#include <Seed.h>
using namespace Seed;

class AgentData;

class StateWorking : public StateMachineState
{
	public:
		StateWorking(AgentData *agentData);
		virtual ~StateWorking();
		virtual void OnStart(AgentData *);
		virtual void OnUpdate(f32);
		virtual void OnStop(AgentData *);

	private:
		AgentData *pAgentData;
};

#endif // _STATE_WORKING_H
