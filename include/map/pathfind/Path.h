#ifndef __PATH_H__
#define __PATH_H__

#include "../../Defines.h"
#include "Container.h"
#include "MathUtil.h"

namespace Seed {

class SEED_CORE_API Path
{
	SEED_DISABLE_COPY(Path)

	public:
		Path();
		virtual ~Path();
		void AppendPositionStep(const Vector3f &vector3f);
		void AppendDirectionStep(const Vector3f &vector3f);
		Stack<Vector3f> &GetPositionSteps();
		Stack<Vector3f> &GetDirectionSteps();

	private:
		Stack<Vector3f> cPositionSteps;
		Stack<Vector3f> cDirectionSteps;
};

} // end namespace

#endif // __PATH_H__
