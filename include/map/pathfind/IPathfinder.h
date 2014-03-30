#ifndef __IPATHFINDER_H__
#define __IPATHFINDER_H__

#include "../../Defines.h"
#include "../../interface/IObject.h"
#include "map/MapLayerTiled.h"
#include "map/MapLayerMetadata.h"
#include "map/pathfind/Heuristic.h"
#include "Path.h"
#include "Memory.h"

namespace Seed {

struct TileNode
{
	vec3		cPos;
	vec3		cDir;
	f32			uF{0}; // depth
	f32			uG{0}; // cost
	f32			uH{0}; // heuristic
	TileNode	*parent{nullptr};
};

class SEED_CORE_API IPathfinder
{
	SEED_DISABLE_COPY(IPathfinder)
	SEED_DECLARE_CONTAINER(Vector, TileNode)

	public:
		IPathfinder();
		virtual ~IPathfinder();

		// Common Operations
		virtual Path &FindPath(const vec3 &start, const vec3 &end, Path &path) = 0;
		void GetNeigboursAtTile(const TileNode &tile);

	protected:
		MapLayerTiled		*pMapBackground;
		TileNodeVector		vNeighbors;
		u32					iWeight;
		u32					iColliderTileId;
		bool				bIsDiagonalAllowed;
		bool				bIsCornerCrossable;
};

} // end namespace

#endif // __IPATHFINDER_H__
