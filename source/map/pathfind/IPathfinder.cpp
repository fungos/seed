#include "map/pathfind/IPathfinder.h"

namespace Seed {


IPathfinder::IPathfinder()
{
}

IPathfinder::~IPathfinder()
{
}

Path &IPathfinder::FindPath(const Vector3f &start, const Vector3f &end, MapLayerTiled &map, Path &path)
{
	UNUSED(start);
	UNUSED(end);
	UNUSED(map);
	SEED_ABSTRACT_METHOD;
	return path;
}

f32 IPathfinder::Manhattan(const Vector3f &point) const
{
	return point.getX() + point.getY();
}

f32 IPathfinder::Euclidean(const Vector3f &point) const
{
	return sqrt(point.getX() * point.getX() + point.getY() * point.getY());
}

f32 IPathfinder::Chebyshev(const Vector3f &point) const
{
	return maxElem(point);
}

} // namespace
