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

#ifndef __IINPUT_MOTION_H__
#define __IINPUT_MOTION_H__

#include "interface/IInput.h"
#include "Container.h"
#include "MathUtil.h"

namespace Seed {

class EventInputMotion;
class IEventInputMotionListener;

/// Pointer Input Interface
/**
Platforms that support Accelerometer like input must implement this interface.
*/
class SEED_CORE_API IInputMotion
{
	SEED_DECLARE_CONTAINER(Vector, IEventInputMotionListener)
	SEED_DISABLE_COPY(IInputMotion)

	public:
		IInputMotion();
		virtual ~IInputMotion();

		virtual Vector3f GetAccelerationAxis(u16 joystick = 0) const;
		virtual f32 GetAccelerationMagnitude(u16 joystick = 0) const;
		virtual f32 GetAccelerationSpeed(u16 joystick = 0) const;

		void AddMotionListener(IEventInputMotionListener *listener);
		void RemoveMotionListener(IEventInputMotionListener *listener);

	protected:
		void SendEventAccelerationChanged(const EventInputMotion *ev);

	protected:
		IEventInputMotionListenerVector vMotionListeners;
};

} // namespace

#endif // __IINPUT_POINTER_H__
