#include "Defines.h"
#include "interface/IGameApp.h"
#include <OpenGLES/EAGL.h>

const char *iphGetRootPath();
const char *iphGetHomePath();

namespace Seed {
	void SetGameApp(IGameApp *app);
} // namespace

