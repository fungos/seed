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
#include <chrono>

#include "Config.h"

/*
Compiler specific pragmas here

About warning 4251 - DLL export for templatized classes (std and others)
http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
*/
#if defined(_MSC_VER)
#pragma warning(disable:4514) // unreferenced inline function has been removed (/WD)
#pragma warning(disable:4820) // 'n' bytes padding added after data memeber 'y' (/WD)
#pragma warning(disable:4350) // behavior change: 'method1' called instead of 'method2' (/WD)
#pragma warning(disable:4127) // conditional expression is constant
#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable:4530)
#pragma warning(disable:4996) // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4251) // stl + dll
#pragma warning(disable:4324) // structure was padded due to __declspec(align())
#pragma warning(disable:4316) // object allocated on the heap may not be aligned X
#pragma warning(disable:4351) // new behavior: elements of array 'X' will be default initialized
#pragma warning(disable:4275) // non dll-interface class 'X' used as base for dll-interface class 'Y'
#else
//#define override
#endif

#if defined(__MWERKS__)
#pragma warning off (10342)
#endif // __MWERKS__

#if defined(__MINGW32__)
	#if defined(SEED_BUILD_SHARED)
		#define SEED_CORE_API __declspec(dllexport)
	#elif defined(SEED_EXTRA_BUILD)
		#define SEED_PLATFORM_API __declspec(dllexport)
		#define SEED_EXTRA_API __declspec(dllexport)
		#define SEED_CORE_API __declspec(dllimport)
	#elif defined(SEED_USE_LGPL)
		#define SEED_CORE_API __declspec(dllimport)
		#define SEED_EXTRA_API __declspec(dllimport)
		#define SEED_PLATFORM_API _declspec(dllimport)
	#endif // __MINGW32__
#elif defined(_MSC_VER)
	#if defined(SEED_BUILD_SHARED)
		#define SEED_CORE_API _declspec(dllexport)
	#elif defined(SEED_EXTRA_BUILD)
		#define SEED_CORE_API _declspec(dllimport)
		#define SEED_EXTRA_API _declspec(dllexport)
		#define SEED_PLATFORM_API _declspec(dllexport)
	#elif defined(SEED_USE_LGPL)
		#define SEED_CORE_API __declspec(dllimport)
		#define SEED_EXTRA_API __declspec(dllimport)
		#define SEED_PLATFORM_API _declspec(dllimport)
	#endif // _MSC_VER
#endif

#define SEED_STRINGIZE_HELPER(x)	#x
#define SEED_STRINGIZE(x)			SEED_STRINGIZE_HELPER(x)
#define SEED_DO_PRAGMA(x)			_Pragma (#x)

#if BUILD_MESSAGES == 1 && !defined(_MSC_VER)
#define WARNING(desc)	SEED_DO_PRAGMA(message (__FILE__ "(" SEED_STRINGIZE(__LINE__) ") : warning: " #desc));
						//Wrn("WARNING: " __FILE__ "(" SEED_STRINGIZE(__LINE__) "): " SEED_STRINGIZE(desc));
#else
#define WARNING(desc)	//Wrn("WARNING: " __FILE__ "(" SEED_STRINGIZE(__LINE__) "): " SEED_STRINGIZE(desc));
#endif

#define OV_EXCLUDE_STATIC_CALLBACKS

#if defined(BUILD_SDL)
	#include "platform/sdl/sdlDefines.h"
#elif defined(BUILD_SDL2)
	#include "platform/sdl2/sdl2Defines.h"
#elif defined(BUILD_GLFW)
	#include "platform/glfw/glfwDefines.h"
#elif defined(BUILD_IOS)
	#include "platform/ios/iosDefines.h"
#elif defined(BUILD_QT)
	#include "platform/qt/qtDefines.h"
#endif // platform selector

#include "String.h"

#ifndef SEED_CORE_API
#define SEED_CORE_API
#endif

#ifndef SEED_PLATFORM_API
#define SEED_PLATFORM_API
#endif

#ifndef SEED_EXTRA_API
#define SEED_EXTRA_API
#endif

#define UNUSED(var)						(void)var;

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

#include "Log.h"

//#define SEED_COUNT(x)						((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

template <typename T, size_t N>
constexpr size_t countof(T(&)[N])
{
	return N;
}

// Debugging
#if defined(DEBUG)
	#if defined(__GNUC__)
		#define __FUNC__					__PRETTY_FUNCTION__
	#else
		#define __FUNC__					__FUNCSIG__
	#endif

	#define SEED_ASSERT(x)					if (!(x)) { Err("%s:%d: " #x, __FILE__, __LINE__); HALT}
	#define SEED_ASSERT_MSG(x, msg)			if (!(x)) { Err("%s:%d: (" #x "): " #msg, __FILE__, __LINE__); HALT}
	#define SEED_ASSERT_FMT(x, msg, ...)	if (!(x)) { Err("%s:%d: (" #x "): " #msg, __FILE__, __LINE__, __VA_ARGS__); HALT}
	#define SEED_WARNING(x, msg, ...)		if (x)    { Wrn("%s:%d: WARNING: (" #x "): " #msg, __FILE__, __LINE__, __VA_ARGS__); }
	#define SEED_CHECK_RETURN(x, ret, msg, ...)	if (!(x)) { Err("%s:%d: ERROR: (" #x "): " #msg, __FILE__, __LINE__); return ret; }
	#define SEED_ABSTRACT_METHOD			Dbg(SEED_TAG "WARNING: Calling an 'abstract' method: [%s] (%s:%d).", __FUNC__, __FILE__, __LINE__);
	#define SEED_DEPRECATED_METHOD			Dbg(SEED_TAG "WARNING: Calling a deprected method, please fix it: [%s] (%s:%d)", __FUNC__, __FILE__, __LINE__);
#else
	#define SEED_CHECK_RETURN(x, ret, msg, ...)	if (!(x)) { Err("ERROR: " #msg); return ret; }

	#define SEED_ABSTRACT_METHOD
	#define SEED_DEPRECATED_METHOD

	#if defined(__GNUC__)
		#define SEED_ASSERT(...)
		#define SEED_ASSERT_MSG(...)
		#define SEED_ASSERT_FMT(...)
		#define SEED_WARNING(...)
	#else
		#define SEED_ASSERT
		#define SEED_ASSERT_MSG
		#define SEED_ASSERT_FMT
		#define SEED_WARNING
	#endif // __GNUC__

#endif // DEBUG

#define SEED_INVALID_ID						0xFFFFFFFF

#define SEED_CLAMP(val,min,max) 			((val) = (((val) < (min)) ? (min) : ((val) > (max)) ? (max) : (val)))
#define SEED_ROUND_UP(value, alignment)		(((intptr_t)(value) + (alignment-1)) & ~(alignment-1))
#define SEED_ROUND_DOWN(value, alignment)	((intptr_t)(value) & ~(alignment-1))
#define SEED_PTR_OFF(ptr)					((intptr_t)(ptr))
#define SEED_ALIGN_OFFSET(ptr, align)		(SEED_PTR_OFF(ptr) & ((align) - 1))
#define SEED_ALIGN_FLOOR(ptr, align)		((u8 *)(ptr) - ( SEED_PTR_OFF(ptr) & ((align) - 1)))
#define SEED_ALIGN_CEIL(ptr, align)			((u8 *)(ptr) + (-SEED_PTR_OFF(ptr) & ((align) - 1)))

#define SEED_DISABLE_COPY(Class)			private:										\
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

extern "C" { namespace Seed {
	class ResourceManager;
	SEED_CORE_API extern ResourceManager *pResourceManager;
}}

namespace Seed {
	typedef std::chrono::high_resolution_clock Clock;
	typedef long long Milliseconds; // we use the raw numbers directly internally, no conversions needed.
	typedef float Seconds; // same here
	typedef std::chrono::duration<Milliseconds, std::milli> Duration;
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;
}

#endif // __SEED_DEFINES_H__
