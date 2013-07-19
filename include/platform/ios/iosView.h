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

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow* window;

@end

@interface ViewController : GLKViewController
{
@private
    GLint program;
    
    // The pixel dimensions of the CAEAGLLayer
	GLint backingWidth;
	GLint backingHeight;
    
    // The OpenGL names for the framebuffer and renderbuffer used to render to this view
	GLuint defaultFramebuffer, colorRenderbuffer;
    
   	GLfloat rotz;
}
    - (BOOL)loadShaders;
@end

#endif // BUILD_IOS