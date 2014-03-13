#include "map/pathfind/AStarPathfinder.h"
#include <MathUtil.h>

namespace Seed {

class FindNeighborByTilePos
{
	public:
		FindNeighborByTilePos(const Vector3f &pos)
			: cPos(pos)
		{}

		bool operator()(const TileNode *t) const
		{
			return (t->cPos.getX() == cPos.getX() && t->cPos.getY() == cPos.getY());
		}

	private:
		Vector3f cPos;
};

AStarPathfinder::AStarPathfinder(bool isDiagonalAllowed, bool isCornerCrossable, u32 weight, MapLayerTiled *mapBackground)
	: pStartNode(nullptr)
{
	bIsDiagonalAllowed = isDiagonalAllowed;
	bIsCornerCrossable = isCornerCrossable;
	iWeight = weight;
	pMapBackground = mapBackground;
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
	// The initial node must be zero for g and f
	pStartNode = sdNew(TileNode);
	pStartNode->cPos = start;

	// Push the start node into the open list
	vOpen.insert(pStartNode);

	u32 i = 0;
	// While the open list is not empty
	while (!vOpen.empty())
	{
		i++;
		// Get the current tilenode (the minimum 'f' value)
		auto current = vOpen.begin();
		Log("\nStep[%i]: x:%f y:%f", i, current->cPos.getX(), current->cPos.getY());

		// Remove from open list and add to the close list
		vOpen.erase(vOpen.begin());
		vClose.push_back(current);

		// If reached the end position, construct the path and return it
		if(current->cPos.getX() == end.getX()
			&& current->cPos.getY() == end.getY())
		{
			auto step = current;

			// Push each tile pos into the steps stack
			while (step->cPos.getX() != start.getX()
					&& step->cPos.getY() != start.getY())
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

			if(this->CheckCloseNeighborByTilePos(neighbor->cPos))
			{
				continue;

			// Get the distance between current node and the neighbor
			// and calculate the next g score
			u32 ng = current->iG + ((neighbor->cPos.getX() - current->cPos.getX() >= 40
									|| neighbor->cPos.getY() - current->cPos.getY() >= 40)
									? 1
									: kSqrt2);

			// Check if the neighbor has not been inspected yet, or
			// can be reached with smaller cost from the current node
			if(!this->CheckOpenNeighborByTilePos(neighbor->cPos) || ng < neighbor->iG)
			{
				neighbor->iG = ng;
				neighbor->iH = neighbor->iH
						|| iWeight * Heuristic::Manhattan(
							abs(neighbor->cPos.getX() - end.getX()),
							abs(neighbor->cPos.getY() - end.getY()));
				neighbor->iF = neighbor->iG + neighbor->iH;
				neighbor->parent = current;

				if(!this->CheckOpenNeighborByTilePos(neighbor->cPos))
				{
					vOpen.insert(neighbor);
				}
				else
				{
					// the neighbor can be reached with smaller cost.
					// Since its f value has been updated, we have to
					// update its position in the open list
					vOpen.erase(vOpen.find(neighbor));
					vOpen.insert(neighbor);
				}
			}
		}
	}

	// Fail to find the path
	return path;
}

bool AStarPathfinder::CheckOpenNeighborByTilePos(const Vector3f &pos)
{
	TileNodeSetIterator it = std::find_if(vOpen.begin(), vOpen.end(), FindNeighborByTilePos(pos));
	return (it != vOpen.end());
}

bool AStarPathfinder::CheckCloseNeighborByTilePos(const Vector3f &pos)
{
	TileNodeVectorIterator it = std::find_if(vClose.begin(), vClose.end(), FindNeighborByTilePos(pos));
	return (it != vClose.end());
}

}
