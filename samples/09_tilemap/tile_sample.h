#ifndef _TILESAMPLE_H_
#define _TILESAMPLE_H_

#include <Seed.h>
using namespace Seed;

extern SceneNode *gScene;

class TileSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener
{
	SEED_DISABLE_COPY(TileSample)

	public:
		TileSample();
		virtual ~TileSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;
		virtual bool OnInputKeyboardPress(const EventInputKeyboard *ev) override;

	protected:
		ISceneObject	*pPlayer;
		Camera			*pCamera;
		GameMap			*pMap;
		Presentation	cPres;
		vec3			vDir;
		float			fSpeed;
		bool			bLoaded;

};

#endif // _TILESAMPLE_H_
