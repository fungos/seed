#include "map/pathfind/IPathfinder.h"

namespace Seed {

const vec3 vectorUp		{0, -1, 0};	// ↑
const vec3 vectorRight	{1, 0, 0};	// →
const vec3 vectorDown	{0, 1, 0};	// ↓
const vec3 vectorLeft	{-1, 0, 0};	// ←

const vec3 vectorUpLeft		= vectorUp + vectorLeft;	// ↖
const vec3 vectorUpRight	= vectorUp + vectorRight;	// ↗
const vec3 vectorDownRight	= vectorDown + vectorRight;	// ↘
const vec3 vectorDownLeft	= vectorDown + vectorLeft;	// ↙

IPathfinder::IPathfinder()
{
}

IPathfinder::~IPathfinder()
{
}

Path &IPathfinder::FindPath(const vec3 &start, const vec3 &end, Path &path)
{
	UNUSED(start);
	UNUSED(end);
	SEED_ABSTRACT_METHOD;
	return path;
}

void IPathfinder::GetNeigboursAtTile(const TileNode &tile)
{
	vNeighbors.clear();

	auto tileScaleX = iWeight;
	auto tileScaleY = iWeight;

	vec3 seachPos;

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
	seachPos.x = tile.cPos.x;
	seachPos.y = tile.cPos.y - tileScaleY;
	if (pMapBackground-> GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorUp;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		s0 = true;
	}

	// →
	seachPos.x = tile.cPos.x + tileScaleX;
	seachPos.y = tile.cPos.y;
	if (pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorRight;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		s1 = true;
	}

	// ↓
	seachPos.x = tile.cPos.x;
	seachPos.y = tile.cPos.y + tileScaleY;
	if (pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorDown;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		s2 = true;
	}

	// ←
	seachPos.x = tile.cPos.x - tileScaleX;
	seachPos.y = tile.cPos.y;
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
	seachPos.x = tile.cPos.x - tileScaleX;
	seachPos.y = tile.cPos.y - tileScaleY;
	if (d0 && pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorUpLeft;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
	}

	// ↗
	seachPos.x = tile.cPos.x + tileScaleX;
	seachPos.y = tile.cPos.y - tileScaleY;
	if (d1 && pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorUpRight;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
	}

	// ↘
	seachPos.x = tile.cPos.x + tileScaleX;
	seachPos.y = tile.cPos.y + tileScaleY;
	if (d2 && pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorDownRight;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
	}

	// ↙
	seachPos.x = tile.cPos.x - tileScaleX;
	seachPos.y = tile.cPos.y + tileScaleY;
	if (d3 && pMapBackground->GetTileAt(seachPos) != iColliderTileId)
	{
		auto tileNode = sdNew(TileNode);
		tileNode->cDir = vectorLeft;
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
	}
}

} // namespace
