#ifndef __PATH_H__
#define __PATH_H__

#include "../../Defines.h"
#include "Container.h"
#include <glm/vec3.hpp>

namespace Seed {

class SEED_CORE_API Path
{
	SEED_DISABLE_COPY(Path)

	public:
		Path();
		virtual ~Path();
		void AppendPositionStep(const vec3 &vec3);
		void AppendDirectionStep(const vec3 &vec3);
		Stack<vec3> &GetPositionSteps();
		Stack<vec3> &GetDirectionSteps();

	private:
		Stack<vec3> cPositionSteps;
		Stack<vec3> cDirectionSteps;
};

} // end namespace

#endif // __PATH_H__
