#include "map/pathfind/Path.h"

namespace Seed {

Path::Path()
{
}

Path::~Path()
{
}

void Path::AppendStep(const Vector3f &vector3f)
{
	cSteps.push(vector3f);
}

Stack<Vector3f> &Path::GetSteps()
{
	return cSteps;
}

} // namespace
