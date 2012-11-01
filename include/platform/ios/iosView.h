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
@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) ViewController *viewController;
@end

#endif // BUILD_IOS