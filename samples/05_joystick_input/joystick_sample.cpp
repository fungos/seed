#include "joystick_sample.h"

#define VECTOR_UP		vec3{ 0.0f, -0.05f, 0.f};
#define VECTOR_DOWN		vec3{ 0.0f,  0.05f, 0.f};
#define VECTOR_LEFT		vec3{-0.05f, 0.0f,  0.f};
#define VECTOR_RIGHT	vec3{ 0.05f, 0.0f,  0.f};
#define VECTOR_ZERO		vec3{ 0.0f,  0.0f,  0.f};

JoystickSample::JoystickSample()
	: cPres()
	, pObject(nullptr)
	, vPlayerVectorDirection(0.0f, 0.0f, 0.0f)
	, fVelocity(0.0f)
	, bPresentationLoaded(false)
{
}

JoystickSample::~JoystickSample()
{
}

bool JoystickSample::Initialize()
{
	// All loading will be async, so we need flags, this is correct to use in this context.
	bPresentationLoaded = false;

	// Set the velocity of player
	fVelocity = 250.0f;

	// Set the direction of player
	vPlayerVectorDirection = VECTOR_ZERO;

	// Initialize the game by a config file
	return cPres.Load("joystick_sample.config",  [&](Presentation *pres, Viewport *aborted)
	{
		if (!aborted)
		{
			pSystem->AddListener(this);
			pInput->AddKeyboardListener(this);
			pInput->AddJoystickListener(this);

			pObject = pres->GetViewportByName("MainView")->GetScene()->GetChildByName("Panda");
			bPresentationLoaded = true;
		}
	});
}

bool JoystickSample::Update(Seconds dt)
{
	if (bPresentationLoaded)
	{
		// SpriteVector = SpriteVector + DirectionVector * (velocity * deltaTime)
		pObject->SetPosition(pObject->GetPosition() + (vPlayerVectorDirection * (fVelocity * dt)));
	}

	return true;
}

bool JoystickSample::Shutdown()
{
	cPres.Unload();

	pInput->RemoveKeyboardListener(this);
	pInput->RemoveJoystickListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void JoystickSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

bool JoystickSample::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	UNUSED(ev)
	return true;
}

bool JoystickSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();

	return true;
}

void JoystickSample::OnInputJoystickButtonPress(const EventInputJoystick *ev)
{
	auto k = ev->GetPressed();

	if (k == eInputButton::Button14)
	{
		pObject = cPres.GetViewportByName("MainView")->GetScene()->GetChildByName("Panda2");
	}

	if (k == eInputButton::Button13)
	{
		pObject = cPres.GetViewportByName("MainView")->GetScene()->GetChildByName("Panda");
	}

	if (k == eInputButton::JoystickUp)
	{
		// Sum the normalized vector up with the current vector
		vPlayerVectorDirection += VECTOR_UP;
	}

	if (k == eInputButton::JoystickLeft)
	{
		// Sum the normalized vector left with the current vector
		vPlayerVectorDirection += VECTOR_LEFT;

		// Change the scale to turn the player sprite
		if (pObject->GetScaleX() < 0)
			pObject->SetScaleX(pObject->GetScaleX() * -1);
	}

	if (k == eInputButton::JoystickRight)
	{
		// Sum the normalized vector right with the current vector
		vPlayerVectorDirection += VECTOR_RIGHT;

		// Change the scale to turn the player sprite
		if (pObject->GetScaleX() > 0)
			pObject->SetScaleX(pObject->GetScaleX() * -1);
	}

	if (k == eInputButton::JoystickDown)
	{
		// Sum the normalized vector down with the current vector
		vPlayerVectorDirection += VECTOR_DOWN;
	}
}

void JoystickSample::OnInputJoystickButtonRelease(const EventInputJoystick *ev)
{
	UNUSED(ev)
	/*
	auto k = ev->GetReleased();

	// Remove the directions
	if (k == eInputButton::Button2)
	{
		vPlayerVectorDirection -= VECTOR_UP;
	}

	if (k == eInputButton::Button7)
	{
		vPlayerVectorDirection -= VECTOR_LEFT;
	}

	if (k == eInputButton::Button5)
	{
		vPlayerVectorDirection -= VECTOR_RIGHT;
	}

	if (k == eInputButton::Button6)
	{
		vPlayerVectorDirection -= VECTOR_DOWN;
	}*/
}

void JoystickSample::OnInputJoystickDPadMove(const EventInputJoystick *ev)
{
	UNUSED(ev)
}

void JoystickSample::OnInputJoystickAxisMove(const EventInputJoystick *ev)
{
	UNUSED(ev)
}
