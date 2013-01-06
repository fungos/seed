#ifndef _AGENT_DATA_H
#define _AGENT_DATA_H

#include <Seed.h>

using namespace Seed;

class AgentData : public IObject
{
    private:
        u32 fatigue;

    public:
        AgentData();
        virtual ~AgentData();

        virtual u32 GetFatigue();
        virtual void SetFatigue(u32);

        // IObject
        virtual const String GetClassName() const override;
        virtual int GetObjectType() const override;
};

#endif // _AGENT_DATA_H
