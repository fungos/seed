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
	vNeighbors.clear();

	u32 tileScaleX = 40;//pMapBackground->GetScaleX();
	u32 tileScaleY = 40;//pMapBackground->GetScaleY();
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
	seachPos.setX(tile.cPos.getX()); seachPos.setY(tile.cPos.getY() - tileScaleY);
	if (pMapBackground-> GetTileAt(seachPos) != 3)
	{
		TileNode *tileNode = sdNew(TileNode);
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		s0 = true;
		Log("↑: x:%f y:%f", seachPos.getX(), seachPos.getY());
	}

	// →
	seachPos.setX(tile.cPos.getX() + tileScaleX); seachPos.setY(tile.cPos.getY());
	if (pMapBackground->GetTileAt(seachPos) != 3)
	{
		TileNode *tileNode = sdNew(TileNode);
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		s1 = true;
		Log("→: x:%f y:%f", seachPos.getX(), seachPos.getY());
	}

	// ↓
	seachPos.setX(tile.cPos.getX()); seachPos.setY(tile.cPos.getY() + tileScaleY);
	if (pMapBackground->GetTileAt(seachPos) != 3)
	{
		TileNode *tileNode = sdNew(TileNode);
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		s2 = true;
		Log("↓: x:%f y:%f", seachPos.getX(), seachPos.getY());
	}

	// ←
	seachPos.setX(tile.cPos.getX() - tileScaleX); seachPos.setY(tile.cPos.getY());
	if (pMapBackground->GetTileAt(seachPos) != 3)
	{
		TileNode *tileNode = sdNew(TileNode);
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		s3 = true;
		Log("←: x:%f y:%f", seachPos.getX(), seachPos.getY());
	}


	if (!bIsDiagonalAllowed)
	{
		return;
	}

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
	seachPos.setX(tile.cPos.getX() - tileScaleX); seachPos.setY(tile.cPos.getY() - tileScaleY);
	if (d0 && pMapBackground->GetTileAt(seachPos) != 3)
	{
		TileNode *tileNode = sdNew(TileNode);
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		Log("↖: x:%f y:%f", seachPos.getX(), seachPos.getY());
	}

	// ↗
	seachPos.setX(tile.cPos.getX() + tileScaleX); seachPos.setY(tile.cPos.getY() - tileScaleY);
	if (d1 && pMapBackground->GetTileAt(seachPos) != 3)
	{
		TileNode *tileNode = sdNew(TileNode);
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		Log("↗: x:%f y:%f", seachPos.getX(), seachPos.getY());
	}

	// ↘
	seachPos.setX(tile.cPos.getX() + tileScaleX); seachPos.setY(tile.cPos.getY() + tileScaleY);
	if (d2 && pMapBackground->GetTileAt(seachPos) != 3)
	{
		TileNode *tileNode = sdNew(TileNode);
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		Log("↘: x:%f y:%f", seachPos.getX(), seachPos.getY());
	}

	// ↙
	seachPos.setX(tile.cPos.getX() - tileScaleX); seachPos.setY(tile.cPos.getY() + tileScaleY);
	if (d3 && pMapBackground->GetTileAt(seachPos) != 3)
	{
		TileNode *tileNode = sdNew(TileNode);
		tileNode->cPos = seachPos;

		vNeighbors.push_back(tileNode);
		Log("↙: x:%f y:%f", seachPos.getX(), seachPos.getY());
	}
}

} // namespace
