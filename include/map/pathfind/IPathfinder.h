#ifndef __IPATHFINDER_H__
#define __IPATHFINDER_H__

#include "../../Defines.h"
#include "../../interface/IObject.h"
#include "map/MapLayerTiled.h"
#include "Path.h"
#include "MathUtil.h"

namespace Seed {

class SEED_CORE_API IPathfinder : public IObject
{
	SEED_DISABLE_COPY(IPathfinder)
	SEED_DECLARE_RTTI(IPathfinder, IObject)

	public:
		IPathfinder();
		virtual ~IPathfinder();

		// Common Operations
		virtual Path &FindPath(const Vector3f &start, const Vector3f &end, MapLayerTiled &map, Path &path) = 0;

	protected:
		f32 Manhattan(const Vector3f &point) const;
		f32 Euclidean(const Vector3f &point) const;
		f32 Chebyshev(const Vector3f &point) const;
};

} // end namespace

#endif // __IPATHFINDER_H__
