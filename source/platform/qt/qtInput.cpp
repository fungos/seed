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
	IManager::Shutdown();
	Log(TAG "Terminated.");

	return r;
}

bool Input::Initialize()
{
	Log(TAG "Initializing...");
	IManager::Initialize();
	bool r = this->Reset();
	Log(TAG "Initialization completed.");

	return r;
}

bool Input::Update(Seconds dt)
{
	UNUSED(dt);
	return true;
}

bool Input::Reset()
{
	IManager::Reset();
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
			m = Seed::ModifierScrollLock;
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
		eKey t = eKey::None;

		switch (key)
		{
			case Qt::Key_Escape:
				t = eKey::Escape;
			break;

			case Qt::Key_Tab:
				t = eKey::Tab;
			break;

			case Qt::Key_Backspace:
				t = eKey::Backspace;
			break;

			case Qt::Key_Return:
				t = eKey::Return;
			break;

			case Qt::Key_Enter:
				t = eKey::Enter;
			break;

			case Qt::Key_Pause:
				t = eKey::Pause;
			break;

			case Qt::Key_Clear:
				t = eKey::Clear;
			break;

			case Qt::Key_Delete:
				t = eKey::Delete;
			break;

			case Qt::Key_Insert:
				t = eKey::Insert;
			break;

			case Qt::Key_Print:
				t = eKey::PrintScreen;
			break;

			case Qt::Key_SysReq:

			case Qt::Key_Home:
				t = eKey::Home;
			break;

			case Qt::Key_End:
				t = eKey::End;
			break;

			case Qt::Key_Left:
				t = eKey::Left;
			break;

			case Qt::Key_Up:
				t = eKey::Up;
			break;

			case Qt::Key_Down:
				t = eKey::Down;
			break;

			case Qt::Key_Right:
				t = eKey::Right;
			break;

			case Qt::Key_PageUp:
				t = eKey::PageUp;
			break;

			case Qt::Key_PageDown:
				t = eKey::PageDown;
			break;

			case Qt::Key_Shift:
				t = eKey::ShiftLeft;
			break;

			case Qt::Key_Control:
				t = eKey::ControlLeft;
			break;

			case Qt::Key_Meta:
				t = eKey::MetaLeft;
			break;

			case Qt::Key_Alt:
				t = eKey::AltLeft;
			break;

			case Qt::Key_Super_L:
				t = eKey::SuperLeft;
			break;

			case Qt::Key_Super_R:
				t = eKey::SuperRight;
			break;

			case Qt::Key_AltGr:
				t = eKey::AltGr;
			break;

			case Qt::Key_CapsLock:
				t = eKey::CapsLock;
			break;

			case Qt::Key_NumLock:
				t = eKey::NumLock;
			break;

			case Qt::Key_ScrollLock:
				t = eKey::ScrollLock;
			break;

			case Qt::Key_F1:
				t = eKey::F1;
			break;

			case Qt::Key_F2:
				t = eKey::F2;
			break;

			case Qt::Key_F3:
				t = eKey::F3;
			break;

			case Qt::Key_F4:
				t = eKey::F4;
			break;

			case Qt::Key_F5:
				t = eKey::F5;
			break;

			case Qt::Key_F6:
				t = eKey::F6;
			break;

			case Qt::Key_F7:
				t = eKey::F7;
			break;

			case Qt::Key_F8:
				t = eKey::F8;
			break;

			case Qt::Key_F9:
				t = eKey::F9;
			break;

			case Qt::Key_F10:
				t = eKey::F10;
			break;

			case Qt::Key_F11:
				t = eKey::F11;
			break;

			case Qt::Key_F12:
				t = eKey::F12;
			break;

			case Qt::Key_F13:
				t = eKey::F13;
			break;

			case Qt::Key_F14:
				t = eKey::F14;
			break;

			case Qt::Key_F15:
				t = eKey::F15;
			break;

			case Qt::Key_Menu:
				t = eKey::Menu;
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
