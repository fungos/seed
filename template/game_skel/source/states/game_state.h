#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <Seed.h>
using namespace Seed;

class GameScene;
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
		GameScene *pGame;
		bool bDoStop;
};

#endif // GAME_STATE_H
