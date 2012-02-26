/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include "Defines.h"
#include "interface/IEvent.h"
#include "Log.h"

namespace Seed {

/// State Machine Event
class SEED_CORE_API StateMachineEvent : public IEvent
{
	public:
		StateMachineEvent()
			: IEvent(IEvent::TypeStateMachine, 0)
		{
		}

		StateMachineEvent(u32 _id)
			: IEvent(IEvent::TypeStateMachine, _id)
		{
		}

		inline void Initialize(u32 _id)
		{
			iId = _id;
		}

		virtual ~StateMachineEvent()
		{
		}

		virtual const char *GetObjectName() const
		{
			return "StateMachineEvent";
		}

		virtual int GetObjectType() const
		{
			return Seed::ObjectEvent;
		}
};

/// State Machine State
class SEED_CORE_API StateMachineState
{
	public:
		virtual ~StateMachineState() {}
		virtual void OnStart(IObject *) {}
		virtual void OnUpdate(f32) {}
		virtual void OnStop(IObject *) {}
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

		inline StateMachineState *GetFromState() const
		{
			return pFrom;
		}

		inline StateMachineState *GetToState() const
		{
			return pTo;
		}

		inline StateMachineEvent *GetEvent() const
		{
			return pEvent;
		}
};

/// State Machine
class SEED_CORE_API StateMachine
{
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

		SEED_DISABLE_COPY(StateMachine);

	public:
		StateMachine()
			: pCurrentState(NULL)
			, vTransitions()
		{}

		~StateMachine()
		{
			Vector<StateMachineTransition *>().swap(vTransitions);
		}

		eReturnCode Initialize(StateMachineState *state, IObject *userData)
		{
			if (!state)
				return StateNotFound; //invalid state

			pCurrentState = NULL;
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

		inline void RegisterTransition(StateMachineTransition *transition)
		{
			//Check if the transition is has a valid configuration
			ASSERT(transition->GetFromState() && transition->GetToState() && transition->GetEvent());
			vTransitions += transition;
		}

		inline void ClearTransitions()
		{
			Vector<StateMachineTransition *>().swap(vTransitions);
		}

		inline StateMachineState *GetCurrentState()
		{
			return pCurrentState;
		}

		inline eReturnCode OnEvent(StateMachineEvent *evt, IObject *pUserData)
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

		inline eReturnCode Update(f32 dt)
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
