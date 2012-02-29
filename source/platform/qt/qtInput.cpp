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
#if defined(BUILD_QT)

#include "Defines.h"
#include "Input.h"
#include "Log.h"
#include "Screen.h"
#include "Trigonometry.h"
#include "System.h"
#include "Input.h"
#include "EventInputPointer.h"
#include "EventInputKeyboard.h"
#include "Enum.h"

#define TAG "[Input] "

namespace Seed { namespace QT {

SEED_SINGLETON_DEFINE(Input)

Input::Input()
	: fX(0.0f)
	, fY(0.0f)
{
}

Input::~Input()
{
	Reset();
}

bool Input::Shutdown()
{
	Log(TAG "Terminating...");
	bool r = this->Reset();
	IModule::Shutdown();
	Log(TAG "Terminated.");

	return r;
}

bool Input::Initialize()
{
	Log(TAG "Initializing...");
	IModule::Initialize();
	bool r = this->Reset();
	Log(TAG "Initialization completed.");

	return r;
}

bool Input::Update(f32 dt)
{
	UNUSED(dt);
	return true;
}

bool Input::Reset()
{
	IModule::Reset();
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

bool Input::IsEnabled(u16 joystick) const
{
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

eInputButton Input::GetButtonCode(u32 button) const
{
	UNUSED(button);
	return Seed::ButtonLeft;
}

u32 Input::ConvertButtonFlags(u32 flags)
{
	UNUSED(flags);
	return 0;
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

bool Input::IsPointer() const
{
	return true;
}

bool Input::IsKeyboard() const
{
	return true;
}

void Input::PointerMove(f32 x, f32 y, u16 joystick)
{
	EventInputPointer ev(joystick, 0, 0, 0, x, y);
	this->SendEventPointerMove(&ev);
}

void Input::PointerPress(f32 x, f32 y, u32 btn, u16 joystick)
{
	EventInputPointer ev(joystick, btn, 0, 0, x, y);
	this->SendEventPointerPress(&ev);
}

void Input::PointerRelease(f32 x, f32 y, u32 btn, u16 joystick)
{
	EventInputPointer ev(joystick, 0, 0, btn, x, y);
	this->SendEventPointerRelease(&ev);
}

void Input::KeyboardPress(u32 key, u32 modifier, u16 joystick)
{
	UNUSED(joystick)
	EventInputKeyboard ev(this->TranslateKey(key), this->TranslateModifier(modifier), key);
	this->SendEventKeyboardPress(&ev);
}

void Input::KeyboardRelease(u32 key, u32 modifier, u16 joystick)
{
	UNUSED(joystick)
	EventInputKeyboard ev(this->TranslateKey(key), this->TranslateModifier(modifier), key);
	this->SendEventKeyboardRelease(&ev);
}

eModifier Input::TranslateModifier(u32 mod)
{
	eModifier m = Seed::ModifierNone;

	switch (mod)
	{
		case Qt::ShiftModifier:
		{
			m = Seed::ModifierShift;
		}
		break;

		case Qt::ControlModifier:
		{
			m = Seed::ModifierControl;
		}
		break;

		case Qt::AltModifier:
		{
			m = Seed::ModifierAlt;
		}
		break;

		case Qt::MetaModifier:
		{
			m = Seed::ModifierMeta;
		}
		break;

		case Qt::KeypadModifier:
		{
			m = Seed::ModifierKeypad;
		}
		break;

		case Qt::NoModifier:
		default:
		{
		}
		break;
	}

	return m;
}

eKey Input::TranslateKey(u32 key)
{
	u32 k = key;

	if (k >= 'a' && k <= 'z')
	{
		k = key - 'a' - 'A';
	}
	else if (k >= ' ' && k <= '~')
	{
		k = key;
	}
	else
	{
		eKey t = KeyNone;

		switch (key)
		{
			case Qt::Key_Escape:
				t = KeyEscape;
			break;

			case Qt::Key_Tab:
				t = KeyTab;
			break;

			case Qt::Key_Backspace:
				t = KeyBackspace;
			break;

			case Qt::Key_Return:
				t = KeyReturn;
			break;

			case Qt::Key_Enter:
				t = KeyEnter;
			break;

			case Qt::Key_Pause:
				t = KeyPause;
			break;

			case Qt::Key_Clear:
				t = KeyClear;
			break;

			case Qt::Key_Delete:
				t = KeyDelete;
			break;

			case Qt::Key_Insert:
				t = KeyInsert;
			break;

			case Qt::Key_Print:
				t = KeyPrintScreen;
			break;

			case Qt::Key_SysReq:

			case Qt::Key_Home:
				t = KeyHome;
			break;

			case Qt::Key_End:
				t = KeyEnd;
			break;

			case Qt::Key_Left:
				t = KeyLeft;
			break;

			case Qt::Key_Up:
				t = KeyUp;
			break;

			case Qt::Key_Down:
				t = KeyDown;
			break;

			case Qt::Key_Right:
				t = KeyRight;
			break;

			case Qt::Key_PageUp:
				t = KeyPageUp;
			break;

			case Qt::Key_PageDown:
				t = KeyPageDown;
			break;

			case Qt::Key_Shift:
				t = KeyShiftLeft;
			break;

			case Qt::Key_Control:
				t = KeyControlLeft;
			break;

			case Qt::Key_Meta:
				t = KeyMetaLeft;
			break;

			case Qt::Key_Alt:
				t = KeyAltLeft;
			break;

			case Qt::Key_Super_L:
				t = KeySuperLeft;
			break;

			case Qt::Key_Super_R:
				t = KeySuperRight;
			break;

			case Qt::Key_AltGr:
				t = KeyAltGr;
			break;

			case Qt::Key_CapsLock:
				t = KeyCapsLock;
			break;

			case Qt::Key_NumLock:
				t = KeyNumLock;
			break;

			case Qt::Key_ScrollLock:
				t = KeyScrollLock;
			break;

			case Qt::Key_F1:
				t = KeyF1;
			break;

			case Qt::Key_F2:
				t = KeyF2;
			break;

			case Qt::Key_F3:
				t = KeyF3;
			break;

			case Qt::Key_F4:
				t = KeyF4;
			break;

			case Qt::Key_F5:
				t = KeyF5;
			break;

			case Qt::Key_F6:
				t = KeyF6;
			break;

			case Qt::Key_F7:
				t = KeyF7;
			break;

			case Qt::Key_F8:
				t = KeyF8;
			break;

			case Qt::Key_F9:
				t = KeyF9;
			break;

			case Qt::Key_F10:
				t = KeyF10;
			break;

			case Qt::Key_F11:
				t = KeyF11;
			break;

			case Qt::Key_F12:
				t = KeyF12;
			break;

			case Qt::Key_F13:
				t = KeyF13;
			break;

			case Qt::Key_F14:
				t = KeyF14;
			break;

			case Qt::Key_F15:
				t = KeyF15;
			break;

			case Qt::Key_Menu:
				t = KeyMenu;
			break;

			case Qt::Key_F16:
			case Qt::Key_F17:
			case Qt::Key_F18:
			case Qt::Key_F19:
			case Qt::Key_F20:
			case Qt::Key_F21:
			case Qt::Key_F22:
			case Qt::Key_F23:
			case Qt::Key_F24:
			case Qt::Key_F25:
			case Qt::Key_F26:
			case Qt::Key_F27:
			case Qt::Key_F28:
			case Qt::Key_F29:
			case Qt::Key_F30:
			case Qt::Key_F31:
			case Qt::Key_F32:
			case Qt::Key_F33:
			case Qt::Key_F34:
			case Qt::Key_F35:
			case Qt::Key_Hyper_L:
			case Qt::Key_Hyper_R:
			case Qt::Key_Help:
			case Qt::Key_Direction_L:
			case Qt::Key_Direction_R:
			case Qt::Key_unknown:
			default:
			break;
		}

		k = static_cast<u32>(t);
	}

	return static_cast<eKey>(k);
}

}} // namespace

#endif // BUILD_QT
