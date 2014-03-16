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

#ifndef __MATH_UTIL_H__
#define __MATH_UTIL_H__

#include "Defines.h"
#include <math.h>
#include <vectormath/cpp/vectormath_aos.h>

using namespace Vectormath::Aos;
typedef Vector4 Vector4f;
typedef Vector3 Vector3f;
typedef Matrix4 Matrix4f;
typedef Quat Quaternion;

#define VectorEquals(a, b)		(a.getX() == b.getX() && a.getY() == b.getY() && a.getZ() == b.getZ())
#define VectorAgg(a, b)			a.setX(a.getX() * b.getX()); a.setY(a.getY() * b.getY()); a.setZ(a.getZ() * b.getZ())

#if defined(_MSC_VER)
#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
#endif

namespace Seed {

// Declare a global constant for pi and a few multiples.
// Math constants
const f32 kPi = 3.14159265358979323846f; // M_PI
const f32 kSqrt2 = 1.4142135623730951f; // SQRT2
const f32 k2Pi = kPi * 2.0f;

const f32 kPiOver2 = kPi / 2.0f; // M_PI_2 1.57079632679489661923f
const f32 kPiOver4 = kPi / 4.0f; // M_PI_4 0.785398163397448309616f
const f32 k1OverPi = 1.0f / kPi; // M_1_PI 0.318309886183790671538f
const f32 k2OverPi = 2.0f / kPi; // M_2_PI 0.636619772367581343076f
const f32 k1Over2Pi = 1.0f / k2Pi;
const f32 kPiOver180 = kPi / 180.0f;
const f32 k180OverPi = 180.0f / kPi;

// "Wrap" an angle in range -pi...pi by adding the correct multiple of 2 pi
template <typename T>
T WrapPi(T theta);

// "Safe" inverse trig functions
template <typename T>
T SafeAcos(T x);

// Set the Euler angle triple to its "canonical" value
template <typename T>
void CanonizeEulerAngles(T &roll, T &pitch, T &yaw);

// Convert between degrees and radians
template <typename T>
inline T DegToRad(T deg)
{
	return deg * kPiOver180;
}

template <typename T> inline T RadToDeg(T rad)
{
	return rad * k180OverPi;
}

// Convert between "field of view" and "zoom".
// The FOV angle is specified in radians.
template <typename T> inline T FovToZoom(T fov)
{
	return 1.0f / LIB_TAN(fov * 0.5f);
}

template <typename T> inline T ZoomToFov(T zoom)
{
	return 2.0f * LIB_ATAN(1.0f / zoom);
}


inline f32 VectorAngle(const Vector3f &a, const Vector3f *b = nullptr)
{
		if (b)
		{
				Vector3f s = a, t = *b;

				s = normalize(s);
				t = normalize(t);
				return acosf(dot(s, t));
		}
		else
		{
				return atan2f(a.getY(), a.getX());
		}
}

// --------------------------------------------------------------------------
// WrapPi
//
// "Wrap" an angle in range -pi...pi by adding the correct multiple
// of 2 pi
// --------------------------------------------------------------------------
template <typename T>
inline T WrapPi(T theta)
{
	theta += kPi;
	theta -= LIB_FLOOR(theta * k1Over2Pi) * k2Pi;
	theta -= kPi;

	return theta;
}

// --------------------------------------------------------------------------
// safeAcos
//
// Same as acos(x), but if x is out of range, it is "clamped" to the nearest
// valid value. The value returned is in range 0...pi, the same as the
// standard C acos() function
// --------------------------------------------------------------------------
template <typename T>
inline T SafeAcos(T x)
{
	// Check limit conditions
	if (x <= -1.0)
		return kPi;

	if (x >= 1.0)
		return 0.0;

	// value is in the domain - use standard C function
	return LIB_ACOS(x);
}

// --------------------------------------------------------------------------
// canonizeEulerAngles
//
// Set the Euler angle triple to its "canonical" value.  This does not change
// the meaning of the Euler angles as a representation of Orientation in 3D,
// but if the angles are for other purposes such as angular velocities, etc,
// then the operation might not be valid.
// --------------------------------------------------------------------------
template <typename T>
inline void CanonizeEulerAngles(T &roll, T &pitch, T &yaw)
{
	// First, wrap pitch in range -pi ... pi
	pitch = WrapPi(pitch);

	// Now, check for "the back side" of the matrix, pitch outside
	// the canonical range of -pi/2 ... pi/2
	if (pitch < -kPiOver2)
	{
		roll += kPi;
		pitch = -kPi - pitch;
		yaw += kPi;
	}
	else if (pitch > kPiOver2)
	{
		roll += kPi;
		pitch = kPi - pitch;
		yaw += kPi;
	}

	// OK, now check for the Gimbal lock case (within a slight
	// tolerance)
	if (LIB_FABS(pitch) > kPiOver2 - 1e-4)
	{
		// We are in gimbal lock.  Assign all rotation
		// about the vertical axis to heading
		yaw += roll;
		roll = 0.0;
	}
	else
	{
		// Not in gimbal lock.  Wrap the bank angle in
		// canonical range
		roll = WrapPi(roll);
	}

	// Wrap heading in canonical range
	yaw = WrapPi(yaw);
}

} // namespace

#endif // __MATH_UTIL_H__
