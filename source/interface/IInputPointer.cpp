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

#include "interface/IInputPointer.h"
#include "interface/IEventInputPointerListener.h"
#include "Log.h"

namespace Seed {

IInputPointer::IInputPointer()
	: vPointerListeners()
{
}

IInputPointer::~IInputPointer()
{
	IEventInputPointerListenerVector().swap(vPointerListeners);
}

bool IInputPointer::IsHold(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return false;
}

bool IInputPointer::IsPressed(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return false;
}

bool IInputPointer::IsReleased(u32 button, u16 joystick) const
{
	UNUSED(button);
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return false;
}

f32 IInputPointer::GetX(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

f32 IInputPointer::GetY(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

u32 IInputPointer::GetSensitivity(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0;
}

void IInputPointer::SetSensitivity(u32 sens, u16 joystick)
{
	UNUSED(sens);
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
}

u32 IInputPointer::GetMaximumSensitivity(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0;
}

u32 IInputPointer::GetMaximumPointers() const
{
	SEED_ABSTRACT_METHOD;
	return 0;
}

f32 IInputPointer::GetRelativeX(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

f32 IInputPointer::GetRelativeY(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

f32 IInputPointer::GetDistance(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

f32 IInputPointer::GetOrientation(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

bool IInputPointer::IsEnabled(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return false;
}

void IInputPointer::AddPointerListener(IEventInputPointerListener *listener)
{
	vPointerListeners += listener;
}

void IInputPointer::RemovePointerListener(IEventInputPointerListener *listener)
{
	vPointerListeners -= listener;
}

void IInputPointer::SendEventPointerEnable(const EventInputPointer *ev)
{
	SEED_ASSERT(ev);

	ForEach(IEventInputPointerListenerVector, vPointerListeners,
	{
		(*it)->OnInputPointerEnable(ev);
	});
}

void IInputPointer::SendEventPointerDisable(const EventInputPointer *ev)
{
	SEED_ASSERT(ev);

	ForEach(IEventInputPointerListenerVector, vPointerListeners,
	{
		(*it)->OnInputPointerDisable(ev);
	});
}

void IInputPointer::SendEventPointerMove(const EventInputPointer *ev)
{
	SEED_ASSERT(ev);

	ForEach(IEventInputPointerListenerVector, vPointerListeners,
	{
		(*it)->OnInputPointerMove(ev);
	});
}

void IInputPointer::SendEventPointerPress(const EventInputPointer *ev)
{
	SEED_ASSERT(ev);

	ForEach(IEventInputPointerListenerVector, vPointerListeners,
	{
		(*it)->OnInputPointerPress(ev);
	});
}

void IInputPointer::SendEventPointerRelease(const EventInputPointer *ev)
{
	SEED_ASSERT(ev);

	ForEach(IEventInputPointerListenerVector, vPointerListeners,
	{
		(*it)->OnInputPointerRelease(ev);
	});
}

} // namespace
