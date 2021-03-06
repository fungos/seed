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

	public:
		AStarPathfinder(bool isDiagonalAllowed, bool isCornerCrossable, u32 weight, u32 colliderTileId, MapLayerTiled *mapBackground);

		virtual ~AStarPathfinder();
		bool CheckOpenNeighborByTilePos(const vec3 &pos);
		bool CheckCloseNeighborByTilePos(const vec3 &pos);

		// IPathfinder
		virtual Path &FindPath(const vec3 &start, const vec3 &end, Path &path) override;

	protected:
		void Destroy();

	private:
		TileNodeVector			vOpen;
		TileNodeVector			vClose;
		TileNode				*pStartNode;
};

/// TileNode ascending predicate
struct SEED_CORE_API TileNodeAscendingPrioritySort
{
	bool operator()(TileNode * const &left, TileNode * const &right)
	{
		return (left->uF < right->uF);
	}
};

// Find TileNode by position
class SEED_CORE_API FindNeighborByTilePos
{
	public:
		FindNeighborByTilePos(const vec3 &pos)
			: cPos(pos)
		{}

		bool operator()(const TileNode *t) const
		{
			return (ceil(t->cPos.x) == ceil(cPos.x) && ceil(t->cPos.y) == ceil(cPos.y));
		}

	private:
		vec3 cPos;
};

}// end namespace

#endif // __ASTARPATHFINDER_H__
