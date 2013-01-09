#include "keyboard_sample.h"

#define VECTOR_UP		Vector3f(0, -1, 0);
#define VECTOR_DOWN		Vector3f(0, 1, 0);
#define VECTOR_LEFT		Vector3f(-1, 0, 0);
#define VECTOR_RIGHT	Vector3f(1, 0, 0);
#define VECTOR_ZERO		Vector3f(0, 0, 0);

KeyboardSample::KeyboardSample()
	: fVelocity(0.0f)
	, bPresentationLoaded(false)
{
}

KeyboardSample::~KeyboardSample()
{
}

bool KeyboardSample::Initialize()
{
	// All loading will be async, so we need flags, this is correct to use in this context.
	bPresentationLoaded = false;

	// Set the velocity of player
	fVelocity = 250.0f;

	// Set the direction of player
	vPlayerVectorDirection = VECTOR_ZERO;

	// Initialize the game by a config file
	return cPres.Load("keyboard_sample.config", this);
}

void KeyboardSample::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	// Retreive the sprite of player
	pPlayerSprite = (Image *)cPres.GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Panda");

	bPresentationLoaded = true;
}

bool KeyboardSample::Update(f32 dt)
{
	if (bPresentationLoaded)
	{
		// SpriteVector = SpriteVector + DirectionVector * (velocity * deltaTime)
		pPlayerSprite->SetPosition(pPlayerSprite->GetPosition() + (vPlayerVectorDirection * (fVelocity * dt)));
	}

	return true;
}

bool KeyboardSample::Shutdown()
{
	cPres.Unload();

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void KeyboardSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void KeyboardSample::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyUp)
	{
		// Sum the normalized vector up with the current vector
		vPlayerVectorDirection += VECTOR_UP;
	}

	if (k == Seed::KeyLeft)
	{
		// Sum the normalized vector left with the current vector
		vPlayerVectorDirection += VECTOR_LEFT;

		// Change the scale to turn the player sprite
		if (pPlayerSprite->GetScaleX() < 0)
			pPlayerSprite->SetScaleX(pPlayerSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyRight)
	{
		// Sum the normalized vector right with the current vector
		vPlayerVectorDirection += VECTOR_RIGHT;

		// Change the scale to turn the player sprite
		if (pPlayerSprite->GetScaleX() > 0)
			pPlayerSprite->SetScaleX(pPlayerSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyDown)
	{
		// Sum the normalized vector down with the current vector
		vPlayerVectorDirection += VECTOR_DOWN;
	}
}

void KeyboardSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	// Remove the directions
	if (k == Seed::KeyUp)
	{
		vPlayerVectorDirection -= VECTOR_UP;
	}

	if (k == Seed::KeyLeft)
	{
		vPlayerVectorDirection -= VECTOR_LEFT;
	}

	if (k == Seed::KeyRight)
	{
		vPlayerVectorDirection -= VECTOR_RIGHT;
	}

	if (k == Seed::KeyDown)
	{
		vPlayerVectorDirection -= VECTOR_DOWN;
	}

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
}
