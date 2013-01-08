#ifndef _KEYBOARD_INPUT_SAMPLE_H
#define _KEYBOARD_INPUT_SAMPLE_H

#define VECTOR_UP		Vector3f(0, -1, 0);
#define VECTOR_DOWN		Vector3f(0, 1, 0);
#define VECTOR_LEFT		Vector3f(-1, 0, 0);
#define VECTOR_RIGHT	Vector3f(1, 0, 0);
#define VECTOR_ZERO		Vector3f(0, 0, 0);

#include <Seed.h>
using namespace Seed;

class KeyboardInputSample : public IGameApp,
							public IEventSystemListener,
							public IEventInputKeyboardListener,
							public IEventInputPointerListener,
							public IEventPresentationListener
{
	public:
		KeyboardInputSample();
		virtual ~KeyboardInputSample();

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
		SEED_DISABLE_COPY(KeyboardInputSample);

	protected:
		Presentation cPres;
		Image		*pPlayerSprite;
		Vector3f	vPlayerVectorDirection;
		float		fVelocity;
		bool		bPresentationLoaded;
};

#endif // _KEYBOARD_INPUT_SAMPLE_H
