#ifndef _CREDITS_STATE_H
#define _CREDITS_STATE_H

#include <Seed.h>
using namespace Seed;

class CreditsState : public StateMachineState
{
	public:
		CreditsState();
		virtual ~CreditsState();
		virtual void OnStart(IObject *);
		virtual void OnUpdate(f32);
		virtual void OnStop(IObject *);
};

#endif // _CREDITS_STATE_H
