#ifndef __ASTARPATHFINDER_H__
#define __ASTARPATHFINDER_H__

#include "Container.h"
#include "map/pathfind/IPathfinder.h"
#include "map/TileSet.h"

namespace Seed {

class SEED_CORE_API AStarPathfinder : public IPathfinder
{
	SEED_DISABLE_COPY(AStarPathfinder)
	SEED_DECLARE_RTTI(AStarPathfinder, IPathfinder)

	public:
		AStarPathfinder();
		virtual ~AStarPathfinder();

		// IPathfinder
		virtual Path &FindPath(const Vector3f &start, const Vector3f &end, MapLayerTiled &map, Path &path) override;

	private:
		Stack<u32>	sOpen;
		u32			iStartNode;
		u32			iEndNode;
		f32			fEuristic;

};

}// end namespace

#endif // __ASTARPATHFINDER_H__
