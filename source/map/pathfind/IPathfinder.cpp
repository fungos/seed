#include "map/pathfind/IPathfinder.h"

namespace Seed {

const Vector3f vectorUp		{0, -1, 0};	// ↑
const Vector3f vectorRight	{1, 0, 0};	// →
const Vector3f vectorDown	{0, 1, 0};	// ↓
const Vector3f vectorLeft	{-1, 0, 0};	// ←

const Vector3f vectorUpLeft		= vectorUp + vectorLeft;	// ↖
const Vector3f vectorUpRight	= vectorUp + vectorRight;	// ↗
const Vector3f vectorDownRight	= vectorDown + vectorRight;	// ↘
const Vector3f vectorDownLeft	= vectorDown + vectorLeft;	// ↙

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
	vNeighbors.clear();

	auto tileScaleX = iWeight;
	auto tileScaleY = iWeight;

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
	if (pMapBackground-> GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorUp;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		s0 = true;
	}

	// →
	seachPos.setX(tile.cPos.getX() + tileScaleX);
	seachPos.setY(tile.cPos.getY());
	if (pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorRight;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		s1 = true;
	}

	// ↓
	seachPos.setX(tile.cPos.getX());
	seachPos.setY(tile.cPos.getY() + tileScaleY);
	if (pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorDown;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		s2 = true;
	}

	// ←
	seachPos.setX(tile.cPos.getX() - tileScaleX);
	seachPos.setY(tile.cPos.getY());
	if (pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorLeft;
		tileNode->cPos = seachPos;

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
	if (d0 && pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorUpLeft;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
	}

	// ↗
	seachPos.setX(tile.cPos.getX() + tileScaleX);
	seachPos.setY(tile.cPos.getY() - tileScaleY);
	if (d1 && pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorUpRight;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
	}

	// ↘
	seachPos.setX(tile.cPos.getX() + tileScaleX);
	seachPos.setY(tile.cPos.getY() + tileScaleY);
	if (d2 && pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorDownRight;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
	}

	// ↙
	seachPos.setX(tile.cPos.getX() - tileScaleX);
	seachPos.setY(tile.cPos.getY() + tileScaleY);
	if (d3 && pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorLeft;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
	}
}

} // namespace
