#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#include "../../Defines.h"
#include "../../interface/IObject.h"
#include "MathUtil.h"

namespace Seed {

class SEED_CORE_API Heuristic : public IObject
{
	SEED_DISABLE_COPY(Heuristic)
	SEED_DECLARE_RTTI(Heuristic, IObject)

	public:
		Heuristic();
		virtual ~Heuristic();

		f32 Manhattan(const u32 x, const u32 y) const;
		f32 Euclidean(const u32 x, const u32 y) const;
		f32 Chebyshev(const u32 x, const u32 y) const;
};

}

#endif // __HEURISTIC_H__
