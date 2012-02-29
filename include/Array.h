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

#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "Log.h"

namespace Seed {

/// Static sized Array template
template <class TYPE, int SIZE> class Array
{
	private:
		TYPE array[SIZE];
		u32 used;

	public:
		Array()
			: array()
			, used(0)
		{}

		inline int ArraySize()
		{
			return SIZE;
		}

		inline u32 Size() const
		{
			return used;
		}

		inline TYPE& operator[](u32 index)
		{
			ASSERT(index < used);
			return array[index];
		}

		inline const TYPE& At(u32 index) const
		{
			ASSERT(index < used);
			return array[index];
		}

		inline void Add(TYPE element)
		{
			ASSERT(used < SIZE);
			array[used++] = element;
		}

		inline void Add(TYPE element, u32 pos)
		{
			ASSERT((used + 1) < SIZE);

			for (s32 i = (s32)used-1; i >= (s32)pos; i--)
			{
				array[i+1] = array[i];
			}

			used++;

			array[pos] = element;
		}

		inline void Add()
		{
			ASSERT(used < SIZE);
			used++;
		}

		inline void Truncate(u32 pos = 0)
		{
			if (pos == 0 && used == 0)
				return;

			ASSERT(pos < used);
			used = pos;
		}

		inline void Del(u32 pos, u32 qtt = 1)
		{
			ASSERT(qtt >= 1 && (pos + qtt) <= used);
			for (u32 i = 0; i < (used - (pos + qtt)); i++)
				array[pos + i] = array[pos + qtt + i];

			used -= qtt;
		}

		inline void Remove(TYPE element)
		{
			ASSERT(element != NULL);

			u32 removed = 0;
			for (u32 i = 0; i < used; i++)
			{
				if (array[i] == element)
				{
					removed++;
					array[i] = NULL;
				}
			}

			u32 firstUsed = 0;
			if (removed)
			{
				u32 moved = 0;
				while (moved != removed)
				{
					u32 emptyPos = 0;
					u32 lastUsed = used - 1;

					for (u32 i = firstUsed; i < used; i++)
					{
						if (array[i] == NULL)
						{
							emptyPos = i;

							if (i)
								firstUsed = i - 1;

							break;
						}
					}

					for (u32 i = lastUsed; i > 0; i--)
					{
						if (array[i])
						{
							lastUsed = i;
							break;
						}
					}

					if (lastUsed > emptyPos)
					{
						array[emptyPos] = array[lastUsed];
						array[lastUsed] = NULL;
					}
					used--;
					moved++;
				}
			}
		}

		inline void Replace(TYPE element, u32 pos, u32 qtt = 1)
		{
			ASSERT(qtt > 0 && (pos + qtt) < used);
			for (u32 i = pos; i < (pos + qtt); i++)
				array[i] = element;
		}
};

} // namespace

#endif // __ARRAY_H__
