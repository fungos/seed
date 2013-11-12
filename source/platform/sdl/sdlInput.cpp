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

#if defined(BUILD_SDL)

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

#if defined(WIN32)
#include <SDL/SDL_syswm.h>
#endif

#if defined(EMSCRIPTEN)
#define SDL_EVENT_KEY_WHICH 0
#else
#define SDL_EVENT_KEY_WHICH event.key.which
#endif

#define TAG "[Input] "

namespace Seed { namespace SDL {

SEED_SINGLETON_DEFINE(Input)

Input::Input()
	: iJoystickCount(0)
	, iX(0)
	, iY(0)
{
}

Input::~Input()
{
	this->Reset();
}

bool Input::Shutdown()
{
	Log(TAG "Terminating...");

	for (u32 i = 0; i < iJoystickCount; i++)
	{
		if (SDL_JoystickOpened(i))
			SDL_JoystickClose(parJoy[i]);
	}

	memset(parJoy, '\0', sizeof(parJoy));

	bool r = this->Reset();
	Log(TAG "Terminated.");

	return r;
}

bool Input::Initialize()
{
	Log(TAG "Initializing...");
	bool r = this->Reset();
	#if defined(WIN32) && defined(DEBUG)
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
	#endif

	memset(parJoy, '\0', sizeof(parJoy));
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	iJoystickCount = SDL_NumJoysticks();
	if (iJoystickCount)
	{
		SDL_JoystickEventState(SDL_ENABLE);

		Log(TAG "Joystick(s): ");
		for (u32 i = 0; i < iJoystickCount; i++)
		{
			parJoy[i] = SDL_JoystickOpen(i);
			if (parJoy[i])
			{
				Log("Opened Joystick %d:", i);
				Log(TAG "\tName: %s", SDL_JoystickName(i));
				Log(TAG "\t\tAxes: %d", SDL_JoystickNumAxes(parJoy[i]));
				Log(TAG "\t\tButtons: %d", SDL_JoystickNumButtons(parJoy[i]));
				Log(TAG "\t\tHats: %d", SDL_JoystickNumHats(parJoy[i]));
				Log(TAG "\t\tBalls: %d", SDL_JoystickNumBalls(parJoy[i]));
			}
		}
	}
	Log(TAG "Initialization completed.");

	return r;
}

bool Input::Update(f32 dt)
{
	UNUSED(dt);

/*
FIXME: 2009-02-17 | BUG | Usar polling? Isso deve ferrar com o frame rate configurado pelo usuario. Verificar tambem... | Danny Angelo Carminati Grein
*/
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			#if defined(WIN32) && defined(DEBUG)
			case SDL_SYSWMEVENT:
			{
				switch (event.syswm.msg->msg)
				{
					case WM_SYSCOMMAND:
					case WM_IME_SETCONTEXT:
					case WM_IME_NOTIFY:
					break;

					case WM_GETTEXT:
					case WM_GETICON:
					case WM_NCHITTEST:
					case WM_NCMOUSEMOVE:
					case WM_NCLBUTTONDOWN:
					case WM_NCLBUTTONDBLCLK:
					case WM_NCLBUTTONUP:
					case WM_CAPTURECHANGED:
					case WM_WINDOWPOSCHANGING:
					case WM_WINDOWPOSCHANGED:
					case WM_MOVE:
					case WM_MOVING:
					case WM_ENTERSIZEMOVE:
					case WM_EXITSIZEMOVE:
					case WM_MOUSEACTIVATE:
					case WM_NCCALCSIZE:
					case WM_SIZE:
					case WM_QUERYOPEN:
					break;

					case WM_DISPLAYCHANGE:
						Log(TAG "event DISPLAYCHANGE");
					break;

					case WM_SYNCPAINT:
						Log(TAG "event SYNCPAINT");
					break;

					case WM_NCPAINT:
						Log(TAG "event NCPAINT");
					break;

					case WM_NCACTIVATE:
						Log(TAG "event NCACTIVATE");
					break;

					case WM_KILLFOCUS:
						Log(TAG "event KILLFOCUS");
					break;

					case WM_SETFOCUS:
						Log(TAG "event SETFOCUS");
					break;

					case WM_ACTIVATEAPP:
						Log(TAG "event ACTIVATEAPP");
					break;

					case 0xc086: //WM_TASKBAR_CREATED:
						Log(TAG "event TASKBAR_CREATED");
					break;

					case WM_DWMCOMPOSITIONCHANGED:
						Log(TAG "event DWMCOMPOSITIONCHANGED");
					break;

					default:
						Log(TAG "Received system event. Message (0x%x) wParam = %d, lParam = %d.", event.syswm.msg->msg, event.syswm.msg->wParam, event.syswm.msg->lParam);
					break;
				}
			}
			break;
			#endif

			case SDL_KEYDOWN:
			{
				EventInputKeyboard ev(GetKeyCode(event.key.keysym.sym), GetModifierCode(event.key.keysym.mod), event.key.keysym.scancode, SDL_EVENT_KEY_WHICH); // key.which deprecated
				this->SendEventKeyboardPress(&ev);
			}
			break;

			case SDL_KEYUP:
			{
				EventInputKeyboard ev(GetKeyCode(event.key.keysym.sym), GetModifierCode(event.key.keysym.mod), event.key.keysym.scancode, SDL_EVENT_KEY_WHICH); // key.which deprecated
				this->SendEventKeyboardRelease(&ev);
			}
			break;

			case SDL_QUIT:
			{
				EventSystem ev;
				pSystem->SendEventShutdown(&ev);
			}
			break;

			case SDL_MOUSEMOTION:
			{
				u32 x, y;
				x = iX = event.motion.x;
				y = iY = event.motion.y;

				EventInputPointer ev(0, eInputButton::None, eInputButton::None, eInputButton::None, x, y);
				this->SendEventPointerMove(&ev);
			}
			break;

			case SDL_MOUSEBUTTONUP:
			{
				u32 x, y;
				x = iX = event.motion.x;
				y = iY = event.motion.y;

				const EventInputPointer ev(0, eInputButton::None, eInputButton::None, eInputButton(this->ConvertButtonFlags(event.button.button)), x, y);
				this->SendEventPointerRelease(&ev);
			}
			break;

			case SDL_MOUSEBUTTONDOWN:
			{
				u32 x, y;
				x = iX = event.motion.x;
				y = iY = event.motion.y;

				const EventInputPointer ev(0, eInputButton(this->ConvertButtonFlags(event.button.button)), eInputButton::None, eInputButton::None, x, y);
				this->SendEventPointerPress(&ev);
			}
			break;

			case SDL_JOYBUTTONDOWN:
			{
				const EventInputJoystick ev(event.jbutton.which, event.jbutton.button, 0, 0, 0, 0);
				this->SendEventJoystickButtonPress(&ev);
			}
			break;

			case SDL_JOYBUTTONUP:
			{
				const EventInputJoystick ev(event.jbutton.which, 0, 0, event.jbutton.button, 0, 0);
				this->SendEventJoystickButtonRelease(&ev);
			}
			break;

			case SDL_JOYAXISMOTION:
			{
				const EventInputJoystick ev(event.jbutton.which, 0, 0, 0, event.jaxis.axis, event.jaxis.value);
				this->SendEventJoystickAxisMove(&ev);
			}
			break;

			case SDL_JOYHATMOTION:
			{
				const EventInputJoystick ev(event.jbutton.which, 0, 0, 0, event.jhat.hat, event.jhat.value);
				this->SendEventJoystickDPadMove(&ev);
			}
			break;

			case SDL_JOYBALLMOTION:
			break;

			default:
			break;
		}
	}

	return true;
}

bool Input::Reset()
{
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
	return static_cast<f32>(iX);
}

f32 Input::GetY(u16 joystick) const
{
	UNUSED(joystick);
	return static_cast<f32>(iY);
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
	eInputButton btn = eInputButton::Invalid;

	if (button & SDL_BUTTON_LMASK)
		btn = eInputButton::Left;
	else if (button & SDL_BUTTON_RMASK)
		btn = eInputButton::Right;
	else if (button & SDL_BUTTON_MMASK)
		btn = eInputButton::Middle;
	else if (button & SDL_BUTTON(SDL_BUTTON_WHEELUP))
		btn = eInputButton::Up;
	else if (button & SDL_BUTTON(SDL_BUTTON_WHEELDOWN))
		btn = eInputButton::Down;

	return btn;
}

u32 Input::ConvertButtonFlags(u32 flags)
{
	u32 converted = 0;

	converted |= u32(this->GetButtonCode(SDL_BUTTON(flags)));

	return converted;
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

eKey Input::GetKeyCode(u32 key) const
{
	if (key >= 'a' && key <= 'z')
		return static_cast<eKey>(u32(eKey::A) + (key - 'a'));
	else
	{
		eKey k = static_cast<eKey>(key);
		return k;
	}
}

eModifier Input::GetModifierCode(u32 mod) const
{
	eModifier m = static_cast<eModifier>(mod);

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

//bool Input::IsMotion() const;
bool Input::IsJoystick() const
{
	return iJoystickCount > 0;
}

bool Input::IsKeyboard() const
{
	return true;
}

}} // namespace

#endif // BUILD_SDL
