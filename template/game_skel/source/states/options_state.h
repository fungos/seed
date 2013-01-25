#ifndef _OPTIONS_STATE_H
#define _OPTIONS_STATE_H

#include <Seed.h>
using namespace Seed;

class OptionsState : public StateMachineState
{
	public:
		OptionsState();
		virtual ~OptionsState();
		virtual void OnStart(IObject *);
		virtual void OnUpdate(f32);
		virtual void OnStop(IObject *);
};

#endif // _OPTIONS_STATE_H
