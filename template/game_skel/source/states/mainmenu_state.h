#ifndef _MAIN_MENU_STATE_H
#define _MAIN_MENU_STATE_H

#include <Seed.h>
using namespace Seed;

class MainMenuState : public StateMachineState
{
	public:
		MainMenuState();
		virtual ~MainMenuState();
		virtual void OnStart(void *);
		virtual void OnUpdate(f32);
		virtual void OnStop(void *);
};

#endif // _MAIN_MENU_H
