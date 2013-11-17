#include "joystick_sample.h"

#define VECTOR_UP		Vector3f(0, -0.05, 0);
#define VECTOR_DOWN		Vector3f(0, 0.05, 0);
#define VECTOR_LEFT		Vector3f(-0.05, 0, 0);
#define VECTOR_RIGHT	Vector3f(0.05, 0, 0);
#define VECTOR_ZERO		Vector3f(0, 0, 0);

JoystickSample::JoystickSample()
	: cPres()
	, pPlayerSprite(nullptr)
	, vPlayerVectorDirection()
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
	return cPres.Load("joystick_sample.config", this);
}

void JoystickSample::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);
	pInput->AddJoystickListener(this);

	// Retreive the sprite of player
	pPlayerSprite = (Image *)cPres.GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Panda");

	bPresentationLoaded = true;
}

bool JoystickSample::Update(f32 dt)
{
	if (bPresentationLoaded)
	{
		// SpriteVector = SpriteVector + DirectionVector * (velocity * deltaTime)
		pPlayerSprite->SetPosition(pPlayerSprite->GetPosition() + (vPlayerVectorDirection * (fVelocity * dt)));
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

void JoystickSample::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	UNUSED(ev)
}

void JoystickSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
}

void JoystickSample::OnInputJoystickButtonPress(const EventInputJoystick *ev)
{
	auto k = ev->GetPressed();

	if (k == eInputButton::Button14)
	{
		pPlayerSprite = (Image *)cPres.GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Panda2");
	}

	if (k == eInputButton::Button15)
	{
		pPlayerSprite = (Image *)cPres.GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Panda");
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
	auto k = ev->GetPressed();

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
		if (pPlayerSprite->GetScaleX() < 0)
			pPlayerSprite->SetScaleX(pPlayerSprite->GetScaleX() * -1);
	}

	if (k == eInputButton::JoystickRight)
	{
		// Sum the normalized vector right with the current vector
		vPlayerVectorDirection += VECTOR_RIGHT;

		// Change the scale to turn the player sprite
		if (pPlayerSprite->GetScaleX() > 0)
			pPlayerSprite->SetScaleX(pPlayerSprite->GetScaleX() * -1);
	}

	if (k == eInputButton::JoystickDown)
	{
		// Sum the normalized vector down with the current vector
		vPlayerVectorDirection += VECTOR_DOWN;
	}
}

void JoystickSample::OnInputJoystickAxisMove(const EventInputJoystick *ev)
{
	UNUSED(ev)
}
