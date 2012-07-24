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

#if defined(WIN32)
	#undef OUT
	#define ATTRIBUTE_ALIGN(x)
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
	#define PLATFORM_PATH_SEPARATOR			'/'
#endif // WIN32

#define HALT	do { exit(-1); } while (1);

#else // BUILD_QT
	#error "Include 'Defines.h' instead 'platform/qt/qtDefines.h' directly."
#endif // BUILD_QT
#endif // __QT_DEFINES_H__
