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
#include "renderer/Viewport.h"
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

void GLFWCALL glfwOnMouseWheelCb(int pos, int btn)
{
	if (btn)
	{
		pInput->OnMouseButtonCb(GLFW_MOUSE_BUTTON_LAST + btn, GLFW_PRESS);
		pInput->OnMouseButtonCb(GLFW_MOUSE_BUTTON_LAST + btn, GLFW_RELEASE);
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
	, iOriginX(0)
	, iOriginY(0)
	, iOldX(0)
	, iOldY(0)
	, iX(0)
	, iY(0)
	, iAxesMax(0)
	, iButtonsMax(0)
	, arJoyInfo()
	, iModifiers(static_cast<u32>(eModifier::None))
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

static eKey _glfwSpecialKeys[70] = {
	eKey::None, eKey::Escape, eKey::F1, eKey::F2, eKey::F3, eKey::F4, eKey::F5, eKey::F6, eKey::F7, eKey::F8, eKey::F9, eKey::F10, eKey::F11, eKey::F12, eKey::F13, eKey::F14, eKey::F15,
	eKey::None, eKey::None, eKey::None, eKey::None, eKey::None, eKey::None, eKey::None, eKey::None, eKey::None, eKey::None, // F16-F25
	eKey::Up, eKey::Down, eKey::Left, eKey::Right, eKey::ShiftLeft, eKey::ShiftRight, eKey::ControlLeft, eKey::ControlRight, eKey::AltLeft, eKey::AltRight, eKey::Tab,
	eKey::Enter, eKey::Backspace, eKey::Insert, eKey::Delete, eKey::PageUp, eKey::PageDown, eKey::Home, eKey::End, eKey::Pad0, eKey::Pad1, eKey::Pad2, eKey::Pad3, eKey::Pad4,
	eKey::Pad5, eKey::Pad6, eKey::Pad7, eKey::Pad8, eKey::Pad9, eKey::PadDivide, eKey::PadMultiply, eKey::PadMinus, eKey::PadPlus, eKey::PadPeriod, eKey::PadEquals,
	eKey::PadEnter, eKey::NumLock, eKey::CapsLock, eKey::ScrollLock, eKey::Pause, eKey::MetaLeft, eKey::MetaRight, eKey::Menu
};

void Input::OnKeyCb(int key, int action)
{
	eKey nk = eKey::None;

	if (key > GLFW_KEY_LAST)
		return;

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_LALT:
				iModifiers |= static_cast<u32>(eModifier::AltLeft);
			break;

			case GLFW_KEY_RALT:
				iModifiers |= static_cast<u32>(eModifier::AltRight);
			break;

			case GLFW_KEY_LSHIFT:
				iModifiers |= static_cast<u32>(eModifier::ShiftLeft);
			break;

			case GLFW_KEY_RSHIFT:
				iModifiers |= static_cast<u32>(eModifier::ShiftRight);
			break;

			case GLFW_KEY_LCTRL:
				iModifiers |= static_cast<u32>(eModifier::ControlLeft);
			break;

			case GLFW_KEY_RCTRL:
				iModifiers |= static_cast<u32>(eModifier::ControlRight);
			break;

			case GLFW_KEY_LSUPER:
				iModifiers |= static_cast<u32>(eModifier::MetaLeft);
			break;

			case GLFW_KEY_RSUPER:
				iModifiers |= static_cast<u32>(eModifier::MetaRight);
			break;

			case GLFW_KEY_CAPS_LOCK:
				iModifiers |= static_cast<u32>(eModifier::CapsLock);
			break;

			case GLFW_KEY_KP_NUM_LOCK:
				iModifiers |= static_cast<u32>(eModifier::NumLock);
			break;

			case GLFW_KEY_SCROLL_LOCK:
				iModifiers |= static_cast<u32>(eModifier::ScrollLock);
			break;

			default:
			break;
		}

		if (key >= GLFW_KEY_SPECIAL)
			nk = _glfwSpecialKeys[key-GLFW_KEY_SPECIAL];
		else
			nk = static_cast<eKey>(key);

		EventInputKeyboard ev(nk, static_cast<eModifier>(iModifiers), static_cast<u32>(nk), static_cast<u32>(nk));
		this->SendEventKeyboardPress(&ev);
	}
	else
	{
		switch (key)
		{
			case GLFW_KEY_LALT:
				iModifiers ^= static_cast<u32>(eModifier::AltLeft);
			break;

			case GLFW_KEY_RALT:
				iModifiers ^= static_cast<u32>(eModifier::AltRight);
			break;

			case GLFW_KEY_LSHIFT:
				iModifiers ^= static_cast<u32>(eModifier::ShiftLeft);
			break;

			case GLFW_KEY_RSHIFT:
				iModifiers ^= static_cast<u32>(eModifier::ShiftRight);
			break;

			case GLFW_KEY_LCTRL:
				iModifiers ^= static_cast<u32>(eModifier::ControlLeft);
			break;

			case GLFW_KEY_RCTRL:
				iModifiers ^= static_cast<u32>(eModifier::ControlRight);
			break;

			case GLFW_KEY_LSUPER:
				iModifiers ^= static_cast<u32>(eModifier::MetaLeft);
			break;

			case GLFW_KEY_RSUPER:
				iModifiers ^= static_cast<u32>(eModifier::MetaRight);
			break;

			case GLFW_KEY_CAPS_LOCK:
				iModifiers ^= static_cast<u32>(eModifier::CapsLock);
			break;

			case GLFW_KEY_KP_NUM_LOCK:
				iModifiers ^= static_cast<u32>(eModifier::NumLock);
			break;

			case GLFW_KEY_SCROLL_LOCK:
				iModifiers ^= static_cast<u32>(eModifier::ScrollLock);
			break;

			default:
			break;
		}

		if (key >= GLFW_KEY_SPECIAL)
			nk = _glfwSpecialKeys[key-GLFW_KEY_SPECIAL];
		else
			nk = static_cast<eKey>(key);

		EventInputKeyboard ev(nk, static_cast<eModifier>(iModifiers), static_cast<u32>(nk), static_cast<u32>(nk));
		this->SendEventKeyboardRelease(&ev);
	}
}

void Input::OnMouseButtonCb(int button, int action)
{
	int xp = 0;
	int yp = 0;
	glfwGetMousePos(&xp, &yp);

	if (action == GLFW_RELEASE)
	{
		const EventInputPointer ev(0, eInputButton::None, eInputButton::None, this->GetMouseButtonCode(button), xp, yp);
		this->SendEventPointerRelease(&ev);
	}
	else
	{
		const EventInputPointer ev(0, this->GetMouseButtonCode(button), eInputButton::None, eInputButton::None, xp, yp);
		this->SendEventPointerPress(&ev);
	}
}

void Input::OnMouseWheelCb(int pos)
{
	iOldWheel = iWheel;
	iWheel = pos;
}

void Input::OnMousePosCb(int xp, int yp)
{
	iOldX = iX;
	iX = xp;

	iOldY = iY;
	iY = yp;

	EventInputPointer ev(0, eInputButton::None, eInputButton::None, eInputButton::None, xp, yp);
	this->SendEventPointerMove(&ev);
}

bool Input::Update(Seconds dt)
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
			const EventInputJoystick ev(i, eInputButton::None, eInputButton::None, eInputButton::None, axis, axes[axis]);
			this->SendEventJoystickAxisMove(&ev);
		}

		amount = glfwGetJoystickButtons(i, buttons, iButtonsMax);
		for (int btn = 0; btn < amount; btn++)
		{
			eInputButton btnCode = this->GetJoystickButtonCode(btn);

			if (buttons[btn] == GLFW_PRESS)
			{
				const EventInputJoystick ev(i, btnCode, eInputButton::None, eInputButton::None, 0, 0);

				if(btnCode == eInputButton::JoystickUp || btnCode == eInputButton::JoystickRight ||
					btnCode == eInputButton::JoystickDown || btnCode == eInputButton::JoystickLeft)
					this->SendEventJoystickDPadMove(&ev);
				else
					this->SendEventJoystickButtonPress(&ev);
			}
			else
			{
				const EventInputJoystick ev(i, eInputButton::None, eInputButton::None, btnCode, 0, 0);
				this->SendEventJoystickButtonRelease(&ev);
			}
		}
	}

	return true;
}

bool Input::Reset()
{
	memset(arJoyInfo, '\0', sizeof(arJoyInfo));
	iModifiers = static_cast<u32>(eModifier::None);
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

eInputButton Input::GetMouseButtonCode(u32 button) const
{
	eInputButton btn = eInputButton::Invalid;

	if (button == GLFW_MOUSE_BUTTON_LEFT)
		btn = eInputButton::MouseLeft;
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		btn = eInputButton::MouseRight;
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		btn = eInputButton::MouseMiddle;
	else if (button == GLFW_MOUSE_BUTTON_4)
		btn = eInputButton::Button5;
	else if (button == GLFW_MOUSE_BUTTON_5)
		btn = eInputButton::Button6;
	else if (button == GLFW_MOUSE_BUTTON_6)
		btn = eInputButton::Button7;
	else if (button == GLFW_MOUSE_BUTTON_7)
		btn = eInputButton::Button8;
	else if (button == GLFW_MOUSE_BUTTON_8)
		btn = eInputButton::Button9;
	else if (button == GLFW_MOUSE_BUTTON_LAST + 1)
		btn = eInputButton::MouseDown;
	else if (button == GLFW_MOUSE_BUTTON_LAST + 2)
		btn = eInputButton::MouseUp;

	return btn;
}

eInputButton Input::GetJoystickButtonCode(u32 button) const
{
	eInputButton btn = eInputButton::Invalid;

	if (button == GLFW_JOYSTICK_1)
		btn = eInputButton::Button0;
	else if (button == GLFW_JOYSTICK_2)
		btn = eInputButton::Button1;
	else if (button == GLFW_JOYSTICK_3)
		btn = eInputButton::Button2;
	else if (button == GLFW_JOYSTICK_4)
		btn = eInputButton::Button3;
	else if (button == GLFW_JOYSTICK_5)
		btn = eInputButton::Button4;
	else if (button == GLFW_JOYSTICK_6)
		btn = eInputButton::Button5;
	else if (button == GLFW_JOYSTICK_7)
		btn = eInputButton::Button6;
	else if (button == GLFW_JOYSTICK_8)
		btn = eInputButton::Button7;
	else if (button == GLFW_JOYSTICK_9)
		btn = eInputButton::Button8;
	else if (button == GLFW_JOYSTICK_10)
		btn = eInputButton::Button9;
	else if (button == GLFW_JOYSTICK_11)
		btn = eInputButton::Button10;
	else if (button == GLFW_JOYSTICK_12)
		btn = eInputButton::Button11;
	else if (button == GLFW_JOYSTICK_13)
		btn = eInputButton::Button12;
	else if (button == GLFW_JOYSTICK_14)
		btn = eInputButton::Button13;
	else if (button == GLFW_JOYSTICK_15)
		btn = eInputButton::Button14;
	else if (button == GLFW_JOYSTICK_16)
		btn = eInputButton::Button15;

	return btn;
}

u32 Input::ConvertButtonFlags(u32 flags)
{
	return static_cast<u32>(this->GetMouseButtonCode(flags));
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
