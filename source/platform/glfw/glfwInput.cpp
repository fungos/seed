/*
* Copyright (c) 2012, Seed Developers
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
* 4. A credit line is required in the game credits, as the following line:
*    Powered by Seed Framework
* 5. A Seed Framework logo should be displayed within a splash screen, or in
*    the case where the game haven't or cannot have a splash, a logo should be
*    displayed somewhere in the game (eg. menu, credits, other place).
*
* THIS SOFTWARE IS PROVIDED BY SEED DEVELOPERS ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL SEED DEVELOPERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#if defined(BUILD_GLFW)

#include "Defines.h"
#include "Input.h"
#include "Log.h"
#include "Screen.h"
#include "System.h"
#include "Input.h"
#include "EventInputPointer.h"
#include "EventInputKeyboard.h"
#include "EventInputJoystick.h"
#include "EventSystem.h"
#include "ViewManager.h"
#include "Viewport.h"
#include "glfw/glfw.h"

#define TAG "[Input] "

namespace Seed { namespace GLFW {

void GLFWCALL glfwOnKeyCb(int key, int action)
{
	pInput->OnKeyCb(key, action);
}

void GLFWCALL glfwOnCharCb(int character, int action)
{
	pInput->OnKeyCb(character, action);
}

void GLFWCALL glfwOnMouseButtonCb(int button, int action)
{
	pInput->OnMouseButtonCb(button, action);
}

static int _glfw_scroll_origin = 0;
void GLFWCALL glfwOnMouseWheelCb(int pos)
{
	if (!_glfw_scroll_origin)
	{
		_glfw_scroll_origin = pos;
		pInput->iOriginWheel = pos;
	}

	if (pos < _glfw_scroll_origin)
	{
		pInput->OnMouseButtonCb(GLFW_MOUSE_BUTTON_LAST + 1, GLFW_PRESS);
		pInput->OnMouseButtonCb(GLFW_MOUSE_BUTTON_LAST + 1, GLFW_RELEASE);
	}
	else if (pos > _glfw_scroll_origin)
	{
		pInput->OnMouseButtonCb(GLFW_MOUSE_BUTTON_LAST + 2, GLFW_PRESS);
		pInput->OnMouseButtonCb(GLFW_MOUSE_BUTTON_LAST + 2, GLFW_RELEASE);
	}

	pInput->OnMouseWheelCb(pos);
}

static int _glfw_origin_x = 0xdeadbeef;
static int _glfw_origin_y = 0xdeadbeef;
void GLFWCALL glfwOnMousePosCb(int x, int y)
{
	if (_glfw_origin_x == (int)0xdeadbeef)
	{
		_glfw_origin_x = x;
		pInput->iOriginX = x;
	}

	if (_glfw_origin_y == (int)0xdeadbeef)
	{
		_glfw_origin_y = y;
		pInput->iOriginY = y;
	}

	pInput->OnMousePosCb(x, y);
}

SEED_SINGLETON_DEFINE(Input)

Input::Input()
	: iJoystickCount(0)
	, fX(0.0f)
	, fY(0.0f)
	, iWheel(0)
	, iOldWheel(0)
	, iOriginWheel(0)
	, iOriginX(0)
	, iOriginY(0)
	, iOldX(0)
	, iOldY(0)
	, iX(0)
	, iY(0)
	, iAxesMax(0)
	, iButtonsMax(0)
	, arJoyInfo()
	, nModifiers(ModifierNone)
{
	memset(arJoyInfo, '\0', sizeof(arJoyInfo));
}

Input::~Input()
{
	this->Reset();
}

bool Input::Shutdown()
{
	Log(TAG "Terminating...");
	memset(arJoyInfo, '\0', sizeof(arJoyInfo));

	bool r = this->Reset();
	Log(TAG "Terminated.");

	return r;
}

bool Input::Initialize()
{
	Log(TAG "Initializing...");
	bool r = this->Reset();

	Log(TAG "Joystick(s): ");
	for (int i = 0; i < MAX_JOYSTICKS; i++)
	{
		bool b = glfwGetJoystickParam(i, GLFW_PRESENT);
		arJoyInfo[i].bIsPresent = b;

		if (b)
		{
			iJoystickCount++;
			arJoyInfo[i].iAxes = glfwGetJoystickParam(i, GLFW_AXES);
			if (arJoyInfo[i].iAxes > iAxesMax)
				iAxesMax = arJoyInfo[i].iAxes;

			arJoyInfo[i].iButtons = glfwGetJoystickParam(i, GLFW_BUTTONS);
			if (arJoyInfo[i].iButtons > iButtonsMax)
				iButtonsMax = arJoyInfo[i].iButtons;

			Log(TAG "\t\t[%d] Axes: %d Buttons: %d\n", i, arJoyInfo[i].iAxes, arJoyInfo[i].iButtons);
		}
	}

	glfwSetKeyCallback(glfwOnKeyCb);
	glfwSetCharCallback(glfwOnCharCb);
	glfwSetMouseButtonCallback(glfwOnMouseButtonCb);
	glfwSetMouseWheelCallback(glfwOnMouseWheelCb);
	glfwSetMousePosCallback(glfwOnMousePosCb);

	Log(TAG "Initialization completed.");

	return r;
}

void Input::OnKeyCb(int key, int action)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_LALT:
				nModifiers |= ModifierAltLeft;
			break;

			case GLFW_KEY_RALT:
				nModifiers |= (int)ModifierAltRight;
			break;

			case GLFW_KEY_LSHIFT:
				nModifiers |= ModifierShiftLeft;
			break;

			case GLFW_KEY_RSHIFT:
				nModifiers |= ModifierShiftRight;
			break;

			case GLFW_KEY_LCTRL:
				nModifiers |= ModifierControlLeft;
			break;

			case GLFW_KEY_RCTRL:
				nModifiers |= ModifierControlRight;
			break;

			case GLFW_KEY_LSUPER:
				nModifiers |= ModifierMetaLeft;
			break;

			case GLFW_KEY_RSUPER:
				nModifiers |= ModifierMetaRight;
			break;

			case GLFW_KEY_CAPS_LOCK:
				nModifiers |= ModifierCapsLock;
			break;

			case GLFW_KEY_KP_NUM_LOCK:
				nModifiers |= ModifierNumLock;
			break;

			case GLFW_KEY_SCROLL_LOCK:
				nModifiers |= ModifierScrollLock;
			break;

			default:
			break;
		}

		EventInputKeyboard ev(key, nModifiers, key, key);
		this->SendEventKeyboardPress(&ev);
	}
	else
	{
		switch (key)
		{
			case GLFW_KEY_LALT:
				nModifiers ^= ModifierAltLeft;
			break;

			case GLFW_KEY_RALT:
				nModifiers ^= ModifierAltRight;
			break;

			case GLFW_KEY_LSHIFT:
				nModifiers ^= ModifierShiftLeft;
			break;

			case GLFW_KEY_RSHIFT:
				nModifiers ^= ModifierShiftRight;
			break;

			case GLFW_KEY_LCTRL:
				nModifiers ^= ModifierControlLeft;
			break;

			case GLFW_KEY_RCTRL:
				nModifiers ^= ModifierControlRight;
			break;

			case GLFW_KEY_LSUPER:
				nModifiers ^= ModifierMetaLeft;
			break;

			case GLFW_KEY_RSUPER:
				nModifiers ^= ModifierMetaRight;
			break;

			case GLFW_KEY_CAPS_LOCK:
				nModifiers ^= ModifierCapsLock;
			break;

			case GLFW_KEY_KP_NUM_LOCK:
				nModifiers ^= ModifierNumLock;
			break;

			case GLFW_KEY_SCROLL_LOCK:
				nModifiers ^= ModifierScrollLock;
			break;

			default:
			break;
		}

		EventInputKeyboard ev(key, nModifiers, key, key);
		this->SendEventKeyboardRelease(&ev);
	}
}

void Input::OnMouseButtonCb(int button, int action)
{
	int xp = 0;
	int yp = 0;
	glfwGetMousePos(&xp, &yp);

	f32 x, y;
	x = fX = (f32)xp / (f32)pScreen->GetWidth();
	y = fY = (f32)yp / (f32)pScreen->GetHeight();

	Viewport *viewport = static_cast<Viewport*>(pViewManager->GetViewportAt(fX, fY));
	f32 fw = 1.0f;
	f32 fh = 1.0f;
	if (viewport)
	{
		fw = (viewport->GetWidth());
		fh = (viewport->GetHeight());
		x = (fX - viewport->GetX()) / fw;
		y = (fY - viewport->GetY()) / fh;
	}

	if (action == GLFW_RELEASE)
	{
		const EventInputPointer ev(0, 0, 0, this->GetButtonCode(button), x, y);
		this->SendEventPointerRelease(&ev);
	}
	else
	{
		const EventInputPointer ev(0, this->GetButtonCode(button), 0, 0, x, y);
		this->SendEventPointerPress(&ev);
	}
}

void Input::OnMouseWheelCb(int pos)
{
	iOldWheel = iWheel;
	iWheel = pos;
	// ?
}

void Input::OnMousePosCb(int xp, int yp)
{
	iOldX = iX;
	iX = xp;

	iOldY = iY;
	iY = yp;

	f32 x, y;
	x = fX = (f32)xp / (f32)pScreen->GetWidth();
	y = fY = (f32)yp / (f32)pScreen->GetHeight();

	Viewport *viewport = static_cast<Viewport*>(pViewManager->GetViewportAt(fX, fY));
	f32 fw = 1.0f;
	f32 fh = 1.0f;
	if (viewport)
	{
		fw = (viewport->GetWidth());
		fh = (viewport->GetHeight());
		x = (fX - viewport->GetX()) / fw;
		y = (fY - viewport->GetY()) / fh;
	}

	EventInputPointer ev(0, 0, 0, 0, x, y);
	this->SendEventPointerMove(&ev);
}

bool Input::Update(f32 dt)
{
	UNUSED(dt);

	float axes[iAxesMax];
	memset(axes, '\0', sizeof(axes));

	unsigned char buttons[iButtonsMax];
	memset(buttons, '\0', sizeof(buttons));

	for (int i = 0; i < MAX_JOYSTICKS; i++)
	{
		int amount = glfwGetJoystickPos(i, axes, iAxesMax);
		for (int axis = 0; axis < amount; axis++)
		{
			const EventInputJoystick ev(i, 0, 0, 0, axis, axes[axis]);
			this->SendEventJoystickAxisMove(&ev);
		}

		amount = glfwGetJoystickButtons(i, buttons, iButtonsMax);
		for (int btn = 0; btn < amount; btn++)
		{
			if (buttons[btn] == GLFW_PRESS)
			{
				const EventInputJoystick ev(i, btn, 0, 0, 0, 0);
				this->SendEventJoystickButtonPress(&ev);
			}
			else
			{
				const EventInputJoystick ev(i, 0, 0, btn, 0, 0);
				this->SendEventJoystickButtonRelease(&ev);
			}
		}
	}

	return true;
}

bool Input::Reset()
{
	memset(arJoyInfo, '\0', sizeof(arJoyInfo));
	nModifiers = ModifierNone;
	return true;
}

bool Input::IsPressed(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	return false;
}

bool Input::IsHold(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	return false;
}

bool Input::IsReleased(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	return false;
}

bool Input::IsPointerEnabled(u16 joystick) const
{
	UNUSED(joystick);
	return (joystick == 0);
}

f32 Input::GetX(u16 joystick) const
{
	UNUSED(joystick);
	return this->fX;
}

f32 Input::GetY(u16 joystick) const
{
	UNUSED(joystick);
	return this->fY;
}

f32 Input::GetRelativeX(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

f32 Input::GetRelativeY(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

f32 Input::GetOrientation(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

f32 Input::GetDistance(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

Seed::eInputButton Input::GetButtonCode(u32 button) const
{
	Seed::eInputButton btn = Seed::ButtonInvalid;

	if (button == GLFW_MOUSE_BUTTON_LEFT)
		btn = Seed::ButtonLeft;
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		btn = Seed::ButtonRight;
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		btn = Seed::ButtonMiddle;
	else if (button == GLFW_MOUSE_BUTTON_4)
		btn = Seed::Button5;
	else if (button == GLFW_MOUSE_BUTTON_5)
		btn = Seed::Button6;
	else if (button == GLFW_MOUSE_BUTTON_6)
		btn = Seed::Button7;
	else if (button == GLFW_MOUSE_BUTTON_7)
		btn = Seed::Button8;
	else if (button == GLFW_MOUSE_BUTTON_8)
		btn = Seed::Button9;
	else if (button == GLFW_MOUSE_BUTTON_LAST + 1)
		btn = Seed::ButtonDown;
	else if (button == GLFW_MOUSE_BUTTON_LAST + 2)
		btn = Seed::ButtonUp;

	return btn;
}

u32 Input::ConvertButtonFlags(u32 flags)
{
	return this->GetButtonCode(flags);
}

u32 Input::GetSensitivity(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

u32 Input::GetMaximumSensitivity(u16 joystick) const
{
	UNUSED(joystick);
	return 0;
}

u32 Input::GetMaximumPointers() const
{
	return 0;
}

void Input::SetSensitivity(u32 sens, u16 joystick)
{
	UNUSED(sens);
	UNUSED(joystick);
}

Seed::eKey Input::GetKeyCode(u32 key) const
{
	Seed::eKey k = static_cast<Seed::eKey>(key);

	return k;
}

Seed::eModifier Input::GetModifierCode(u32 mod) const
{
	Seed::eModifier m = static_cast<Seed::eModifier>(mod);

	return m;
}

u32 Input::GetMaximumJoysticks() const
{
	return iJoystickCount;
}

bool Input::IsPointer() const
{
	return true;
}

bool Input::IsJoystick() const
{
	return iJoystickCount > 0;
}

bool Input::IsKeyboard() const
{
	return true;
}

}} // namespace

#endif // BUILD_GLFW