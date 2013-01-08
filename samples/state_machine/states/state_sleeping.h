#ifndef _STATE_SLEEPING_H
#define _STATE_SLEEPING_H

#include <Seed.h>
using namespace Seed;

class AgentData;

class StateSleeping : public StateMachineState
{
	public:
		StateSleeping(AgentData *agentData);
		virtual ~StateSleeping();
		virtual void OnStart(IObject *);
		virtual void OnUpdate(f32);
		virtual void OnStop(IObject *);

	private:
		AgentData *pAgentData;
};

#endif // _STATE_SLEEPING_H
