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

#ifndef __RECT_H__
#define __RECT_H__

#include "Log.h"

namespace Seed {

/// Rectangle Template
template <class TYPE> class Rect
{
	public:
		TYPE x1;
		TYPE y1;
		TYPE x2;
		TYPE y2;

	public:
		Rect(TYPE mx = 0, TYPE my = 0, TYPE mwidth = 0, TYPE mheight = 0)
			: x1(mx)
			, y1(my)
			, x2(mwidth + mx)
			, y2(mheight + my)
		{
		}

		Rect(const Rect<TYPE> &rect)
			: x1(0)
			, y1(0)
			, x2(0)
			, y2(0)
		{
			LoadRect(rect);
		}

		Rect<TYPE> &operator=(const Rect<TYPE> &rect)
		{
			x1	= rect.x1;
			y1	= rect.y1;
			x2	= rect.x2;
			y2	= rect.y2;

			return *this;
		}

		inline TYPE Width() const
		{
			return (x2 - x1);
		}

		inline TYPE Height() const
		{
			return (y2 - y1);
		}

		inline bool operator==(const Rect<TYPE> &rect) const
		{
			return ((x1 == rect.x1) && (y1 == rect.y1) && (x2 == rect.x2) && (y2 == rect.y2));
		}

		inline bool operator!=(const Rect<TYPE> &rect) const
		{
			return (!(x1 == rect.x1) || !(y1 == rect.y1) || !(x2 == rect.x2) || !(y2 == rect.y2));
		}

		inline Rect<TYPE> & LoadRect(const Rect<TYPE> &rect)
		{
			*this = rect;
			return *this;
		}

		inline bool IsCollidedWith(const Rect<TYPE> &rect) const
		{
			TYPE left1, left2;
			TYPE right1, right2;
			TYPE top1, top2;
			TYPE bottom1, bottom2;

			left1   = this->x1;
			left2   = rect.x1;
			right1  = this->x2;
			right2  = rect.x2;
			top1    = this->y;
			top2    = rect.y;
			bottom1 = this->y1;
			bottom2 = rect.y2;

			if (bottom1 < top2) return(false);
			if (top1 > bottom2) return(false);

			if (right1 < left2) return(false);
			if (left1 > right2) return(false);

			return true;
		}

		inline bool GetOverlappedRect(const Rect<TYPE> &rect, Rect<TYPE> &rectDst) const
		{
			TYPE left1, left2;
			TYPE right1, right2;
			TYPE top1, top2;
			TYPE bottom1, bottom2;
			TYPE over_bottom;

			TYPE over_top;
			TYPE over_left;
			TYPE over_right;

			left1   = this->x1;
			left2   = rect.x1;
			right1  = this->x2;
			right2  = rect.x2;
			top1    = this->y1;
			top2    = rect.y1;
			bottom1 = this->y2;
			bottom2 = rect.y2;

			// Ok, compute the rectangle of overlap:
			if (bottom1 > bottom2) over_bottom = bottom2;
			else over_bottom = bottom1;

			if (top1 < top2) over_top = top2;
			else over_top = top1;

			if (right1 > right2) over_right = right2;
			else over_right = right1;

			if (left1 < left2) over_left = left2;
			else over_left = left1;

			rectDst.x      = over_left;
			rectDst.y      = over_top;
			rectDst.width  = (TYPE)(over_right - over_left);
			rectDst.height = (TYPE)(over_bottom - over_top);

			if (bottom1 < top2) return(false);
			if (top1 > bottom2) return(false);

			if (right1 < left2) return(false);
			if (left1 > right2) return(false);

			return true;
		}

		inline bool Contains(TYPE pX, TYPE pY) const
		{
			if (pX > x2)
			{
				return false;
			}
			else if	(pX < x1)
			{
				return false;
			}
			if (pY > y2)
			{
				return false;
			}
			else if	(pY < y1)
			{
				return false;
			}

			return true;
		}

		inline bool Intersect(const Rect<TYPE> &rect) const
		{
			if (this->Contains(rect.x1, rect.y1) ||
				this->Contains(rect.x2, rect.y2))
				return true;

			return false;
		}

		inline bool Intersect(f32 x, f32 y, f32 radius) const
		{
			f32 cx = x;
			CLAMP(cx, x1, x2);
			f32 cy = y;
			CLAMP(cy, y1, y2);

			f32 dx = x - cx;
			f32 dy = y - cy;

			return (dx * dx + dy *  dy) < (radius * radius);
		}

		inline bool ContainsArea(const Rect<TYPE> &rect) const
		{
			if (this->Contains(rect.x1, rect.y1) &&
				this->Contains(rect.x2, rect.y2))
				return true;

			return false;
		}

		inline void Encapsulate(TYPE px, TYPE py)
		{
			if (px < x1) x1 = px;
			if (py < y1) y1 = py;
			if (px > x2) x2 = px;
			if (py > y2) y2 = py;
		}

		inline TYPE CircleRadius() const
		{
			TYPE x = x2 - x1;
			TYPE y = y2 - y1;
			return static_cast<TYPE>(sqrt(x * x + y * y) / 2.0f); // sqrtl
		}

		void Print()
		{
			Log("Rect info: x1 -> %d, y1 -> %d, x2 -> %d, y2 -> %d", x1, y1, x2, y2);
		}
};

typedef Rect<f32> Rect4f;
typedef Rect<double> Rect4d;
typedef Rect<u32> Rect4u;
typedef Rect<s32> Rect4i;


} // namespace

#endif // __RECT_H__
