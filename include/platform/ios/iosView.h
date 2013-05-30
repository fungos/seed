#if defined(BUILD_IOS)

#include "Defines.h"
#include "interface/IGameApp.h"

#include <UIKit/UIKit.h>
#include <GLKit/GLKit.h>

const char *iosGetRootPath();
const char *iosGetHomePath();

namespace Seed {
	void SetGameApp(IGameApp *app);
} // namespace


@interface ViewController : GLKViewController
@end


@interface AppDelegate : UIResponder <UIApplicationDelegate>
@property (nonatomic, retain) UIWindow* window;
@property (nonatomic, retain) ViewController* viewController;
@end

#endif // BUILD_IOS