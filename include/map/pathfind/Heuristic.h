#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#include "../../Defines.h"
#include "../../interface/IObject.h"
#include "MathUtil.h"

namespace Seed {

class SEED_CORE_API Heuristic
{
	public:
		static f32 Manhattan(u32 x, u32 y)
		{
			return x + y;
		}

		static f32 SquareEuclidean(u32 x, u32 y)
		{
			return (x * x + y * y);
		}

		static f32 Euclidean(u32 x, u32 y)
		{
			return sqrt(x * x + y * y);
		}

		static f32 Chebyshev(u32 x, u32 y)
		{
			return std::max(x, y);
		}
};

}

#endif // __HEURISTIC_H__
