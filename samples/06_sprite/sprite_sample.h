#ifndef _SPRITESAMPLE_H_
#define _SPRITESAMPLE_H_

#include <Seed.h>
using namespace Seed;

class SpriteSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener
{
	SEED_DISABLE_COPY(SpriteSample)

	public:
		SpriteSample();
		virtual ~SpriteSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventInputPointerListener
		virtual void OnInputPointerRelease(const EventInputPointer *ev);

	protected:
		Presentation cPres;
		ISceneObject *pSprite;
		Camera		*pCamera;

		Vector3f	vFrom;
		Vector3f	vCurrent;
		Vector3f	vTo;

		f32			fElapsed;
		f32			fDir;
		bool		bRotate : 1;
};

#endif // _SPRITESAMPLE_H_
