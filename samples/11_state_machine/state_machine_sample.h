#ifndef _STATE_MACHINE_SAMPLE_H
#define _STATE_MACHINE_SAMPLE_H

#include <Seed.h>
using namespace Seed;

class AgentData;

class StateMachineSample : public IGameApp,
						   public IEventSystemListener,
						   public IEventInputKeyboardListener,
						   public IEventPresentationListener
{
	public:
		StateMachineSample();
		virtual ~StateMachineSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventPresentationListener
		virtual void OnPresentationLoaded(const EventPresentation *ev);

	private:
		SEED_DISABLE_COPY(StateMachineSample);

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
