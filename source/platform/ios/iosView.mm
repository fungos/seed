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
#include <iostream>

using namespace Seed;

#define MAX_PATH_SIZE	1024

iosTouchInfo iosTouchBuff[PLATFORM_MAX_INPUT];

char * _defaultRootPathA[MAX_PATH_SIZE];
char * _defaultHomePathA[MAX_PATH_SIZE];
static int gPlatformIdentifier = 0; // 0 == iPhone, 1 == iPad
static int gOpenGLVersion = 1;

enum {
	UNIFORM_MODELVIEW_PROJECTION_MATRIX,
	NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// attribute index
enum {
	ATTRIB_VERTEX,
	ATTRIB_COLOR,
	NUM_ATTRIBUTES
};

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

@synthesize window = _window;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
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
@property (strong, nonatomic) GLKBaseEffect *effect;

@end


@implementation ViewController

@synthesize context = _context;
@synthesize effect = _effect;

- (id) initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
	self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
	if (self) {
		// Custom initialization
	}
	return self;
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];

	if ([self isViewLoaded] && ([[self view] window] == nil))
	{
		self.view = nil;

		[self tearDownGL];

		if ([EAGLContext currentContext] == _context)
		{
			[EAGLContext setCurrentContext:nil];
		}
		_context = nil;
	}

	// Dispose of any resources that can be recreated.
}

- (void)setupGL
{
	[EAGLContext setCurrentContext:self.context];
	Seed::Initialize();
}

- (void)tearDownGL
{
	[EAGLContext setCurrentContext:_context];

	Seed::Shutdown();
}

- (void)viewDidLoad
{
	[super viewDidLoad];

#if defined (SEED_ENABLE_OGLES2)
	_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	gOpenGLVersion = 2;

	if (!_context)
	{
		_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		gOpenGLVersion = 1;
	}
#else
	_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
	gOpenGLVersion = 1;
#endif

	if (!_context)
	{
		NSLog(@"Failed to create ES context");
	}

	GLKView *view = (GLKView *)self.view;
	view.context = _context;
	//view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
	view.drawableMultisample = GLKViewDrawableMultisample4X;

	[self setupGL];
}

- (void)viewDidUnload
{
	[super viewDidUnload];

	[self tearDownGL];

	if ([EAGLContext currentContext] == self.context) {
		[EAGLContext setCurrentContext:nil];
	}
	self.context = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - GLKViewDelegate

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	Seed::Render();

	glClearColor(0.30f, 0.74f, 0.20f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLenum err (glGetError());

	while (err != GL_NO_ERROR)
	{
		String error;

		switch (err)
		{
			case GL_INVALID_OPERATION:				error = "INVALID OPERATION";				break;
			case GL_INVALID_ENUM:					error = "INVALID_ENUM";						break;
			case GL_INVALID_VALUE:					error = "INVALID_VALUE";					break;
			case GL_OUT_OF_MEMORY:					error = "OUT_OF_MEMORY";					break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:	error = "INVALID_FRAMEBUFFER_OPERATION";	break;
		}

		std::cerr << "GL_" << error.c_str() << std::endl;
		err = glGetError();
	}
}

#pragma mark - GLKViewControllerDelegate

- (void)update
{
	Seed::Update();
}

@end

#endif // BUILD_IOS
