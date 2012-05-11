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

#ifndef __PROFILER_H__
#define __PROFILER_H__

#if defined(SEED_ENABLE_PROFILER)
#include "Defines.h"
#include <map>
#include <stack>
#include <time.h>

#define SEED_FUNCTION_PROFILER					ProfileContext _ctx_func(__PRETTY_FUNCTION__ ); //__FUNCTION__); // Check: MSVC and Xcode
#define SEED_BEGIN_REGION_PROFILER(name, str)	ProfileContext _c##name(str, Profiler::regionProfilerInstance);
#define SEED_END_REGION_PROFILER(name)			_c##name.Terminate();
#define ProfilerReportPrint		Profiler::funcProfilerInstance->Dump(); Profiler::regionProfilerInstance->Dump();
#define ProfilerTerminate		do { \
									delete Profiler::funcProfilerInstance; \
									Profiler::funcProfilerInstance = NULL; \
									delete Profiler::regionProfilerInstance; \
									Profiler::regionProfilerInstance = NULL; \
								} \
								while (0);

class ProfileContext;

/// Profile Entry Data
struct SEED_CORE_API ProfilerEntry
{
	f32 time;
	f32 maxtime;
	u32 calls;
	f32 stack[256];
	u32 pos;

	ProfilerEntry()
		: time(0)
		, maxtime(0)
		, calls(0)
		, stack()
		, pos(0)
	{}
};

typedef std::map<const char *, ProfilerEntry *> FuncTimeMap;
typedef std::pair<const char *, ProfilerEntry *> FuncTimePair;
typedef FuncTimeMap::iterator FuncTimeMapIt;
typedef std::stack<ProfileContext *> ContextStack;

/// Profiler
class SEED_CORE_API Profiler
{
	public:
		Profiler(const char *name);
		~Profiler();

		void AddSlice(const char *func, f32 time);
		void AddTotal(const char *func, f32 time);
		void Dump();
		void Reset();

		static Profiler *funcProfilerInstance;
		static Profiler *regionProfilerInstance;

	private:
		SEED_DISABLE_COPY(Profiler);

	private:
		const char *pName;
		FuncTimeMap mapSubjectSlice;
		FuncTimeMap mapSubjectTotal;
};

/// Profiler Context
class SEED_CORE_API ProfileContext
{
	public:
		ProfileContext(const char *f, Profiler *prof = Profiler::funcProfilerInstance);
		~ProfileContext();

		void Terminate();
		void StopPrevious();
		void RestorePrevious();
		void StopAndCommit();
		void StartOrContinue();
		void Push();
		void Pop();

		static ContextStack stack;

	private:
		SEED_DISABLE_COPY(ProfileContext);

	private:
		const char *func;
		u64 beg;
		u64 begTotal;
		bool bTerminated;

		Profiler *pProf;
};
#else

#define SEED_FUNCTION_PROFILER
#define SEED_BEGIN_REGION_PROFILER(name, str)
#define SEED_END_REGION_PROFILER(name)
#define ProfilerReportPrint
#define ProfilerTerminate

#endif // SEED_ENABLE_PROFILER

#endif //__PROFILER_H__
