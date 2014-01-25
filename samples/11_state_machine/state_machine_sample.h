#ifndef _STATE_MACHINE_SAMPLE_H
#define _STATE_MACHINE_SAMPLE_H

#include <Seed.h>
using namespace Seed;

class AgentData;

class StateMachineSample : public IGameApp,
						   public IEventSystemListener,
						   public IEventInputKeyboardListener
{
	SEED_DISABLE_COPY(StateMachineSample)

	public:
		StateMachineSample();
		virtual ~StateMachineSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

	protected:
		Presentation		cPres;

		// State Machine states
		StateMachineState	*pStateSleeping;
		StateMachineState	*pStateWorking;

		// State Machine Events
		StateMachineEvent	*pOnSleepEvent;
		StateMachineEvent	*pOnWorkEvent;

		// State Machine transitions
		StateMachineTransition  cTransSleepToWork;
		StateMachineTransition  cTransWorkToSleep;

		// State Machine data
		AgentData			*pAgentData;

		// State Machine
		StateMachine		cAgent;
};

#endif // _STATE_MACHINE_SAMPLE_H
