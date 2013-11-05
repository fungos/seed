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

#ifndef __SEED_DEFINES_H__
#define __SEED_DEFINES_H__

#include <string>
#include "Config.h"

/*
Compiler specific pragmas here

About warning 4251 - DLL export for templatized classes (std and others)
http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
*/
#if defined(_MSC_VER)
#pragma warning(disable:4127) // conditional expression is constant
#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable:4530)
#pragma warning(disable:4996) // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4251) // stl + dll
#else
//#define override
#endif

#define STRINGIZE_HELPER(x) #x
#define STRINGIZE(x) STRINGIZE_HELPER(x)
#define DO_PRAGMA(x) _Pragma (#x)

#if BUILD_MESSAGES == 1
#define WARNING(desc) DO_PRAGMA(message (__FILE__ "(" STRINGIZE(__LINE__) ") : warning: " #desc));
#else
#define WARNING(desc)
#endif

#define OV_EXCLUDE_STATIC_CALLBACKS

#if defined(BUILD_SDL)
	#include "platform/sdl/sdlDefines.h"
#elif defined(BUILD_GLFW)
	#include "platform/glfw/glfwDefines.h"
#elif defined(BUILD_IOS)
	#include "platform/ios/iosDefines.h"
#elif defined(BUILD_QT)
	#include "platform/qt/qtDefines.h"
#endif // platform selector

#ifndef SEED_CORE_API
#define SEED_CORE_API
#endif

#ifndef SEED_PLATFORM_API
#define SEED_PLATFORM_API
#endif

#ifndef SEED_EXTRA_API
#define SEED_EXTRA_API
#endif

#ifdef _MSC_VER
#define UNUSED(var)
#else
#define UNUSED(var)						(void)var;
#endif

typedef std::string String;

struct Color
{
	u8 r;
	u8 g;
	u8 b;
	u8 a;

	Color(u8 R, u8 G, u8 B, u8 A)
		: r(R), g(G), b(B), a(A)
	{
	}

	Color()
		: r(255), g(0), b(255), a(255)
	{
	}
};

typedef Color Color4b;

// Debugging
#if defined(DEBUG)
	#include "Log.h"
	#define SEED_ASSERT(x)					if (!(x)) { Err("%s:%d: " #x, __FILE__, __LINE__); HALT}
	#define SEED_ASSERT_MSG(x, msg)			if (!(x)) { Err("%s:%d: (" #x "): " #msg, __FILE__, __LINE__); HALT}
	#define SEED_ASSERT_FMT(x, msg, ...)	if (!(x)) { Err("%s:%d: (" #x "): " #msg, __FILE__, __LINE__, __VA_ARGS__); HALT}
	#define SEED_WARNING(x, msg, ...)		if (x) { Wrn("%s:%d: WARNING: (" #x "): " #msg, __FILE__, __LINE__, __VA_ARGS__); }

	#if defined(__GNUC__)
		#define __FUNC__					__PRETTY_FUNCTION__
	#else
		#define __FUNC__					__FUNCSIG__
	#endif

	#define SEED_ABSTRACT_METHOD		Dbg(SEED_TAG "WARNING: Calling an 'abstract' method: [%s] (%s:%d).", __FUNC__, __FILE__, __LINE__);
	#define SEED_DEPRECATED_METHOD		Dbg(SEED_TAG "WARNING: Calling a deprected method, please fix it: [%s] (%s:%d)", __FUNC__, __FILE__, __LINE__);
#else
	#define SEED_ABSTRACT_METHOD
	#define SEED_DEPRECATED_METHOD

	#if defined(__GNUC__)
		#define SEED_ASSERT(...)
		#define SEED_ASSERT_MSG(...)
	#else
		#define SEED_ASSERT
		#define SEED_ASSERT_MSG
	#endif // __GNUC__

#endif // DEBUG

#define SEED_INVALID_ID					0xFFFFFFFF

#define CLAMP(val,min,max) 				((val) = (((val) < (min)) ? (min) : ((val) > (max)) ? (max) : (val)))
#define ROUND_UP(value, alignment)		(((u32)(value) + (alignment-1)) & ~(alignment-1))
#define ROUND_DOWN(value, alignment)	((u32)(value) & ~(alignment-1))
#define PTR_OFF(ptr)					((size_t)(ptr))
#define ALIGN_OFFSET(ptr, align)		(PTR_OFF(ptr) & ((align) - 1))
#define ALIGN_FLOOR(ptr, align)			((u8 *)(ptr) - ( PTR_OFF(ptr) & ((align) - 1)))
#define ALIGN_CEIL(ptr, align)			((u8 *)(ptr) + (-PTR_OFF(ptr) & ((align) - 1)))

#define SEED_DISABLE_COPY(Class)		private:										\
											Class(const Class &) = delete;				\
											Class &operator=(const Class &) = delete;	\

#define SEED_COMPILE_TIME_ASSERT(name, x)	typedef int __seed_compile_assert_ ## name[(x) * 2 - 1]

SEED_COMPILE_TIME_ASSERT(u8,  sizeof(u8) == 1);
SEED_COMPILE_TIME_ASSERT(s8,  sizeof(s8) == 1);
SEED_COMPILE_TIME_ASSERT(u16, sizeof(u16) == 2);
SEED_COMPILE_TIME_ASSERT(s16, sizeof(s16) == 2);
SEED_COMPILE_TIME_ASSERT(u32, sizeof(u32) == 4);
SEED_COMPILE_TIME_ASSERT(s32, sizeof(s32) == 4);
SEED_COMPILE_TIME_ASSERT(u64, sizeof(u64) == 8);
SEED_COMPILE_TIME_ASSERT(s64, sizeof(s64) == 8);

typedef enum { SEED_ENUM_ASSERT_VALUE } SEED_ENUM_ASSERT;
SEED_COMPILE_TIME_ASSERT(enum, sizeof(SEED_ENUM_ASSERT) == sizeof(u32));

#include "LeakReport.h"

extern "C" { namespace Seed {
	class ResourceManager;
	SEED_CORE_API extern ResourceManager *pResourceManager;
}}

#endif // __SEED_DEFINES_H__
