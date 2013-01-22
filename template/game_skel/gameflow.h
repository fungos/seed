#ifndef _GAMEFLOW_H_
#define _GAMEFLOW_H_

#include <api/rocket/RocketInterface.h>
#include <Rocket/Core.h>
#include <Seed.h>
using namespace Seed;
using namespace Seed::RocketGui;

class GameFlow;
extern GameFlow *gFlow;

class MainMenu : public StateMachineState
{
	public:
		MainMenu();
		virtual ~MainMenu();
		virtual void OnStart(IObject *);
		virtual void OnUpdate(f32);
		virtual void OnStop(IObject *);
};

class Credits : public StateMachineState
{
	public:
		Credits();
		virtual ~Credits();
		virtual void OnStart(IObject *);
		virtual void OnUpdate(f32);
		virtual void OnStop(IObject *);
};

class GameFlow : public IGameApp,
							public IEventSystemListener,
							public IEventInputKeyboardListener,
							public IEventPresentationListener
{
	public:
		GameFlow();
		virtual ~GameFlow();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// GUI
		bool LoadGUI(const String &doc);
		bool ReloadGUI();
		bool UnloadGUI();
		bool InitializeGUI();
		void ReleaseGUI();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventPresentationListener
		virtual void OnPresentationLoaded(const EventPresentation *ev);

	private:
		SEED_DISABLE_COPY(GameFlow);

		SceneNode			*pScene;
		Presentation		cPres;
		String				sDocument;

		// State Machine
		StateMachine		cFlow;

		// State Machine states
		MainMenu			cMenu;
		StateMachineState	cOptions;
		Credits				cCredits;
		StateMachineState	cGame;

		// State Machine Events
		StateMachineEvent	cOnMenu;
		StateMachineEvent	cOnOptions;
		StateMachineEvent	cOnCredits;
		StateMachineEvent	cOnGame;

		// State Machine transitions
		StateMachineTransition cMenuToGame;
		StateMachineTransition cMenuToOptions;
		StateMachineTransition cMenuToCredits;
		StateMachineTransition cOptionsToMenu;
		StateMachineTransition cCreditsToMenu;
		StateMachineTransition cGameToMenu;

		// GUI
		RocketInterface			*pRocket;
		Rocket::Core::Context	*pContext;
		Rocket::Core::ElementDocument *pDoc;
};

#endif // _GAMEFLOW_H_
