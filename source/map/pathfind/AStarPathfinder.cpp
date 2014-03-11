#include "map/pathfind/AStarPathfinder.h"

namespace Seed {

AStarPathfinder::AStarPathfinder()
{
}

AStarPathfinder::~AStarPathfinder()
{
}

Path &AStarPathfinder::FindPath(const Vector3f &start, const Vector3f &end, MapLayerTiled &map, Path &path)
{
	iStartNode = map.GetTileAt(start);
	iEndNode = map.GetTileAt(end);

	// push the start node into the open list
	sOpen.push(iStartNode);

	// while the open list is not empty
	while(!sOpen.empty())
	{
		// pop the position of tile which has the minimum `f` value.
		//u32 tile = sOpen.pop();

		// if reached the end position, construct the path and return it
		/*if(tile == iEndNode)
		{
			return path;
		}*/

		// get neigbours of the current node
		//map.
	}

	return path;
}

}
