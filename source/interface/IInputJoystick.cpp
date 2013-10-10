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

#include "interface/IInputJoystick.h"
#include "interface/IEventInputJoystickListener.h"
#include "Log.h"
#include <iostream>

namespace Seed {

IInputJoystick::IInputJoystick()
	: vJoystickListeners()
{
#if defined(DEBUG)
	buttonsName[0] = "None";
	buttonsName[1] = "Button0";
	buttonsName[2] = "Button1";
	buttonsName[4] = "Button2";
	buttonsName[8] = "Button3";
	buttonsName[16] = "JoystickUp";
	buttonsName[32] = "JoystickRight";
	buttonsName[64] = "JoystickDown";
	buttonsName[128] = "JoystickLeft";
	buttonsName[256] = "Button8";
	buttonsName[512] = "Button9";
	buttonsName[1024] = "Button10";
	buttonsName[2048] = "Button11";
	buttonsName[4096] = "Button12";
	buttonsName[8192] = "Button13";
	buttonsName[16384] = "Button14";
	buttonsName[32768] = "Button15";
	buttonsName[65536] = "Invalid";
	buttonsName[131072] = "All";
#endif
}

IInputJoystick::~IInputJoystick()
{
	IEventInputJoystickListenerVector().swap(vJoystickListeners);
}

u32 IInputJoystick::GetMaximumJoysticks() const
{
	SEED_ABSTRACT_METHOD;
	return 0;
}

void IInputJoystick::AddJoystickListener(IEventInputJoystickListener *listener)
{
	vJoystickListeners += listener;
}

void IInputJoystick::RemoveJoystickListener(IEventInputJoystickListener *listener)
{
	vJoystickListeners -= listener;
}

void IInputJoystick::SendEventJoystickButtonPress(const EventInputJoystick *ev)
{
	SEED_ASSERT(ev);

#if defined(DEBUG)
	Dbg(">>>> Joystick Button Press: %s Value: 0x%04x", buttonsName[static_cast<u32>(ev->GetPressed())].c_str(), static_cast<u32>(ev->GetPressed()));
#endif

	for (auto each: vJoystickListeners)
		each->OnInputJoystickButtonPress(ev);
}

void IInputJoystick::SendEventJoystickButtonRelease(const EventInputJoystick *ev)
{
	SEED_ASSERT(ev);

	for (auto each: vJoystickListeners)
		each->OnInputJoystickButtonRelease(ev);
}

void IInputJoystick::SendEventJoystickDPadMove(const EventInputJoystick *ev)
{
	SEED_ASSERT(ev);

#if defined(DEBUG)
	Dbg(">>>> Joystick Button Dpad Press: %s Value: 0x%04x", buttonsName[static_cast<u32>(ev->GetPressed())].c_str(), static_cast<u32>(ev->GetPressed()));
#endif
	for (auto each: vJoystickListeners)
		each->OnInputJoystickDPadMove(ev);
}

void IInputJoystick::SendEventJoystickAxisMove(const EventInputJoystick *ev)
{
	for (auto each: vJoystickListeners)
		each->OnInputJoystickAxisMove(ev);
}

} // namespace
