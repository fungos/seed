#ifndef _AGENT_DATA_H
#define _AGENT_DATA_H

#include <Seed.h>
using namespace Seed;

#define TYPE_AGENTDATA (Seed::TypeUser + 1)

class AgentData : public IObject
{
	public:
		AgentData();
		virtual ~AgentData();

		virtual u32 GetFatigue();
		virtual void SetFatigue(u32);

		// IObject
		virtual const String GetClassName() const override;
		virtual int GetObjectType() const override;

	private:
		u32 iFatigue;
};

#endif // _AGENT_DATA_H
