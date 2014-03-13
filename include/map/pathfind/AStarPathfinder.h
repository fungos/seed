#ifndef __ASTARPATHFINDER_H__
#define __ASTARPATHFINDER_H__

#include "Container.h"
#include "map/pathfind/IPathfinder.h"
#include "map/TileSet.h"

namespace Seed {

class SEED_CORE_API AStarPathfinder : public IPathfinder
{
	SEED_DISABLE_COPY(AStarPathfinder)
	SEED_DECLARE_CONTAINER(Vector, TileNode)

	public:
		AStarPathfinder(bool isDiagonalAllowed, bool isCornerCrossable, u32 weight,
						MapLayerTiled *mapBackground,
						MapLayerTiled *mapColliders);
		virtual ~AStarPathfinder();
		bool CheckOpenNeighborByTileId(const u32 tileId);
		bool CheckCloseNeighborByTileId(const u32 tileId);

		// IPathfinder
		virtual Path &FindPath(const Vector3f &start, const Vector3f &end, Path &path) override;

	protected:
		void Destroy();

	private:
		TileNodeVector	vOpen;
		TileNodeVector	vClose;
		TileNode *pStartNode;
};

}// end namespace

#endif // __ASTARPATHFINDER_H__
