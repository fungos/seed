#include "keyboard_sample.h"

#define VECTOR_UP		vec3{ 0.0f, -1.0f, 0.0f};
#define VECTOR_DOWN		vec3{ 0.0f,  1.0f, 0.0f};
#define VECTOR_LEFT		vec3{-1.0f,  0.0f, 0.0f};
#define VECTOR_RIGHT	vec3{ 1.0f,  0.0f, 0.0f};
#define VECTOR_ZERO		vec3{ 0.0f,  0.0f, 0.0f};

KeyboardSample::KeyboardSample()
	: cPres()
	, pObject(nullptr)
	, vPlayerVectorDirection(0.0f, 0.0f, 0.0f)
	, fVelocity(0.0f)
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
	return cPres.Load("keyboard_sample.config", [&](Presentation *pres, Viewport *aborted) {
		if (!aborted)
		{
			pSystem->AddListener(this);
			pInput->AddKeyboardListener(this);

			// Retreive the sprite of player
			pObject = pres->GetViewportByName("MainView")->GetScene()->GetChildByName("Panda");
			bPresentationLoaded = true;
		}
	});
}

bool KeyboardSample::Update(Seconds dt)
{
	if (bPresentationLoaded)
		pObject->SetPosition(pObject->GetPosition() + (vPlayerVectorDirection * (fVelocity * dt)));

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

bool KeyboardSample::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if(k == eKey::Enter)
	{
		pScreen->ToggleFullscreen();
	}

	if (k == eKey::Up)
	{
		// Sum the normalized vector up with the current vector
		vPlayerVectorDirection += VECTOR_UP;
	}

	if (k == eKey::Left)
	{
		// Sum the normalized vector left with the current vector
		vPlayerVectorDirection += VECTOR_LEFT;

		// Change the scale to turn the player sprite
		if (pObject->GetScaleX() < 0)
			pObject->SetScaleX(pObject->GetScaleX() * -1);
	}

	if (k == eKey::Right)
	{
		// Sum the normalized vector right with the current vector
		vPlayerVectorDirection += VECTOR_RIGHT;

		// Change the scale to turn the player sprite
		if (pObject->GetScaleX() > 0)
			pObject->SetScaleX(pObject->GetScaleX() * -1);
	}

	if (k == eKey::Down)
	{
		// Sum the normalized vector down with the current vector
		vPlayerVectorDirection += VECTOR_DOWN;
	}

	return true;
}

bool KeyboardSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	// Remove the directions
	if (k == eKey::Up)
	{
		vPlayerVectorDirection -= VECTOR_UP;
	}

	if (k == eKey::Left)
	{
		vPlayerVectorDirection -= VECTOR_LEFT;
	}

	if (k == eKey::Right)
	{
		vPlayerVectorDirection -= VECTOR_RIGHT;
	}

	if (k == eKey::Down)
	{
		vPlayerVectorDirection -= VECTOR_DOWN;
	}

	if (k == eKey::Escape)
		pSystem->Shutdown();

	return true;
}
