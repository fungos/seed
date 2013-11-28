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

#ifndef __POINT_H__
#define __POINT_H__

#include <math.h>
#include <stdlib.h>
#include "Defines.h"

namespace Seed {

/// Point Template
template <class TYPE> class Point
{
	public:
		TYPE x;
		TYPE y;

		Point(TYPE fX = 0, TYPE fY = 0)
			: x(fX)
			, y(fY)
		{
		}

		Point(const Point<TYPE> &point)
			: x(0)
			, y(0)
		{
			LoadPoint(point);
		}

		~Point()
		{
		}

		Point<TYPE> &operator=(const Point<TYPE> &point)
		{
			x = point.x;
			y = point.y;

			return *this;
		}

		void operator+=(const Point<TYPE> &point)
		{
			x += point.x;
			y += point.y;
		}

		Point operator+(const Point<TYPE> &point) const
		{
			Point<TYPE> ret(*this);

			ret += point;

			return ret;
		}

		void operator+=(TYPE f)
		{
			x += f;
			y += f;
		}

		Point<TYPE> operator+(TYPE f) const
		{
			Point<TYPE> ret(*this);

			ret += f;

			return ret;
		}

		void operator-=(const Point<TYPE> &point)
		{
			x -= point.x;
			y -= point.y;
		}

		Point<TYPE> operator-(const Point<TYPE> &point) const
		{
			Point<TYPE> ret(*this);

			ret -= point;

			return ret;
		}

		void operator-=(TYPE f)
		{
			x -= f;
			y -= f;
		}

		Point operator-(TYPE f) const
		{
			Point<TYPE> ret(*this);

			ret -= f;

			return ret;
		}

		void operator*=(TYPE f)
		{
			x *= f;
			y *= f;
		}

		Point<TYPE> operator*(TYPE f) const
		{
			Point<TYPE> ret(*this);

			ret *= f;

			return ret;
		}

		void operator*=(const Point<TYPE> &point)
		{
			x *= point.x;
			y *= point.y;
		}

		Point<TYPE> operator*(const Point &point) const
		{
			Point<TYPE> ret(*this);

			ret *= point;

			return ret;
		}

		void operator/=(TYPE f)
		{
			(*this) *= 1/f;
		}

		Point<TYPE> operator/(TYPE f)
		{
			Point<TYPE> ret(*this);

			ret *= 1 / f;

			return ret;
		}

		bool operator==(const Point<TYPE> &point) const
		{
			return ((x == point.x) && (y == point.y));
		}

		bool operator!=(const Point<TYPE> &point) const
		{
			return (!(x == point.x) || !(y == point.y));
		}

		Point& LoadPoint(const Point<TYPE> &point)
		{
			*this = point;
			return *this;
		}

		TYPE Distance(const Point<TYPE> &point)
		{
			*this -= point;
			*this *= *this;
			return FAST_SQRT(x+y);
		}

		TYPE Dot(const Point<TYPE> &point) const
		{
			return ((x * point.x) + (y * point.y));
		}

		Point<TYPE> & Cross()
		{
			TYPE new_x = y;
			TYPE new_y = -x;

			x = new_x;
			y = new_y;

			return *this;
		}

		TYPE SquareLength() const
		{
			return ((x * x) + (y * y));
		}

		TYPE Length() const
		{
			return sqrt(SquareLength());
		}

		Point<TYPE> & Normalize()
		{
			TYPE fLength = Length();

			if (fLength != 0.0f)
			{
				fLength = 1.0f / fLength;

				(*this) *= fLength;
			}

			return *this;
		}

		Point<TYPE> & Lerp(const Point<TYPE> &point, f32 f)
		{
			x += (point.x-x) * f;
			y += (point.y-y) * f;

			return *this;
		}

		TYPE Angle(const Point<TYPE> *p = nullptr) const
		{
			if (p)
			{
				Point<TYPE> s = *this;
				Point<TYPE> t = *p;

				s.Normalize();
				t.Normalize();
				return acosf(s.Dot(t));
			}
			else
				return atan2f(y, x);
		}
};

// Predefined Point2 and Vector2 types
typedef Point<f32> Vector2f;
typedef Point<f32> Point2f;
typedef Point<double> Vector2d;
typedef Point<double> Point2d;
typedef Point<u32> Vector2u;
typedef Point<u32> Point2u;
typedef Point<s32> Vector2i;
typedef Point<s32> Point2i;

} // namespace

#endif // _POINT_H__
