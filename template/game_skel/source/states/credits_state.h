#ifndef _CREDITS_STATE_H
#define _CREDITS_STATE_H

#include <Seed.h>
using namespace Seed;

class CreditsState : public StateMachineState
{
	public:
		CreditsState();
		virtual ~CreditsState();
		virtual void OnStart(void *);
		virtual void OnUpdate(f32);
		virtual void OnStop(void *);
};

#endif // _CREDITS_STATE_H
