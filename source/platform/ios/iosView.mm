/*
 * Copyright (c) 2012, Seed Developers
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. A credit line is required in the game credits, as the following line:
 *    Powered by Seed Framework
 * 5. A Seed Framework logo should be displayed within a splash screen, or in
 *    the case where the game haven't or cannot have a splash, a logo should be
 *    displayed somewhere in the game (eg. menu, credits, other place).
 *
 * THIS SOFTWARE IS PROVIDED BY SEED DEVELOPERS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SEED DEVELOPERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if defined(BUILD_IOS)

#include <CoreFoundation/CoreFoundation.h>
#include <QuartzCore/QuartzCore.h>
#include <OpenGLES/EAGLDrawable.h>

#include "platform/ios/iosView.h"
#include "platform/ios/iosSystemStub.h"
#include "System.h"
#include "SeedInit.h"
#include "Screen.h"
#include "Input.h"
#include "EventInputPointer.h"

using namespace Seed;

#define MAX_PATH_SIZE	1024

iosTouchInfo iosTouchBuff[PLATFORM_MAX_INPUT];

char * _defaultRootPathA[MAX_PATH_SIZE];
char * _defaultHomePathA[MAX_PATH_SIZE];
static int gPlatformIdentifier = 0; // 0 == iPhone, 1 == iPad
static int gOpenGLVersion = 1;

const char *iosGetRootPath()
{
	CFStringRef fileString;
	fileString = (CFStringRef)[[NSBundle mainBundle] resourcePath];
	
	CFStringGetCString(fileString, (char *)_defaultRootPathA, MAX_PATH_SIZE, kCFStringEncodingASCII);
	//fprintf(stdout, "%s", _defaultRootPath);
	
	return (const char *)_defaultRootPathA;
}

const char *iosGetHomePath()
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask,YES);
	NSString *documentsDirectory= [paths objectAtIndex: 0];
	
	CFStringRef fileString;
	fileString = (CFStringRef)documentsDirectory;
	
	CFStringGetCString(fileString, (char *)_defaultHomePathA, MAX_PATH_SIZE, kCFStringEncodingASCII);
	//fprintf(stdout, "%s", _defaultHomePath);
	
	return (const char *)_defaultHomePathA;
}


@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
	if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
	    self.viewController = [[ViewController alloc] initWithNibName:@"ViewController_iPhone" bundle:nil];
		gPlatformIdentifier = 0;
	} else {
	    self.viewController = [[ViewController alloc] initWithNibName:@"ViewController_iPad" bundle:nil];
		gPlatformIdentifier = 1;
	}
	self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
	// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	// Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end



@interface ViewController()
{
}

@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;

@end


@implementation ViewController

- (void)dealloc
{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
	
	[super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	gOpenGLVersion = 2;
    if (!self.context) {
		self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		gOpenGLVersion = 1;
		if (!self.context) {
			NSLog(@"Failed to create ES context");
		}
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
	
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
	
    // Dispose of any resources that can be recreated.
}

- (void)setupGL
{
    [EAGLContext setCurrentContext: self.context];
	Seed::Initialize();
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
	Seed::Shutdown();
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
	Seed::Update();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	Seed::Render();
}

@end

#endif // BUILD_IOS

