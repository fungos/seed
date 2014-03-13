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

class SEED_CORE_API IPathfinder : public IObject
{
	SEED_DISABLE_COPY(IPathfinder)
	SEED_DECLARE_RTTI(IPathfinder, IObject)
	SEED_DECLARE_CONTAINER(Vector, TileNode)

	public:
		IPathfinder();
		virtual ~IPathfinder();

		// Common Operations
		virtual Path &FindPath(const Vector3f &start, const Vector3f &end, Path &path) = 0;
		void GetNeigboursAtTile(TileNode tile);

	protected:
		bool				bIsDiagonalAllowed;
		bool				bIsCornerCrossable;
		u32					iWeight;
		TileNodeVector		vNeighbors;

		Heuristic			*pHeuristic;
		MapLayerTiled		*pMapBackground;
};

} // end namespace

#endif // __IPATHFINDER_H__
