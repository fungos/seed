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

namespace Seed {
	namespace iOS {}
	namespace PC {}
	namespace OGL {}
};

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

typedef float 					fixed32;
typedef int 					PIXEL;

#if SEED_PATH_WIDE == 1
typedef wchar_t					FilePath;
#define PATHCOPY				wcsncpy
#define PATHCAT					wcsncat
#define PATHLEN					wcslen
#else
typedef char					FilePath;
#define PATHCOPY				strncpy
#define PATHCAT					strncat
#define PATHLEN					strlen
#endif

#define LIB_SIZE_T				size_t

// Memory amd Stings
#define LIB_MEMSET(ptr, v, len)		memset(ptr, v, len)
#define LIB_MEMSET4(ptr, v, len)	do { int *p = static_cast<int *>(ptr); for (u32 x=0; x<len; x++) p[x]=v; } while (0)
#define LIB_MEMCOPY(dst, src, len)  memcpy(dst, src, len)
#define LIB_STRLEN(ptr)				strlen(ptr)
#define LIB_STRCMP					strcmp
#define LIB_STRLCPY					strlcpy
#define LIB_STRLCAT					strlcat
#define LIB_STRCASECMP				strcasecmp
#define LIB_STRDUP					strdup
#define LIB_RAND					rand()

#define LIB_FOPEN(a, b)				fopen(a, b)

// Math
#define LIB_FAST_DIV(a, b)			(a/b)
#define LIB_FAST_MOD(a, b)			(a%b)
#define LIB_FAST_SQRT(a)			(float)sqrt(a) // FIXME: Double...
#define LIB_FAST_COS(a)				cos(a)
#define LIB_FAST_SIN(a)				sin(a)
#define LIB_FAST_ACOS(a)			acos(a)
#define LIB_FAST_ASIN(a)			asin(a)
#define LIB_ABS(a)			    	fabs(a)

// Display Object
// iPhone - BGRA
#define LIB_PIXEL_B_MASK			0xff000000
#define LIB_PIXEL_B_SHIFT			24
#define LIB_PIXEL_G_MASK			0x00ff0000
#define LIB_PIXEL_G_SHIFT			16
#define LIB_PIXEL_R_MASK			0x0000ff00
#define LIB_PIXEL_R_SHIFT			8
#define LIB_PIXEL_A_MASK			0x000000ff
#define LIB_PIXEL_A_SHIFT			0
#define LIB_PIXEL_DEFAULT_KEYCOLOR 	0xff00ffff
#define	LIB_PIXEL_COMPONENT(x)		(x&0xff)
#define	LIB_PIXEL_COLOR(r, g, b, a)	((LIB_PIXEL_COMPONENT(b)<<LIB_PIXEL_B_SHIFT) | (LIB_PIXEL_COMPONENT(g)<<LIB_PIXEL_G_SHIFT) | (LIB_PIXEL_COMPONENT(r)<<LIB_PIXEL_R_SHIFT) | (LIB_PIXEL_COMPONENT(a)<<LIB_PIXEL_A_SHIFT))
#define LIB_PIXEL_ALPHA_MAX_VALUE	255

#define _Halt(x)	Log("%s", x); while (1);
#define HALT		do { Log("HALT!"); exit(-1); } while (1);

// Debugging
#define LIB_ASSERT(x)				if (!(x)) { Log("%s:%d", __FILE__, __LINE__); _Halt("Failed assertion " #x); }
#define LIB_ASSERT_MSG(x, msg)		if (!(x)) { Log("%s:%d", __FILE__, __LINE__); _Halt(msg); }
#define LIB_ASSERT_NULL(x)			if ((x) == NULL) { Log("%s:%d", __FILE__, __LINE__); _Halt("Failed NULL assertion on " #x); }

//#define ENABLE_NATIVE_PVRTC_FORMAT		// MUST HAVE SEED_ENABLE_PRELOAD_TEXTURE enabled!

#else // BUILD_IOS
	#error "Include 'Defines.h' instead 'platform/ios/iosDefines.h' directly."
#endif // BUILD_IOS
#endif // __IOS_DEFINES_H__
