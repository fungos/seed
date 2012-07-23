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

#if defined(BUILD_IOS)

#include "Defines.h"
#include "Input.h"
#include "Log.h"
#include "Screen.h"
#include "platform/ios/iosoneView.h"
#include "EventInputPointer.h"
#include "Timer.h"

#define TAG "[Input] "

namespace Seed { namespace iOS {

SEED_SINGLETON_DEFINE(Input);

Input::Input()
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
	Log(TAG "Terminated.");

	return r;
}

bool Input::Initialize()
{
	Log(TAG "Initializing...");
	bool r = this->Reset();
	Log(TAG "Initialization completed.");

	return r;
}

bool Input::Update(f32 delta)
{
	UNUSED(delta);
	for (u32 i = 0; i < PLATFORM_MAX_INPUT; i++)
	{
		memcpy(&oldState[i], &curState[i], sizeof(curState[i]));
		memcpy(&curState[i].touch, &iphTouchBuff[i], sizeof(iphTouchBuff[i]));

		switch (oldState[i].touch.bStatus)
		{
			case 0: // Nada
			{
				if (curState[i].touch.bStatus == 0)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 0;
					curState[i].bValid = true;
				}
				else if (curState[i].touch.bStatus == 1)
				{
					curState[i].iTrig = 1;//curState[i].touch.iTaps;
					curState[i].iHold = 0;
					curState[i].iRelease = 0;
					curState[i].bValid = true;
				}
				else if (curState[i].touch.bStatus == 2) // nunca deve acontecer
				{
					curState[i].iTrig = 1;//curState[i].touch.iTaps;
					curState[i].iHold = 1;//curState[i].touch.iTaps;
					curState[i].iRelease = 0;
					curState[i].bValid = false;
					Log(TAG "Invalid State: 0 -> 2");
				}
				else if (curState[i].touch.bStatus == 3) // nunca deve acontecer
				{
					curState[i].iTrig = 1;//curState[i].touch.iTaps;
					curState[i].iHold = 0;
					curState[i].iRelease = 1;//curState[i].touch.iTaps;
					curState[i].bValid = false;
					Log(TAG "Invalid State: 0 -> 3");
				}
			}
			break;

			case 1: // Trig
			{
				if (curState[i].touch.bStatus == 0)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 1;//oldState[i].touch.iTaps;
					curState[i].bValid = true;
				}
				else if (curState[i].touch.bStatus == 1) // clica e segura sem mover
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 1;//oldState[i].touch.iTaps;
					curState[i].iRelease = 0;
					curState[i].bValid = true;
				}
				else if (curState[i].touch.bStatus == 2)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 1;//oldState[i].touch.iTaps;
					curState[i].iRelease = 0;
					curState[i].bValid = true;
				}
				else if (curState[i].touch.bStatus == 3)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 1;//oldState[i].touch.iTaps;
					curState[i].bValid = true;
				}
			}
			break;

			case 2: // Move
			{
				if (curState[i].touch.bStatus == 0)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 1;//oldState[i].touch.iTaps;
					curState[i].bValid = true;
				}
				else if (curState[i].touch.bStatus == 1) // nunca deve acontecer
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 1;//oldState[i].touch.iTaps;
					curState[i].iRelease = 0;
					curState[i].bValid = false;
					Log(TAG "Invalid State: 2 -> 1");
				}
				else if (curState[i].touch.bStatus == 2)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 1;//oldState[i].touch.iTaps;
					curState[i].iRelease = 0;
					curState[i].bValid = true;
				}
				else if (curState[i].touch.bStatus == 3)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 1;//oldState[i].touch.iTaps;
					curState[i].bValid = true;
				}
			}
			break;

			case 3: // Release
			{
				if (curState[i].touch.bStatus == 0)
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 0;
					curState[i].bValid = true;
				}
				else if (curState[i].touch.bStatus == 1)
				{
					curState[i].iTrig = 1;//curState[i].touch.iTaps;
					curState[i].iHold = 0;
					curState[i].iRelease = 0;
					curState[i].bValid = true;
				}
				else if (curState[i].touch.bStatus == 2) // nunca deve acontecer
				{
					curState[i].iTrig = 1;//curState[i].touch.iTaps;
					curState[i].iHold = 1;//curState[i].touch.iTaps;
					curState[i].iRelease = 0;
					curState[i].bValid = false;
					Log(TAG "Invalid State: 3 -> 2");
				}
				else if (curState[i].touch.bStatus == 3) // nada mudou no frame
				{
					curState[i].iTrig = 0;
					curState[i].iHold = 0;
					curState[i].iRelease = 0;
					curState[i].bValid = true;
				}
			}
			break;

			default:
			break;
		}

		/*
		if (oldState[i].touch.bStatus != curState[i].touch.bStatus)
		{
			fprintf(stdout, "Old: %d iTrig: %x iHold: %x iRelease: %x iTaps: %d\n", oldState[i].touch.bStatus, oldState[i].iTrig, oldState[i].iHold, oldState[i].iRelease, oldState[i].touch.iTaps);
			fprintf(stdout, "New: %d iTrig: %x iHold: %x iRelease: %x iTaps: %d\n", curState[i].touch.bStatus, curState[i].iTrig, curState[i].iHold, curState[i].iRelease, curState[i].touch.iTaps);

			Log("");
		}

		if (!curState[i].bValid)
			Log(TAG "Invalid state happened.");
		*/
	}

	this->SendEvents();

	return true;
}

bool Input::Reset()
{
	return true;
}

bool Input::IsPressed(u32 button, u16 joystick) const
{
	return (curState[joystick].iTrig && button);
}

bool Input::IsHold(u32 button, u16 joystick) const
{
	return (curState[joystick].iHold && button);
}

bool Input::IsReleased(u32 button, u16 joystick) const
{
	return (curState[joystick].iRelease && button);
}

bool Input::IsPointerEnabled(u16 joystick) const
{
	return (joystick == 0);
}

f32 Input::GetX(u16 joystick) const
{
	f32 w = pScreen->GetWidth();
	f32 x = curState[joystick].touch.fPosX;

	return x / w;
}

f32 Input::GetY(u16 joystick) const
{
	f32 h = pScreen->GetHeight();
	f32 y = curState[joystick].touch.fPosY;

	return y / h;
}

f32 Input::GetRelativeX(u16 joystick) const
{
	f32 w = pScreen->GetWidth();

	//f32 nX = curState[joystick].touch.fPosX;
	//f32 oX = oldState[joystick].touch.fPosX;
	//return (nX - oX) / w;

	return curState[joystick].touch.fRelX / w;
}

f32 Input::GetRelativeY(u16 joystick) const
{
	f32 h = pScreen->GetHeight();

	//f32 nY = curState[joystick].touch.fPosY;
	//f32 oY = oldState[joystick].touch.fPosY;
	//return (nY - oY) / h;

	return curState[joystick].touch.fRelY / h;
}

f32 Input::GetOrientation(u16 joystick) const
{
	UNUSED(joystick);
	return 1;
}

f32 Input::GetDistance(u16 joystick) const
{
	UNUSED(joystick);
	return 1;
}

void Input::SendEvents()
{
	for (u16 i = 0; i < PLATFORM_MAX_INPUT; i++)
	{
		EventInputPointer ev(i,
							 ConvertButtonFlags(curState[i].iTrig),
							 ConvertButtonFlags(curState[i].iHold),
							 ConvertButtonFlags(curState[i].iRelease),
							 this->GetX(i),
							 this->GetY(i));

		if (curState[i].iTrig)
		{
			this->SendEventPointerPress(&ev);
		}

		if (curState[i].iRelease)
		{
			this->SendEventPointerRelease(&ev);
		}

		if (curState[i].touch.fPosX != oldState[i].touch.fPosX || curState[i].touch.fPosY != oldState[i].touch.fPosY)
		{
			this->SendEventPointerMove(&ev);
		}

		curState[i].iRelease = 0;
		curState[i].iTrig = 0;
	}
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
	return 1;
}

void Input::SetSensitivity(u32 sens, u16 joystick)
{
	UNUSED(joystick);
	UNUSED(sens);
}

Seed::eInputButton Input::GetButtonCode(u32 button) const
{
	Seed::eInputButton btn = Seed::ButtonInvalid;
	switch (button)
	{
		case 1:
			btn = Seed::Button0;
		break;

		case 2:
			btn = Seed::Button1;
		break;
	}

	return btn;
}

u32 Input::ConvertButtonFlags(u32 flags)
{
	u32 iFlags = 0;

	iFlags = (this->GetButtonCode(flags));

	return iFlags;
}

}} // namespace

#endif // BUILD_IOS
