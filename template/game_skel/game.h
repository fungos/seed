#ifndef _GAME_H_
#define _GAME_H_

#include <Seed.h>
using namespace Seed;

class Game : public IEventInputKeyboardListener,
			 public IEventJobListener
{
	public:
		Game(SceneNode *parent);
		virtual ~Game();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventJobListener
		virtual void OnJobCompleted(const EventJob *ev);
		virtual void OnJobAborted(const EventJob *ev);

	private:
		SEED_DISABLE_COPY(Game);

		ISceneObject	*pPlayer;
		SceneNode		*pScene;
};

#endif // _GAMEFLOW_H_
