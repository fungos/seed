#ifndef __IPATHFINDER_H__
#define __IPATHFINDER_H__

#include "../../Defines.h"
#include "../../interface/IObject.h"
#include "map/MapLayerTiled.h"
#include "map/pathfind/Heuristic.h"
#include "Path.h"
#include "MathUtil.h"
#include "Memory.h"

namespace Seed {

struct TileNode
{
	u32			iTile;
	Vector3f	cPos;
	u32			iF; // depth
	u32			iG; // cost
	u32			iH; // heuristic
	TileNode	*parent;
	bool		bIsWalkable;
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
		MapLayerTiled		*pMapColliders;

		TileNodeVector		vNeighbors;
		u32					iWeight;

		bool				bIsDiagonalAllowed;
		bool				bIsCornerCrossable;
};

} // end namespace

#endif // __IPATHFINDER_H__
