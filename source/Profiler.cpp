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

#if defined(SEED_ENABLE_PROFILER)

#include "Timer.h"
#include "Profiler.h"
#include "Log.h"

#define TAG "[Profiler] "
#define STORE_VALUE(v) { p->stack[p->pos++] = v; if (p->pos > 256) p->pos = 0; }

Profiler *Profiler::funcProfilerInstance = new Profiler("Function");
Profiler *Profiler::regionProfilerInstance = new Profiler("Region");

ContextStack ProfileContext::stack;

Profiler::Profiler(const char *name)
	: pName(name)
	, mapSubjectSlice()
	, mapSubjectTotal()
{
}

Profiler::~Profiler()
{
	this->Reset();
}

void Profiler::AddSlice(const char *func, f32 time)
{
	FuncTimeMapIt it = mapSubjectSlice.find(func);
	if (it != mapSubjectSlice.end())
	{
		ProfilerEntry *p = (*it).second;
		p->calls++;
		p->time += time;
		STORE_VALUE(time);

		if (time > p->maxtime)
			p->maxtime = time;
	}
	else
	{
		ProfilerEntry *p = new ProfilerEntry();
		p->calls = 0;
		p->time = time;
		p->maxtime = time;
		STORE_VALUE(time);

		mapSubjectSlice.insert(FuncTimePair(func, p));
	}
}

void Profiler::AddTotal(const char *func, f32 time)
{
	FuncTimeMapIt it = mapSubjectTotal.find(func);
	if (it != mapSubjectTotal.end())
	{
		ProfilerEntry *p = (*it).second;
		p->calls++;
		p->time += time;
		STORE_VALUE(time);

		if (time > p->maxtime)
			p->maxtime = time;
	}
	else
	{
		ProfilerEntry *p = new ProfilerEntry();
		p->calls = 1;
		p->time = time;
		p->maxtime = time;
		STORE_VALUE(time);

		mapSubjectTotal.insert(FuncTimePair(func, p));
	}
}

void Profiler::Dump()
{
#ifdef DEBUG
	Log(TAG "Dumping %s Totals: %d", pName, (int)mapSubjectTotal.size());

	for (auto each: mapSubjectTotal)
	{
		ProfilerEntry *entry = each.second;
		int calls = entry->calls;
		float average = static_cast<float>((float)entry->time / (float)calls);
		Log(TAG "%s: %s [average time %fms, called %d times]", pName, each.first, average, calls);
	}

	Log(TAG "Dumping %s Slices: %d", pName, (int)mapSubjectSlice.size());

	for (auto each: mapSubjectSlice)
	{
		ProfilerEntry *entry = each.second;
		int calls = entry->calls;
		float average = static_cast<float>((float)entry->time / (float)calls);
		Log(TAG "%s: %s [average time %fms, interrupted %d times]", pName, each.first, average, calls);
	}
#endif
}

void Profiler::Reset()
{
	for (auto each: mapSubjectTotal)
		delete each.second;
	mapSubjectTotal.clear();

	for (auto each: mapSubjectSlice)
		delete each.second;
	mapSubjectSlice.clear();
}

ProfileContext::ProfileContext(const char *f, Profiler *prof)
	: func(f)
	, beg(0)
	, begTotal(0)
	, bTerminated(false)
	, pProf(prof)
{
	begTotal = pTimer->GetMilliseconds(); //clock();
	this->Push();
	this->StartOrContinue();
}

ProfileContext::~ProfileContext()
{
	this->Terminate();
}

void ProfileContext::Terminate()
{
	if (bTerminated)
		return;

	u64 end = pTimer->GetMilliseconds(); //clock();

	f32 diff = 0;
	diff = static_cast<f32>(end - begTotal);// / CLOCKS_PER_SEC;

	pProf->AddTotal(func, diff);

	this->StopAndCommit();
	this->Pop();
	bTerminated = true;
}

void ProfileContext::StopPrevious()
{
	if (!stack.empty())
	{
		ProfileContext *parent = stack.top();
		parent->StopAndCommit();
	}
}

void ProfileContext::RestorePrevious()
{
	if (!stack.empty())
	{
		ProfileContext *parent = stack.top();
		parent->StartOrContinue();
	}
}

void ProfileContext::StopAndCommit()
{
	u64 end = pTimer->GetMilliseconds();

	f32 diff = 0;
	diff = static_cast<f32>(end - beg);// / CLOCKS_PER_SEC;

	pProf->AddSlice(func, diff);
	beg = 0;
}

void ProfileContext::StartOrContinue()
{
	beg = pTimer->GetMilliseconds();
}

void ProfileContext::Push()
{
	this->StopPrevious();
	stack.push(this);
}

void ProfileContext::Pop()
{
	if (!stack.empty())
		stack.pop();
	this->RestorePrevious();
}

#endif // SEED_ENABLE_PROFILER
