#ifndef _KEYBOARD_INPUT_SAMPLE_H
#define _KEYBOARD_INPUT_SAMPLE_H

#include <Seed.h>
using namespace Seed;

class KeyboardSample : public IGameApp,
							public IEventSystemListener,
							public IEventInputKeyboardListener,
							public IEventInputPointerListener
{
	SEED_DISABLE_COPY(KeyboardSample)

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

	protected:
		Presentation cPres;
		Image		*pPlayerSprite;
		Vector3f	vPlayerVectorDirection;
		f32			fVelocity;
		bool		bPresentationLoaded : 1;
};

#endif // _KEYBOARD_INPUT_SAMPLE_H
