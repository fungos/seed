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

#include "interface/IInputJoystick.h"
#include "interface/IEventInputJoystickListener.h"
#include "Log.h"

namespace Seed {

IInputJoystick::IInputJoystick()
	: vJoystickListeners()
{
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
	ASSERT_NULL(ev);

	ForEach(IEventInputJoystickListenerVector, vJoystickListeners,
	{
		(*it)->OnInputJoystickButtonPress(ev);
	});
}

void IInputJoystick::SendEventJoystickButtonRelease(const EventInputJoystick *ev)
{
	ASSERT_NULL(ev);

	ForEach(IEventInputJoystickListenerVector, vJoystickListeners,
	{
		(*it)->OnInputJoystickButtonRelease(ev);
	});
}

void IInputJoystick::SendEventJoystickDPadMove(const EventInputJoystick *ev)
{
	ASSERT_NULL(ev);

	ForEach(IEventInputJoystickListenerVector, vJoystickListeners,
	{
		(*it)->OnInputJoystickDPadMove(ev);
	});
}

void IInputJoystick::SendEventJoystickAxisMove(const EventInputJoystick *ev)
{
	ASSERT_NULL(ev);

	ForEach(IEventInputJoystickListenerVector, vJoystickListeners,
	{
		(*it)->OnInputJoystickAxisMove(ev);
	});
}

} // namespace
