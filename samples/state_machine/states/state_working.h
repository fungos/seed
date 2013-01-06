#ifndef _STATE_WORKING_H
#define _STATE_WORKING_H

#include <Seed.h>
#include "data/agent_data.h"

using namespace Seed;

class StateWorking : public StateMachineState
{
    public:
        StateWorking(IObject *agentData);
        virtual ~StateWorking();
        virtual void OnStart(AgentData *);
        virtual void OnUpdate(f32);
        virtual void OnStop(AgentData *);

    private:
        IObject   *pAgentData;
};

#endif // _STATE_WORKING_H
