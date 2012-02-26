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

#include "interface/IInputMotion.h"
#include "interface/IEventInputMotionListener.h"
#include "Log.h"
#include "Array.h"

#include <vector>
#include <algorithm>

namespace Seed {

IInputMotion::IInputMotion()
	: vMotionListeners()
{
}

IInputMotion::~IInputMotion()
{
	IEventInputMotionListenerVector().swap(vMotionListeners);
}

Vector3f IInputMotion::GetAccelerationAxis(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return Vector3f(0.0f, 0.0f, 0.0f);
}

f32 IInputMotion::GetAccelerationMagnitude(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

f32 IInputMotion::GetAccelerationSpeed(u16 joystick) const
{
	UNUSED(joystick);
	SEED_ABSTRACT_METHOD;
	return 0.0f;
}

void IInputMotion::AddMotionListener(IEventInputMotionListener *listener)
{
	vMotionListeners += listener;
}

void IInputMotion::RemoveMotionListener(IEventInputMotionListener *listener)
{
	vMotionListeners -= listener;
}

void IInputMotion::SendEventAccelerationChanged(const EventInputMotion *ev)
{
	ASSERT_NULL(ev);
	ForEach(IEventInputMotionListenerVector, vMotionListeners,
	{
		(*it)->OnAccelerationChanged(ev);
	});
}

} // namespace
