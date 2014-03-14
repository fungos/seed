#include "map/pathfind/Path.h"

namespace Seed {

Path::Path()
{
}

Path::~Path()
{
}

void Path::AppendPositionStep(const Vector3f &vector3f)
{
	cPositionSteps.push(vector3f);
}

void Path::AppendDirectionStep(const Vector3f &vector3f)
{
	cDirectionSteps.push(vector3f);
}

Stack<Vector3f> &Path::GetPositionSteps()
{
	return cPositionSteps;
}

Stack<Vector3f> &Path::GetDirectionSteps()
{
	return cDirectionSteps;
}

} // namespace
