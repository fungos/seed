#ifndef _JOYSTICK_INPUT_SAMPLE_H
#define _JOYSTICK_INPUT_SAMPLE_H

#include <Seed.h>
using namespace Seed;

class JoystickSample : public IGameApp,
							public IEventSystemListener,
							public IEventInputKeyboardListener,
							public IEventInputJoystickListener
{
	SEED_DISABLE_COPY(JoystickSample)

	public:
		JoystickSample();
		virtual ~JoystickSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardPress(const EventInputKeyboard *ev);
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventInputJoystickListener
		virtual void OnInputJoystickButtonPress(const EventInputJoystick *ev);
		virtual void OnInputJoystickButtonRelease(const EventInputJoystick *ev);
		virtual void OnInputJoystickDPadMove(const EventInputJoystick *ev);
		virtual void OnInputJoystickAxisMove(const EventInputJoystick *ev);

	protected:
		Presentation cPres;
		Image		*pObject;
		Vector3f	vPlayerVectorDirection;
		f32			fVelocity;
		bool		bPresentationLoaded : 1;
};

#endif // _JOYSTICK_INPUT_SAMPLE_H
