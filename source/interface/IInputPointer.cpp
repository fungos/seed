/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

#include "interface/IInputPointer.h"
#include "interface/IEventInputPointerListener.h"
#include "Log.h"
#include "Array.h"

#include <vector>
#include <algorithm>

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
	ASSERT_NULL(ev);

	ForEach(IEventInputPointerListenerVector, vPointerListeners,
	{
		(*it)->OnInputPointerEnable(ev);
	});
}

void IInputPointer::SendEventPointerDisable(const EventInputPointer *ev)
{
	ASSERT_NULL(ev);

	ForEach(IEventInputPointerListenerVector, vPointerListeners,
	{
		(*it)->OnInputPointerDisable(ev);
	});
}

void IInputPointer::SendEventPointerMove(const EventInputPointer *ev)
{
	ASSERT_NULL(ev);

	ForEach(IEventInputPointerListenerVector, vPointerListeners,
	{
		(*it)->OnInputPointerMove(ev);
	});
}

void IInputPointer::SendEventPointerPress(const EventInputPointer *ev)
{
	ASSERT_NULL(ev);

	ForEach(IEventInputPointerListenerVector, vPointerListeners,
	{
		(*it)->OnInputPointerPress(ev);
	});
}

void IInputPointer::SendEventPointerRelease(const EventInputPointer *ev)
{
	ASSERT_NULL(ev);

	ForEach(IEventInputPointerListenerVector, vPointerListeners,
	{
		(*it)->OnInputPointerRelease(ev);
	});
}

} // namespace
