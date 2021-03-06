/*
* Copyright (c) 2012, Seed Developers
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
* 4. A credit line is required in the game credits, as the following line:
*    Powered by Seed Framework
* 5. A Seed Framework logo should be displayed within a splash screen, or in
*    the case where the game haven't or cannot have a splash, a logo should be
*    displayed somewhere in the game (eg. menu, credits, other place).
*
* THIS SOFTWARE IS PROVIDED BY SEED DEVELOPERS ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL SEED DEVELOPERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include "Defines.h"
#include "Enum.h"
#include "interface/IEvent.h"
#include "Log.h"

namespace Seed {

/// State Machine Event
class SEED_CORE_API StateMachineEvent : public IEvent
{
	SEED_DECLARE_RTTI(StateMachineEvent, IEvent)

	public:
		StateMachineEvent() = default;
		virtual ~StateMachineEvent() {}
};

/// State Machine State
class SEED_CORE_API StateMachineState
{
	public:
		StateMachineState() = default;
		virtual ~StateMachineState() {}
		virtual void OnStart(void *) {}
		virtual void OnUpdate(f32) {}
		virtual void OnStop(void *) {}
};

/// State Machine Transition
class SEED_CORE_API StateMachineTransition
{
	private:
		StateMachineState *pFrom;
		StateMachineState *pTo;
		StateMachineEvent *pEvent;

	public:
		void Initialize(StateMachineState *from, StateMachineEvent *event, StateMachineState *to)
		{
			pFrom = from;
			pTo = to;
			pEvent = event;
		}

		StateMachineState *GetFromState() const
		{
			return pFrom;
		}

		StateMachineState *GetToState() const
		{
			return pTo;
		}

		StateMachineEvent *GetEvent() const
		{
			return pEvent;
		}
};

/// State Machine
class SEED_CORE_API StateMachine
{
	SEED_DISABLE_COPY(StateMachine)

	public:
		enum eReturnCode
		{
			ResultOk		= 0x00,
			StateNotFound	= 0x01,
			EventNotFound	= 0x02,
			ResultError		= 0xFF
		};

	private:
		StateMachineState *pCurrentState;
		Vector<StateMachineTransition *> vTransitions;

	public:
		StateMachine()
			: pCurrentState(nullptr)
			, vTransitions()
		{}

		~StateMachine()
		{
			Vector<StateMachineTransition *>().swap(vTransitions);
		}

		eReturnCode Initialize(StateMachineState *state, void *userData = nullptr)
		{
			if (!state)
				return StateNotFound; //invalid state

			pCurrentState = nullptr;
			unsigned int i = 0;

			for (; i < vTransitions.size(); i++)
				if (vTransitions[i]->GetFromState() == state)
					break;

			if (i >= vTransitions.size())
				return StateNotFound; //invalid state

			pCurrentState = state;
			pCurrentState->OnStart(userData);

			return ResultOk;
		}

		void RegisterTransition(StateMachineTransition *transition)
		{
			//Check if the transition is has a valid configuration
			SEED_ASSERT(transition->GetFromState() && transition->GetToState() && transition->GetEvent());
			vTransitions += transition;
		}

		void ClearTransitions()
		{
			Vector<StateMachineTransition *>().swap(vTransitions);
		}

		StateMachineState *GetCurrentState()
		{
			return pCurrentState;
		}

		eReturnCode OnEvent(StateMachineEvent *evt, void *pUserData = nullptr)
		{
			unsigned int i = 0;

			for (; i < vTransitions.size(); i++)
			{
				if (vTransitions[i]->GetEvent() == evt)
				{
					if (vTransitions[i]->GetFromState() == pCurrentState)
					{
						//Old State Exit
						vTransitions[i]->GetFromState()->OnStop(pUserData);
						pCurrentState = vTransitions[i]->GetToState();

						//New State Entry callback
						pCurrentState->OnStart(pUserData);

						break;
					}
				}
			}

			if (i >= vTransitions.size()) //no registred transition helding this event
			{
				Log("StateMachine: Event not found.");
				return EventNotFound; //invalid event
			}

			return ResultOk;
		}

		eReturnCode Update(Seconds dt)
		{
			if (!pCurrentState) //not initialized
			{
				Log("StateMachine: State not found.");
				return StateNotFound;
			}

			//State Looping callback
			pCurrentState->OnUpdate(dt);

			return ResultOk;
		}
};

} // namespace

#endif // __STATE_MACHINE_H__
