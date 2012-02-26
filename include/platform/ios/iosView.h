#include "Defines.h"
#include "interface/IGameApp.h"

const FilePath *iphGetRootPath();
const FilePath *iphGetHomePath();

namespace Seed {
	void SetGameApp(IGameApp *app);
} // namespace

