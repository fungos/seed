#include "map/pathfind/IPathfinder.h"

namespace Seed {

IPathfinder::IPathfinder()
{
}

IPathfinder::~IPathfinder()
{
}

Path &IPathfinder::FindPath(const Vector3f &start, const Vector3f &end, Path &path)
{
	UNUSED(start);
	UNUSED(end);
	SEED_ABSTRACT_METHOD;
	return path;
}

void IPathfinder::GetNeigboursAtTile(TileNode tile)
{
	auto tileScaleX = pMapBackground->GetScaleX();
	auto tileScaleY = pMapBackground->GetScaleY();
	Vector3f seachPos;

	/**
	 * Get the neighbors of the given node.
	 *
	 *     offsets      diagonalOffsets:
	 *  +---+---+---+    +---+---+---+
	 *  |   | 0 |   |    | 0 |   | 1 |
	 *  +---+---+---+    +---+---+---+
	 *  | 3 |   | 1 |    |   |   |   |
	 *  +---+---+---+    +---+---+---+
	 *  |   | 2 |   |    | 3 |   | 2 |
	 *  +---+---+---+    +---+---+---+
	 *
	 */
	bool	s0 = false, d0 = false,
			s1 = false, d1 = false,
			s2 = false, d2 = false,
			s3 = false, d3 = false;

	// ↑
	seachPos.setX(tile.cPos.getX());
	seachPos.setY(tile.cPos.getY() - tileScaleY);
	if (!pMapColliders->GetTileAt(seachPos))
	{
		auto tileNode = sdNew(TileNode);
		tileNode->iTile = pMapColliders->GetTileAt(seachPos);
		tileNode->cPos = seachPos;
		tileNode->iG = 0;
		tileNode->iF = 0;
		tileNode->bIsWalkable = true;

		vNeighbors.push_back(tileNode);
		s0 = true;
	}

	// →
	seachPos.setX(tile.cPos.getX() + tileScaleX);
	seachPos.setY(tile.cPos.getY());
	if (!pMapColliders->GetTileAt(seachPos))
	{
		auto tileNode = sdNew(TileNode);
		tileNode->iTile = pMapColliders->GetTileAt(seachPos);
		tileNode->cPos = seachPos;
		tileNode->iG = 0;
		tileNode->iF = 0;
		tileNode->bIsWalkable = true;

		vNeighbors.push_back(tileNode);
		s1 = true;
	}

	// ↓
	seachPos.setX(tile.cPos.getX());
	seachPos.setY(tile.cPos.getY() + tileScaleY);
	if (!pMapColliders->GetTileAt(seachPos))
	{
		auto tileNode = sdNew(TileNode);
		tileNode->iTile = pMapColliders->GetTileAt(seachPos);
		tileNode->cPos = seachPos;
		tileNode->iG = 0;
		tileNode->iF = 0;
		tileNode->bIsWalkable = true;

		vNeighbors.push_back(tileNode);
		s2 = true;
	}

	// ←
	seachPos.setX(tile.cPos.getX() - tileScaleX);
	seachPos.setY(tile.cPos.getY());
	if (!pMapColliders->GetTileAt(seachPos))
	{
		auto tileNode = sdNew(TileNode);
		tileNode->iTile = pMapColliders->GetTileAt(seachPos);
		tileNode->cPos = seachPos;
		tileNode->iG = 0;
		tileNode->iF = 0;
		tileNode->bIsWalkable = true;

		vNeighbors.push_back(tileNode);
		s3 = true;
	}

	if (!bIsDiagonalAllowed)
		return;

	if (!bIsCornerCrossable)
	{
		d0 = s3 && s0;
		d1 = s0 && s1;
		d2 = s1 && s2;
		d3 = s2 && s3;
	}
	else
	{
		d0 = s3 || s0;
		d1 = s0 || s1;
		d2 = s1 || s2;
		d3 = s2 || s3;
	}

	// ↖
	seachPos.setX(tile.cPos.getX() - tileScaleX);
	seachPos.setY(tile.cPos.getY() - tileScaleY);
	if (d0 && !pMapColliders->GetTileAt(seachPos))
	{
		auto tileNode = sdNew(TileNode);
		tileNode->iTile = pMapColliders->GetTileAt(seachPos);
		tileNode->cPos = seachPos;
		tileNode->iG = 0;
		tileNode->iF = 0;
		tileNode->bIsWalkable = true;

		vNeighbors.push_back(tileNode);
	}

	// ↗
	seachPos.setX(tile.cPos.getX() + tileScaleX);
	seachPos.setY(tile.cPos.getY() - tileScaleY);
	if (d1 && !pMapColliders->GetTileAt(seachPos))
	{
		auto tileNode = sdNew(TileNode);
		tileNode->iTile = pMapColliders->GetTileAt(seachPos);
		tileNode->cPos = seachPos;
		tileNode->iG = 0;
		tileNode->iF = 0;
		tileNode->bIsWalkable = true;

		vNeighbors.push_back(tileNode);
	}

	// ↘
	seachPos.setX(tile.cPos.getX() + tileScaleX);
	seachPos.setY(tile.cPos.getY() + tileScaleY);
	if (d2 && !pMapColliders->GetTileAt(seachPos))
	{
		auto tileNode = sdNew(TileNode);
		tileNode->iTile = pMapColliders->GetTileAt(seachPos);
		tileNode->cPos = seachPos;
		tileNode->iG = 0;
		tileNode->iF = 0;
		tileNode->bIsWalkable = true;

		vNeighbors.push_back(tileNode);
	}

	// ↙
	seachPos.setX(tile.cPos.getX() - tileScaleX);
	seachPos.setY(tile.cPos.getY() + tileScaleY);
	if (d3 && !pMapColliders->GetTileAt(seachPos))
	{
		auto tileNode = sdNew(TileNode);
		tileNode->iTile = pMapColliders->GetTileAt(seachPos);
		tileNode->cPos = seachPos;
		tileNode->iG = 0;
		tileNode->iF = 0;
		tileNode->bIsWalkable = true;

		vNeighbors.push_back(tileNode);
	}
}

} // namespace
