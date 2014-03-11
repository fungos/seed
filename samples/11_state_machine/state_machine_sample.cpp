#include "state_machine_sample.h"
#include "states/state_sleeping.h"
#include "states/state_working.h"
#include "data/agent_data.h"

StateMachineSample::StateMachineSample()
	: cPres()
	, pStateSleeping(nullptr)
	, pStateWorking(nullptr)
	, pOnSleepEvent(nullptr)
	, pOnWorkEvent(nullptr)
	, cTransSleepToWork()
	, cTransWorkToSleep()
	, pAgentData(nullptr)
	, cAgent()
{
}

StateMachineSample::~StateMachineSample()
{
}

bool StateMachineSample::Initialize()
{
	bool init = cPres.Load("state_machine_sample.config", [&](Presentation *, Renderer *) {
		pSystem->AddListener(this);
		pInput->AddKeyboardListener(this);
	});

	// Create the data for state machine
	pAgentData = sdNew(AgentData());

	// Create the states
	pStateSleeping = sdNew(StateSleeping(pAgentData));
	pStateWorking = sdNew(StateWorking(pAgentData));

	// Create the events
	pOnSleepEvent = sdNew(StateMachineEvent());
	pOnWorkEvent = sdNew(StateMachineEvent());

	// Create the transitions
	cTransSleepToWork.Initialize(pStateSleeping, pOnWorkEvent, pStateWorking);
	cTransWorkToSleep.Initialize(pStateWorking, pOnSleepEvent, pStateSleeping);

	// Create the State Machine.
	cAgent.RegisterTransition(&cTransSleepToWork);
	cAgent.RegisterTransition(&cTransWorkToSleep);

	cAgent.Initialize(pStateSleeping, pAgentData);
	cAgent.Initialize(pStateWorking, pAgentData);

	return init;
}

bool StateMachineSample::Update(Seconds dt)
{
	if (pAgentData->GetFatigue() > 50)
	{
		cAgent.OnEvent(pOnSleepEvent, pAgentData);
	}

	if (pAgentData->GetFatigue() <= 0)
	{
		cAgent.OnEvent(pOnWorkEvent, pAgentData);
	}

	// Update the state machine
	cAgent.Update(dt);

	return true;
}

bool StateMachineSample::Shutdown()
{
	cPres.Unload();

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	sdDelete(pStateSleeping);
	sdDelete(pStateWorking);

	sdDelete(pOnSleepEvent);
	sdDelete(pOnWorkEvent);

	sdDelete(pAgentData);

	return IGameApp::Shutdown();
}

void StateMachineSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

bool StateMachineSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();

	return true;
}
