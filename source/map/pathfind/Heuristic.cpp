#include "map/pathfind/Heuristic.h"

namespace Seed {

Heuristic::Heuristic()
{
}

Heuristic::~Heuristic()
{
}

f32 Heuristic::Manhattan(const u32 x, const u32 y) const
{
	return x + y;
}

f32 Heuristic::Euclidean(const u32 x, const u32 y) const
{
	return sqrt(x * x + y * y);
}

f32 Heuristic::Chebyshev(const u32 x, const u32 y) const
{
	return std::max(x, y);
}

}
