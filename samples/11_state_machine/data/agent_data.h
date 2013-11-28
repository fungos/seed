#ifndef _AGENT_DATA_H
#define _AGENT_DATA_H

#include <Seed.h>
using namespace Seed;

class AgentData : public IObject
{
	SEED_DECLARE_RTTI(AgentData, IObject)

	public:
		AgentData();
		virtual ~AgentData();

		virtual u32 GetFatigue();
		virtual void SetFatigue(u32);

	private:
		u32 iFatigue;
};

#endif // _AGENT_DATA_H
