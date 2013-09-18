#ifndef _SPRITESAMPLE_H_
#define _SPRITESAMPLE_H_

#include <Seed.h>
using namespace Seed;

class SpriteSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener,
					public IEventPresentationListener
{
	public:
		SpriteSample();
		virtual ~SpriteSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventInputPointerListener
		virtual void OnInputPointerRelease(const EventInputPointer *ev);

		// IEventPresentationListener
		virtual void OnPresentationLoaded(const EventPresentation *ev);

	private:
		SEED_DISABLE_COPY(SpriteSample);

	protected:
		Presentation cPres;
		ISceneObject *pSprite;
		Camera		*pCamera;

		f32			fElapsed;
		f32			fDir;
		bool		bRotate;

		Vector3f	vFrom;
		Vector3f	vCurrent;
		Vector3f	vTo;
};

#endif // _SPRITESAMPLE_H_
