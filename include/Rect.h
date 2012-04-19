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
		TYPE x;
		TYPE y;
		TYPE width;
		TYPE height;

	public:
		Rect(TYPE mx = 0, TYPE my = 0, TYPE mwidth = 0, TYPE mheight = 0)
			: x(mx)
			, y(my)
			, width(mwidth)
			, height(mheight)
		{
		}

		Rect(const Rect<TYPE> &rect)
			: x(0)
			, y(0)
			, width(0)
			, height(0)
		{
			LoadRect(rect);
		}

		Rect<TYPE> &operator=(const Rect<TYPE> &rect)
		{
			x		= rect.x;
			y		= rect.y;
			width	= rect.width;
			height	= rect.height;

			return *this;
		}

		bool operator==(const Rect<TYPE> &rect) const
		{
			return((x == rect.x) && (y == rect.y) && (width == rect.width) && (height == rect.height));
		}

		bool operator!=(const Rect<TYPE> &rect) const
		{
			return(!(x == rect.x) || !(y == rect.y) || !(width == rect.width) || !(height == rect.height));
		}

		Rect<TYPE> & LoadRect(const Rect<TYPE> &rect)
		{
			*this = rect;
			return *this;
		}

		bool IsCollidedWith(const Rect<TYPE> &rect) const
		{
			TYPE left1, left2;
			TYPE right1, right2;
			TYPE top1, top2;
			TYPE bottom1, bottom2;

			left1   = this->x;
			left2   = rect.x;
			right1  = this->x + (TYPE)this->width;
			right2  = rect.x + (TYPE)rect.width;
			top1    = this->y;
			top2    = rect.y;
			bottom1 = this->y + (TYPE)this->height;
			bottom2 = rect.y + (TYPE)rect.height;

			if (bottom1 < top2) return(false);
			if (top1 > bottom2) return(false);

			if (right1 < left2) return(false);
			if (left1 > right2) return(false);

			return true;
		}

		bool GetOverlappedRect(const Rect<TYPE> &rect, Rect<TYPE> &rectDst) const
		{
			TYPE left1, left2;
			TYPE right1, right2;
			TYPE top1, top2;
			TYPE bottom1, bottom2;
			TYPE over_bottom;

			TYPE over_top;
			TYPE over_left;
			TYPE over_right;

			left1   = this->x;
			left2   = rect.x;
			right1  = this->x + (TYPE)this->width;
			right2  = rect.x + (TYPE)rect.width;
			top1    = this->y;
			top2    = rect.y;
			bottom1 = this->y + (TYPE)this->height;
			bottom2 = rect.y + (TYPE)rect.height;

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

		bool Contains(TYPE pX, TYPE pY) const
		{
			if (pX > (TYPE)(x + width))
			{
				return false;
			}
			else if	(pX < x)
			{
				return false;
			}
			if (pY > (TYPE)(y + height))
			{
				return false;
			}
			else if	(pY < y)
			{
				return false;
			}

			return true;
		}

		bool ContainsArea(const Rect<TYPE> &rect) const
		{
			if (this->Contains(rect.x, rect.y) &&
				this->Contains(rect.x+rect.width, rect.y+rect.height))
				return true;

			return false;
		}

		void ScaleSize(f32 fScale)
		{
			this->width		*= fScale;
			this->height	*= fScale;
		}

		void ScaleBounds(f32 fScale)
		{
			TYPE newWidth	= width * fScale;
			TYPE newHeight	= height * fScale;

			x -= (newWidth - width)   / 2;
			y -= (newHeight - height) / 2;

			width	= newWidth;
			height	= newHeight;
		}

		// FIXME: testar
		void Encapsulate(TYPE px, TYPE py)
		{
			if (px < x) x = px;
			if (py < y) y = py;
			if (px > x + width) width = px - x;
			if (py > y + height) height = py - y;
		}

		void Print()
		{
			Log("Rect info: x -> %d, y -> %d, width -> %d, height -> %d", x, y, width, height);
		}
};

typedef Rect<f32> Rect4f;
typedef Rect<double> Rect4d;
typedef Rect<u32> Rect4u;
typedef Rect<s32> Rect4i;


} // namespace

#endif // __RECT_H__
