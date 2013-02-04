#include "key_input_sample.h"

KeyboardInputSample::KeyboardInputSample()
	: fVelocity(0.0f)
	, bPresentationLoaded(false)
{
}

KeyboardInputSample::~KeyboardInputSample()
{
}

bool KeyboardInputSample::Initialize()
{
	// All loading will be async, so we need flags, this is correct to use in this context.
	bPresentationLoaded = false;

	// Set the velocity of player
	fVelocity = 250.0f;

	// Set the direction of player
	vPlayerVectorDirection = VECTOR_ZERO;

	// Initialize the game by a config file
	return cPres.Load("key_input_sample.config", this);
}

void KeyboardInputSample::OnPresentationLoaded(const EventPresentation *ev)
{
	UNUSED(ev)

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	// Retreive the sprite of player
	pPlayerSprite = (Image*) cPres.GetRendererByName("Renderer1")->GetScene()->GetChildByName("Panda");

	bPresentationLoaded = true;
}

bool KeyboardInputSample::Update(f32 dt)
{
	if (bPresentationLoaded)
	{
		// SpriteVector = SpriteVector + DirectionVector * (velocity * deltaTime)
		pPlayerSprite->SetPosition(pPlayerSprite->GetPosition() + (vPlayerVectorDirection * (fVelocity * dt)));
	}

	return true;
}

bool KeyboardInputSample::Shutdown()
{
	cPres.Unload();

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void KeyboardInputSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void KeyboardInputSample::OnInputKeyboardPress(const EventInputKeyboard *ev)
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

		// Change the scale to turn the player sprite (I think that logic violates the layer :P)
		if (pPlayerSprite->GetScaleX() < 0)
			pPlayerSprite->SetScaleX(pPlayerSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyRight)
	{
		// Sum the normalized vector right with the current vector
		vPlayerVectorDirection += VECTOR_RIGHT;

		// Change the scale to turn the player sprite (I think that logic violates the layer :P)
		if (pPlayerSprite->GetScaleX() > 0)
			pPlayerSprite->SetScaleX(pPlayerSprite->GetScaleX() * -1);
	}

	if (k == Seed::KeyDown)
	{
		// Sum the normalized vector down with the current vector
		vPlayerVectorDirection += VECTOR_DOWN;
	}
}

void KeyboardInputSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
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
