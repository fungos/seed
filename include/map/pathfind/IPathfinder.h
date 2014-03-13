#ifndef __IPATHFINDER_H__
#define __IPATHFINDER_H__

#include "../../Defines.h"
#include "../../interface/IObject.h"
#include "map/MapLayerTiled.h"
#include "map/MapLayerMetadata.h"
#include "map/pathfind/Heuristic.h"
#include "Path.h"
#include "MathUtil.h"
#include "Memory.h"

namespace Seed {

struct TileNode
{
	Vector3f	cPos;
	u32			iF{0}; // depth
	u32			iG{0}; // cost
	u32			iH{0}; // heuristic
	TileNode	*parent{nullptr};

	bool operator<(const TileNode& other) const
	{
		return iF < other.iF;
	}
};

class SEED_CORE_API IPathfinder
{
	SEED_DISABLE_COPY(IPathfinder)
	SEED_DECLARE_CONTAINER(Vector, TileNode)

	public:
		IPathfinder();
		virtual ~IPathfinder();

		// Common Operations
		virtual Path &FindPath(const Vector3f &start, const Vector3f &end, Path &path) = 0;
		void GetNeigboursAtTile(TileNode tile);

	protected:
		MapLayerTiled		*pMapBackground;
		TileNodeVector		vNeighbors;
		u32					iWeight;
		bool				bIsDiagonalAllowed;
		bool				bIsCornerCrossable;
};

} // end namespace

#endif // __IPATHFINDER_H__
