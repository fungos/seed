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

#ifndef __IOS_DEFINES_H__
#define __IOS_DEFINES_H__

#if defined(BUILD_IOS)

/* For now we only support OpenGL ES 1.1 */
#if !defined(SEED_ENABLE_OGLES1)
#define SEED_ENABLE_OGLES1
#endif // SEED_ENABLE_OGLES1

#include <string.h>

#define PLATFORM iPhone

// Workaround EAGL.h including Debugging.h and giving error on Release build.
#if defined(RELEASE)
	#define __DEBUGGING__
#endif // RELEASE

#define USE_API_OGL		1
#define BUILD_PC		1

namespace Seed
{
	namespace iOS {}
	namespace PC {}
	namespace OGL {}
}

using namespace Seed::iOS;
using namespace Seed::PC;
using namespace Seed::OGL;

#define PLATFORM_MAX_SCREEN_WIDTH		480
#define PLATFORM_MAX_SCREEN_HEIGHT		320
#define PLATFORM_CODE					2
#define PLATFORM_MAX_INPUT				2
#define PLATFORM_PATH_SEPARATOR			'/'
#define PLATFORM_SAVESYSTEM_SLOTS_MAX	4

// Types
typedef unsigned long long int 	u64;
typedef unsigned int 			u32;
typedef unsigned short int 		u16;
typedef unsigned char 			u8;
typedef signed long long		s64;
typedef signed int 				s32;
typedef signed short int 		s16;
typedef signed char 			s8;
typedef float 					f32;

#define HALT(x)		do { Log("%s", x); exit(-1); } while (1);

//#define ENABLE_NATIVE_PVRTC_FORMAT		// MUST HAVE SEED_ENABLE_PRELOAD_TEXTURE enabled!

#else // BUILD_IOS
	#error "Include 'Defines.h' instead 'platform/ios/iosDefines.h' directly."
#endif // BUILD_IOS
#endif // __IOS_DEFINES_H__
