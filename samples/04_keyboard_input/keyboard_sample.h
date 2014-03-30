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
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardPress(const EventInputKeyboard *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

	protected:
		Presentation cPres;
		ISceneObject *pObject;
		vec3		vPlayerVectorDirection;
		f32			fVelocity;
		bool		bPresentationLoaded : 1;
};

#endif // _KEYBOARD_INPUT_SAMPLE_H
