#ifndef __ASTARPATHFINDER_H__
#define __ASTARPATHFINDER_H__

#include "Container.h"
#include "map/pathfind/IPathfinder.h"
#include "map/TileSet.h"
#include <set>

namespace Seed {

class SEED_CORE_API AStarPathfinder : public IPathfinder
{
	SEED_DISABLE_COPY(AStarPathfinder)
	SEED_DECLARE_CONTAINER(Vector, TileNode)

	typedef std::set<TileNode *> TileNodeSet;
	typedef TileNodeSet::iterator TileNodeSetIterator;

	public:
		AStarPathfinder(bool isDiagonalAllowed, bool isCornerCrossable, u32 weight, MapLayerTiled *mapBackground);

		virtual ~AStarPathfinder();
		bool CheckOpenNeighborByTilePos(const Vector3f &pos);
		bool CheckCloseNeighborByTilePos(const Vector3f &pos);

		// IPathfinder
		virtual Path &FindPath(const Vector3f &start, const Vector3f &end, Path &path) override;

	protected:
		void Destroy();

	private:
		TileNodeSet				vOpen;
		TileNodeVector			vClose;
		TileNode				*pStartNode;
};

}// end namespace

#endif // __ASTARPATHFINDER_H__
