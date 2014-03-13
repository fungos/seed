#include "map/pathfind/AStarPathfinder.h"
#include <MathUtil.h>

namespace Seed {

class FindNeighborByTileId
{
	public:
		FindNeighborByTileId(const u32 tileId)
			: tileId(tileId)
		{}

		bool operator()(const TileNode *t) const
		{
			return (t->iTile == tileId);
		}

	private:
		u32 tileId;
};

AStarPathfinder::AStarPathfinder(bool isDiagonalAllowed, bool isCornerCrossable
								, u32 weight, MapLayerTiled *mapBackground
								, MapLayerTiled *mapColliders)
	: pStartNode(nullptr)
{
	bIsDiagonalAllowed = isDiagonalAllowed;
	bIsCornerCrossable = isCornerCrossable;
	iWeight = weight;
	pMapBackground = mapBackground;
	pMapColliders = mapColliders;
}

AStarPathfinder::~AStarPathfinder()
{
	this->Destroy();
}

void AStarPathfinder::Destroy()
{
	for (auto obj : vOpen)
		sdDelete(obj);

	for (auto obj : vClose)
		sdDelete(obj);

	TileNodeVector().swap(vOpen);
	TileNodeVector().swap(vClose);

	pStartNode = nullptr;
}

Path &AStarPathfinder::FindPath(const Vector3f &start, const Vector3f &end, Path &path)
{
	auto startNode = pMapBackground->GetTileAt(start);
	auto endNode = pMapBackground->GetTileAt(end);

	// The initial node must be zero for g and f
	pStartNode = sdNew(TileNode);
	pStartNode->iTile = startNode;
	pStartNode->cPos = start;
	pStartNode->iG = 0;
	pStartNode->iF = 0;
	pStartNode->bIsWalkable = true;

	// Push the start node into the open list
	vOpen.push_back(pStartNode);

	// While the open list is not empty
	while (!vOpen.empty())
	{
		// Get the current tilenode (the minimum 'f' value)
		auto current = vOpen.front();

		// Remove from open list and add to the close list
		vOpen.erase(vOpen.begin());
		vClose.push_back(current);

		// If reached the end position, construct the path and return it
		if (current->iTile == endNode)
		{
			auto step = current;

			// Push each tile pos into the steps stack
			while (step->iTile != startNode)
			{
				path.AppendStep(step->cPos);
				step = step->parent;
			}
			path.AppendStep(start);

			return path;
		}

		// Get neigbours of the current node
		this->GetNeigboursAtTile(*current);

		for (u32 i = 0; i < vNeighbors.size(); ++i)
		{
			auto neighbor = vNeighbors.at(i);

			if (this->CheckCloseNeighborByTileId(neighbor->iTile))
				continue;

			// Get the distance between current node and the neighbor
			// and calculate the next g score
			u32 ng = current->iG + ((neighbor->cPos.getX() - current->cPos.getX() == 0
									|| neighbor->cPos.getY() - current->cPos.getY() == 0)
									? 1
									: kSqrt2);

			// Check if the neighbor has not been inspected yet, or
			// can be reached with smaller cost from the current node
			if (this->CheckOpenNeighborByTileId(neighbor->iTile) || ng < neighbor->iG)
			{
				neighbor->iG = ng;
				neighbor->iH = neighbor->iH
						|| iWeight * Heuristic::Manhattan(
							abs(neighbor->cPos.getX() - end.getX()),
							abs(neighbor->cPos.getY() - end.getY()));
				neighbor->iF = neighbor->iG + neighbor->iH;
				neighbor->parent = current;

				if (this->CheckOpenNeighborByTileId(neighbor->iTile))
				{
					vOpen.push_back(neighbor);
				}
				else
				{
					// the neighbor can be reached with smaller cost.
					// Since its f value has been updated, we have to
					// update its position in the open list
				}
			}
		}
	}

	// Fail to find the path
	return path;
}

bool AStarPathfinder::CheckOpenNeighborByTileId(const u32 tileId)
{
	TileNodeVectorIterator it = std::find_if(vOpen.begin(), vOpen.end(), FindNeighborByTileId(tileId));
	return (it != vOpen.end());
}

bool AStarPathfinder::CheckCloseNeighborByTileId(const u32 tileId)
{
	TileNodeVectorIterator it = std::find_if(vClose.begin(), vClose.end(), FindNeighborByTileId(tileId));
	return (it != vClose.end());
}

}
