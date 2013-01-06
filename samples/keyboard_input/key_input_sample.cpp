#include "key_input_sample.h"

KeyboardInputSample::KeyboardInputSample()
{
}

KeyboardInputSample::~KeyboardInputSample()
{
}

bool KeyboardInputSample::Initialize()
{
    // I don't know other way to solve this, I had to create this boolean
    // because Update method runs before OnPresentationLoaded method finishes
    presentationLoaded = false;

    // Set the velocity of player
    velocity = 250.0f;

    // Set the direction of player
    pPlayerVectorDirection = VECTOR_ZERO;

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

    // I don't know other way to solve this, I had to create this boolean
    // because Update method runs before OnPresentationLoaded method finishes
    presentationLoaded = true;
}

bool KeyboardInputSample::Update(f32 dt)
{
    // I don't know other way to solve this, I had to create this boolean
    // because Update method runs before OnPresentationLoaded method finishes
    if (presentationLoaded)
    {
        // SpriteVector = SpriteVector + DirectionVector * (velocity * deltaTime)
        pPlayerSprite->SetPosition(pPlayerSprite->GetPosition().operator +(pPlayerVectorDirection->operator *(velocity * dt) ));
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
        // Move up
        const Vector3f *vectorUp = VECTOR_UP;

        // Sum the normalized vector up with the current vector
        pPlayerVectorDirection->operator +=(vectorUp->operator *(1));
    }

    if (k == Seed::KeyLeft)
    {
        // Move Left
        const Vector3f *vectorLeft = VECTOR_LEFT;

        // Sum the normalized vector left with the current vector
        pPlayerVectorDirection->operator +=(vectorLeft->operator *(1));

        // Change the scale to turn the player sprite (I think that logic violates the layer :P)
        if (pPlayerSprite->GetScaleX() < 0)
            pPlayerSprite->SetScaleX(pPlayerSprite->GetScaleX() * -1);
    }

    if (k == Seed::KeyRight)
    {
        // Move Right
        const Vector3f *vectorRight = VECTOR_RIGHT;

        // Sum the normalized vector right with the current vector
        pPlayerVectorDirection->operator +=(vectorRight->operator *(1));

        // Change the scale to turn the player sprite (I think that logic violates the layer :P)
        if (pPlayerSprite->GetScaleX() > 0)
            pPlayerSprite->SetScaleX(pPlayerSprite->GetScaleX() * -1);
    }

    if (k == Seed::KeyDown)
    {
        // Move Down
        const Vector3f *vectorDown = VECTOR_DOWN;

        // Sum the normalized vector down with the current vector
        pPlayerVectorDirection->operator +=(vectorDown->operator *(1));
    }
}

void KeyboardInputSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
    Key k = ev->GetKey();

    // Remove the directions
    if (k == Seed::KeyUp)
    {
        const Vector3f *vectorUp = VECTOR_UP;
        pPlayerVectorDirection->operator -= (vectorUp->operator *(1));
    }

    if (k == Seed::KeyLeft)
    {
        const Vector3f *vectorLeft = VECTOR_LEFT;
        pPlayerVectorDirection->operator -=(vectorLeft->operator *(1));
    }

    if (k == Seed::KeyRight)
    {
        const Vector3f *vectorRight = VECTOR_RIGHT;
        pPlayerVectorDirection->operator -=(vectorRight->operator *(1));
    }

    if (k == Seed::KeyDown)
    {
        const Vector3f *vectorDown = VECTOR_DOWN;
        pPlayerVectorDirection->operator -=(vectorDown->operator *(1));
    }

    if (k == Seed::KeyEscape)
        pSystem->Shutdown();
}
