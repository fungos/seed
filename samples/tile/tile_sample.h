#ifndef _TILESAMPLE_H_
#define _TILESAMPLE_H_

#include <Seed.h>
using namespace Seed;

extern SceneNode *gScene;

class TileSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventPresentationListener
{
	public:
		TileSample();
		virtual ~TileSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);
		virtual void OnInputKeyboardPress(const EventInputKeyboard *ev);

		// IEventPresentationListener
		virtual void OnPresentationLoaded(const EventPresentation *ev);

	private:
		SEED_DISABLE_COPY(TileSample);

	protected:

		ISceneObject	*pPlayer;
		Camera			*pCamera;
		Presentation	cPres;
		Vector3f		vDir;
		float			fSpeed;
		bool			bLoaded;

		GameMap			*pMap;
};

#endif // _TILESAMPLE_H_
