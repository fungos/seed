#ifndef _KEYBOARD_INPUT_SAMPLE_H
#define _KEYBOARD_INPUT_SAMPLE_H

#include <Seed.h>
using namespace Seed;

class KeyboardSample : public IGameApp,
							public IEventSystemListener,
							public IEventInputKeyboardListener,
							public IEventInputPointerListener,
							public IEventPresentationListener
{
	public:
		KeyboardSample();
		virtual ~KeyboardSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardPress(const EventInputKeyboard *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventPresentationListener
		virtual void OnPresentationLoaded(const EventPresentation *ev);

	private:
		SEED_DISABLE_COPY(KeyboardSample);

	protected:
		Presentation cPres;
		Image		*pPlayerSprite;
		Vector3f	vPlayerVectorDirection;
		float		fVelocity;
		bool		bPresentationLoaded;
};

#endif // _KEYBOARD_INPUT_SAMPLE_H
