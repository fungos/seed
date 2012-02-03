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

/*! \file Vector.h
	\author	Rafael Eduardo Gonchor
	\brief Point and Vector
*/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "MathUtil.h"

/*
#include <math.h>


namespace Seed {

// was fixed32
template <class TYPE> class Vector
{
	public:
		TYPE x;
		TYPE y;
		TYPE z;

		Vector(TYPE fX = 0, TYPE fY = 0, TYPE fZ = 0)
			: x(fX)
			, y(fY)
			, z(fZ)
		{
		}

		Vector(const Vector &vec)
			: x(0)
			, y(0)
			, z(0)
		{
			LoadVector(vec);
		}

		virtual ~Vector()
		{
		}

		Vector<TYPE> &operator=(const Vector<TYPE> &vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;

			return *this;
		}

		void operator+=(const Vector<TYPE> &vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
		}

		Vector operator+(const Vector<TYPE> &vec) const
		{
			Vector<TYPE> ret(*this);

			ret += vec;

			return ret;
		}

		void operator+=(TYPE f)
		{
			x += f;
			y += f;
			z += f;
		}

		Vector operator+(TYPE f) const
		{
			Vector ret(*this);

			ret += f;

			return ret;
		}

		void operator-=(const Vector<TYPE> &vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
		}

		Vector operator-(const Vector<TYPE> &vec) const
		{
			Vector<TYPE> ret(*this);

			ret -= vec;

			return ret;
		}

		void operator-=(TYPE f)
		{
			x -= f;
			y -= f;
			z -= f;
		}

		Vector<TYPE> operator-(TYPE f) const
		{
			Vector<TYPE> ret(*this);

			ret -= f;

			return ret;
		}

		Vector<TYPE> operator-()
		{
			return Vector(-x, -y, -z);
		}

		virtual void operator*=(TYPE f)
		{
			x *= f;
			y *= f;
			z *= f;
		}

		Vector<TYPE> operator*(TYPE f) const
		{
			Vector<TYPE> ret(*this);

			ret *= f;

			return ret;
		}

		virtual void operator*=(const Vector<TYPE> &vec)
		{
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
		}

		Vector operator*(const Vector<TYPE> &vec) const
		{
			Vector<TYPE> ret(*this);

			ret *= vec;

			return ret;
		}

		virtual void operator/=(TYPE f)
		{
			(*this) *= 1 / f;
		}

		Vector<TYPE> operator/(TYPE f)
		{
			Vector<TYPE> ret(*this);

			ret *= 1 / f;

			return ret;
		}

		bool operator==(const Vector<TYPE> &vec) const
		{
			return (((x == vec.x) && (y == vec.y)) || !(z == vec.z));
		}

		bool operator!=(const Vector &vec) const
		{
			return (!(x == vec.x) || !(y == vec.y) || !(z == vec.z));
		}

		Vector<TYPE> & LoadVector(const Vector<TYPE> &vec)
		{
			*this = vec;
			return *this;
		}

		TYPE Distance(const Vector<TYPE> &vec)
		{
			*this -= vec;
			*this *= *this;
			return FAST_SQRT(x+y+z);
		}

		TYPE Dot(const Vector<TYPE> &vec) const
		{
			return ((x * vec.x) + (y * vec.y) + (z * vec.z));
		}

		TYPE SquareLength() const
		{
			return ((x * x) + (y * y) + (z * z));
		}

		TYPE Length() const
		{
			return sqrt(SquareLength());
		}

		Vector<TYPE> & Normalize()
		{
			TYPE fLength = Length();

			if (fLength != 0)
			{
				fLength = 1 / fLength;

				(*this) *= fLength;
			}

			return *this;
		}

		Vector<TYPE> & Lerp(const Vector<TYPE> &vec, f32 f)
		{
			x += (vec.x - x) * f;
			y += (vec.y - y) * f;
			z += (vec.z - z) * f;

			return *this;
		}

		TYPE Angle(const Vector<TYPE> *v = NULL) const
		{
			if (v)
			{
				Vector<TYPE> s = *this, t = *v;

				s.Normalize(); 
				t.Normalize();
				return acosf(s.Dot(t));
			}
			else 
				return atan2f(y, x);
		}

};

} // namespace
*/

#endif // __VECTOR_H__


