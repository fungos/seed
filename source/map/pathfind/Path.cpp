#include "map/pathfind/Path.h"

namespace Seed {

Path::Path()
{
}

Path::~Path()
{
}

void Path::AppendPositionStep(const vec3 &vec3)
{
	cPositionSteps.push(vec3);
}

void Path::AppendDirectionStep(const vec3 &vec3)
{
	cDirectionSteps.push(vec3);
}

Stack<vec3> &Path::GetPositionSteps()
{
	return cPositionSteps;
}

Stack<vec3> &Path::GetDirectionSteps()
{
	return cDirectionSteps;
}

} // namespace
