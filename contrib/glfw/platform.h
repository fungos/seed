#ifndef __glfw_platform_h__
#define __glfw_platform_h__

#include "glew/glew.h"

#if defined(__APPLE__)
	#include "glfw/cocoa/platform.h"
#elif defined(WIN32)
	#include "glfw/win32/platform.h"
#else
	#if defined(__linux)
		#define _GLFW_HAS_GLXGETPROCADDRESS
		#define _GLFW_HAS_PTHREAD
		#define _GLFW_USE_LINUX_JOYSTICKS
	#endif

	#include "glfw/x11/platform.h"
#endif

#endif // __glfw_platform_h__
