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

#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <math.h>
#include "Log.h"
#include "MathUtil.h"

namespace Seed {

/// Vector3
template <typename T> class Vector3
{
	public:
		// Constructors
		inline Vector3()
			: x(0)
			, y(0)
			, z(0)
		{
		}

		inline Vector3(T x, T y, T z)
			: x(x)
			, y(y)
			, z(z)
		{
		}

		inline ~Vector3()
		{
		}

		bool operator==(const Vector3<T> &v) const;
		bool operator!=(const Vector3<T> &v) const;
		Vector3<T> operator-() const;

		// Vector operations
		Vector3<T> operator+(const Vector3<T> &v) const;
		Vector3<T> operator-(const Vector3<T> &v) const;
		Vector3<T> operator*(T s) const;
		Vector3<T> operator/(T s) const;

		// Combined assignment operators to conform to C notation convention
		Vector3<T> &operator+=(const Vector3<T> &v);
		Vector3<T> &operator-=(const Vector3<T> &v);
		Vector3<T> &operator*=(T s);
		Vector3<T> &operator/=(T s);

		// Accessor.  This allows to use the vector object like an array of T. For example:
		// Vector3<float> v (...);
		// float f = v[1]; // access to y
		inline operator const T*()
		{
			return v;
		}

		bool IsZero();
		Vector3<T> & Normalize();
		T Distance(const Vector3<T> &v);
		T DistanceSquared(const Vector3<T> &v);
		T Dot(const Vector3<T> &v) const;
		T LengthSquared() const;
		T Length() const;
		Vector3<T> & Lerp(const Vector3<T> &v, f32 f);
		T Angle(const Vector3<T> *v = NULL) const;

		union
		{
			struct
			{
				T x;
				T y;
				T z;
			};

			T v[3];
		};
};

template <typename T>
inline bool Vector3<T>::operator==(const Vector3<T> &v) const
{
	return ((x == v.x) && (y == v.y) && (z == v.z));
}

template <typename T>
inline bool Vector3<T>::operator!=(const Vector3<T> &v) const
{
	return ((x != v.x) || (y != v.y) || (z != v.z));
}

template <typename T>
inline Vector3<T> Vector3<T>::operator-() const
{
	return Vector3<T>(-x, -y, -z);
}

// Vector operations
template <typename T>
inline Vector3<T> Vector3<T>::operator+(const Vector3<T> &v) const
{
	return Vector3<T>(x + v.x, y + v.y, z + v.z);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator-(const Vector3<T> &v) const
{
	return Vector3<T>(x - v.x, y - v.y, z - v.z);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator*(T s) const
{
	return Vector3<T>(x * s, y * s, z * s);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator/(T s) const
{
	ASSERT(s != 0);

	T oneOverS = (T)((T)1.0 / (T)s); // assert for divide by zero
	return Vector3<T>(x * oneOverS, y * oneOverS, z * oneOverS);
}

// Combined assignment operators to conform to C notation convention
template <typename T>
inline Vector3<T> &Vector3<T>::operator+=(const Vector3<T> &v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

template <typename T>
inline Vector3<T> &Vector3<T>::operator-=(const Vector3<T> &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

template <typename T>
inline Vector3<T> &Vector3<T>::operator*=(T s)
{
	x *= s;
	y *= s;
	z *= s;

	return *this;
}

template <typename T>
inline Vector3<T> &Vector3<T>::operator/=(T s)
{
	ASSERT(s != 0);

	T oneOverS = 1.0 / s; // assert divide by zero!
	x *= oneOverS;
	y *= oneOverS;
	z *= oneOverS;

	return *this;
}

// --------------------------------------------------------------------------
// Return true if is zero vector.
// --------------------------------------------------------------------------
template <typename T>
inline bool Vector3<T>::IsZero()
{
		return (x == 0.0) && (y == 0.0) && (z == 0.0);
}

// --------------------------------------------------------------------------
// Set vector length to 1.
// --------------------------------------------------------------------------
template <typename T>
inline Vector3<T> &Vector3<T>::Normalize()
{
	T len = ((x * x) + (y * y) + (z * z));

	if (len > 0.0)
	{
		len = 1.0f / LIB_SQRT(len);
		x *= len;
		y *= len;
		z *= len;
	}

	return *this;
}

template <typename T>
inline T Vector3<T>::Distance(const Vector3<T> &v)
{
	*this -= v;
	*this *= *this;
	return LIB_SQRT(x + y + z);
}

template <typename T>
inline T Vector3<T>::DistanceSquared(const Vector3<T> &v)
{
	*this -= v;
	*this *= *this;
	return (x + y + z);
}

template <typename T>
inline T Vector3<T>::Dot(const Vector3<T> &v) const
{
	return ((x * v.x) + (y * v.y) + (z * v.z));
}

template <typename T>
inline T Vector3<T>::LengthSquared() const
{
	return ((x * x) + (y * y) + (z * z));
}

template <typename T>
inline T Vector3<T>::Length() const
{
	return LIB_SQRT((x * x) + (y * y) + (z * z));
}

template <typename T>
inline Vector3<T> &Vector3<T>::Lerp(const Vector3<T> &v, f32 f)
{
	x += (v.x - x) * f;
	y += (v.y - y) * f;
	z += (v.z - z) * f;

	return *this;
}

template <typename T>
inline T Vector3<T>::Angle(const Vector3<T> *v) const
{
	if (v)
	{
		Vector3<T> s = *this, t = *v;

		s.Normalize();
		t.Normalize();
		return acosf(s.Dot(t));
	}
	else
	{
		return atan2f(y, x);
	}
}

//
// Nonmember Vector3 functions
//
template <typename T>
inline Vector3<T> operator*(T k, Vector3<T> v)
{
	return Vector3<T>(k * v.x, k * v.y, k * v.z);
}

template <typename T> T LengthSquared(const Vector3<T> &v)
{
	return (v.x * v.x) +  (v.y * v.y) +  (v.z * v.z);
}

template <typename T> T Length(const Vector3<T> &v)
{
	return LIB_SQRT((v.x * v.x) +  (v.y * v.y) +  (v.z * v.z));
}

template <typename T> T DotProduct(const Vector3<T> &a, const Vector3<T> &b)
{
	return ((a.x * b.x) +  (a.y * b.y) +  (a.z * b.z));
}

template <typename T> Vector3<T> CrossProduct(const Vector3<T> &a, const Vector3<T> &b)
{
	return Vector3<T>((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
}

template <typename T> Vector3<T> ComputeNormal(const Vector3<T> &p1, const Vector3<T> &p2, const Vector3<T> &p3)
{
	Vector3<T> vec1(p1 - p2);
	Vector3<T> vec2(p1 - p3);

	Vector3<T> result(CrossProduct(vec1, vec2));
	result.Normalize();

	return result;
}

template <typename T> T Distance(const Vector3<T> &a, const Vector3<T> &b)
{
	T dx = a.x - b.x;
	T dy = a.y - b.y;
	T dz = a.z - b.z;

	return LIB_SQRT((dx * dx) + (dy * dy) + (dz * dz));
}

template <typename T> T DistanceSquared(const Vector3<T> &a, const Vector3<T> &b)
{
	T dx = a.x - b.x;
	T dy = a.y - b.y;
	T dz = a.z - b.z;

	return ((dx * dx) + (dy * dy) + (dz * dz));
}

// Predefined Vector3 types
typedef Vector3<f32> Vector3f;
typedef Vector3<double> Vector3d;

// We provide a global constant zero vector
static const Vector3f ZeroVectorf(0.0f, 0.0f, 0.0f);
static const Vector3d ZeroVectord(0.0, 0.0, 0.0);

} // namespace

#endif // __VECTOR3_H__
