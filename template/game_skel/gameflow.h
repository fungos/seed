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

class Options : public StateMachineState
{
	public:
		Options();
		virtual ~Options();
		virtual void OnStart(IObject *);
		virtual void OnUpdate(f32);
		virtual void OnStop(IObject *);
};

class GamePause : public StateMachineState
{
	public:
		GamePause();
		virtual ~GamePause();
		virtual void OnStart(IObject *);
		virtual void OnUpdate(f32);
		virtual void OnStop(IObject *);
};

class Game;
class GameState : public StateMachineState
{
	public:
		GameState();
		virtual ~GameState();
		virtual void OnStart(IObject *);
		virtual void OnUpdate(f32);
		virtual void OnStop(IObject *);
		void LateStop();

		SceneNode cScene;
		Game *pGame;
		bool bDoStop;
};

class GameFlow : public IGameApp,
							public IEventSystemListener,
							public IEventInputKeyboardListener,
							public IEventPresentationListener,
							public Rocket::Core::EventListener
{
	public:
		GameFlow();
		virtual ~GameFlow();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		void AddScene(SceneNode *node);
		void RemoveScene(SceneNode *node);

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

		// Rocket::Core::EventListener
		virtual void ProcessEvent(Rocket::Core::Event &ev);

	private:
		SEED_DISABLE_COPY(GameFlow);

		SceneNode			*pScene;
		Presentation		cPres;
		String				sDocument;

		// State Machine
		StateMachine		cFlow;

		// State Machine states
		MainMenu			cMenu;
		Options				cOptions;
		Credits				cCredits;
		GameState			cGame;
		GamePause			cGamePause;

		// State Machine Events
		StateMachineEvent	cOnMenu;
		StateMachineEvent	cOnOptions;
		StateMachineEvent	cOnCredits;
		StateMachineEvent	cOnGame;
		StateMachineEvent	cOnGamePause;

		// State Machine transitions
		StateMachineTransition cMenuToGame;
		StateMachineTransition cMenuToOptions;
		StateMachineTransition cMenuToCredits;
		StateMachineTransition cOptionsToMenu;
		StateMachineTransition cCreditsToMenu;
		StateMachineTransition cGameToGamePause;
		StateMachineTransition cGamePauseToGame;
		StateMachineTransition cGamePauseToMenu;

		// GUI
		RocketInterface			*pRocket;
		Rocket::Core::Context	*pContext;
		Rocket::Core::ElementDocument *pDoc;
};

#endif // _GAMEFLOW_H_
