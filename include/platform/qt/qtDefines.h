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

#ifndef __QT_DEFINES_H__
#define __QT_DEFINES_H__

#if defined(BUILD_QT)

#include "platform/qt/qtPch.h"

#define _NO_W32_PSEUDO_MODIFIERS
#define USE_API_OGL		1
#define USE_API_OAL		1
#define BUILD_PC		1

namespace Seed { namespace QT {} };
namespace Seed { namespace PC {} };
using namespace Seed::QT;
using namespace Seed::PC;

#if defined(_QT_EMULATE_WII_)
	#define PLATFORM_MAX_SCREEN_WIDTH		640
	#define PLATFORM_MAX_SCREEN_HEIGHT		480
#elif _QT_EMULATE_IPH_
	#define PLATFORM_MAX_SCREEN_WIDTH		480
	#define PLATFORM_MAX_SCREEN_HEIGHT		320
#else
	#define PLATFORM_MAX_SCREEN_WIDTH		1024
	#define PLATFORM_MAX_SCREEN_HEIGHT		768
#endif // BUILD_QTEMULATE_IPH_

#define PLATFORM						QT
#define PLATFORM_CODE					3
#define PLATFORM_MAX_INPUT				1
#define PLATFORM_SAVESYSTEM_SLOTS_MAX	5

// Types
typedef unsigned long long int 	u64;
typedef unsigned int 			u32;
typedef unsigned short int 		u16;
typedef unsigned char 			u8;
typedef signed long long int	s64;
typedef signed int 				s32;
typedef signed short int 		s16;
typedef signed char 			s8;
typedef float 					f32;

typedef float 					fixed32;
typedef int 					PIXEL;
typedef int						bool;

#if SEED_PATH_WIDE == 1
typedef wchar_t					FilePath;
#define PATHCOPY				wcsncpy
#define PATHCAT					wcsncat
#define PATHLEN					wcslen
#define SNPRINTF				swprintf
#else
typedef char					FilePath;
#define PATHCOPY				strncpy
#define PATHCAT					strncat
#define PATHLEN					strlen
#define SNPRINTF				snprintf
#endif

#if defined(WIN32)
	#undef OUT
	#define ATTRIBUTE_ALIGN(x)
	#define LIB_FOPEN(a, b)					_wfopen((wchar_t *)a, L##b)
	#define PLATFORM_PATH_SEPARATOR			'\\'
	#if defined(_MSC_VER)
		#include <direct.h>
		#pragma warning(disable:4351)
		#define getcwd _getcwd
		#define snprintf _snprintf
		#define vsnprintf _vsnprintf
		#define chdir _chdir
	#endif // _MSC_VER
#else
	#define LIB_FOPEN(a, b)					fopen((const char *)a, b)
	#define PLATFORM_PATH_SEPARATOR			'/'
#endif // WIN32

#define LIB_SIZE_T					size_t
#define LIB_RAND					rand() //(u32)(srand((unsigned int)(time(NULL))))

// Memory amd Stings
#define LIB_MEMSET					memset
#define LIB_MEMSET4(ptr, v, len)	do { int *p = static_cast<int *>(ptr); for (u32 x=0; x<len; x++) p[x]=v; } while (0)
#define LIB_MEMCOPY					memcpy
#define LIB_STRLEN					strlen
#define LIB_STRCMP					strcmp
#define LIB_STRLCPY(a, b, c)		strncpy(a, b, c); a[c] = '\0';
#define LIB_STRLCAT					strncat
#define LIB_STRCASECMP				strcasecmp
#define CHDIR						chdir

// Math
#define LIB_FAST_DIV(a, b)			(a/b)
#define LIB_FAST_MOD(a, b)			(a%b)
#define LIB_FAST_SQRT(a)			sqrt(a)

// Display Object
#define LIB_PIXEL_R_MASK			0xff000000
#define LIB_PIXEL_R_SHIFT			24
#define LIB_PIXEL_G_MASK			0x00ff0000
#define LIB_PIXEL_G_SHIFT			16
#define LIB_PIXEL_B_MASK			0x0000ff00
#define LIB_PIXEL_B_SHIFT			8
#define LIB_PIXEL_A_MASK			0x000000ff
#define LIB_PIXEL_A_SHIFT			0
#define LIB_PIXEL_DEFAULT_KEYCOLOR 	0x00ff00ff
#define	LIB_PIXEL_COMPONENT(x)		(x&0xff)
#define	LIB_PIXEL_COLOR(r, g, b, a)	((LIB_PIXEL_COMPONENT(b)<<LIB_PIXEL_B_SHIFT) | (LIB_PIXEL_COMPONENT(g)<<LIB_PIXEL_G_SHIFT) | (LIB_PIXEL_COMPONENT(r)<<LIB_PIXEL_R_SHIFT) | (LIB_PIXEL_COMPONENT(a)<<LIB_PIXEL_A_SHIFT))
#define LIB_PIXEL_ALPHA_MAX_VALUE	255

#define HALT	do { exit(-1); } while (1);

// Debugging
#define LIB_ASSERT(x)				if (!(x)) { fprintf(stderr, "Failed assertion " #x); HALT; }
#define LIB_ASSERT_MSG(x, msg)		if (!(x)) { fprintf(stderr, msg #x); HALT; }
#define LIB_ASSERT_NULL(x)			if (!((void *)x)) { fprintf(stderr, "Failed assertion " #x); HALT; }

#else // BUILD_QT
	#error "Include 'Defines.h' instead 'platform/qt/qtDefines.h' directly."
#endif // BUILD_QT
#endif // __QT_DEFINES_H__
