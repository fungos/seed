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
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardPress(const EventInputKeyboard *ev) override;
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

		// IEventInputJoystickListener
		virtual void OnInputJoystickButtonPress(const EventInputJoystick *ev) override;
		virtual void OnInputJoystickButtonRelease(const EventInputJoystick *ev) override;
		virtual void OnInputJoystickDPadMove(const EventInputJoystick *ev) override;
		virtual void OnInputJoystickAxisMove(const EventInputJoystick *ev) override;

	protected:
		Presentation cPres;
		ISceneObject *pObject;
		vec3		vPlayerVectorDirection;
		f32			fVelocity;
		bool		bPresentationLoaded : 1;
};

#endif // _JOYSTICK_INPUT_SAMPLE_H
