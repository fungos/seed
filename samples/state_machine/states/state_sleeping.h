#ifndef _STATE_SLEEPING_H
#define _STATE_SLEEPING_H

#include <Seed.h>
#include "data/agent_data.h"

using namespace Seed;

class StateSleeping : public StateMachineState
{
    public:
        StateSleeping(IObject *agentData);
        virtual ~StateSleeping();
        virtual void OnStart(IObject *);
        virtual void OnUpdate(f32);
        virtual void OnStop(IObject *);

    private:
        IObject   *pAgentData;
};

#endif // _STATE_SLEEPING_H
