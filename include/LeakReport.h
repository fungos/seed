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

#ifndef __LEAK_REPORT_H__
#define __LEAK_REPORT_H__

#if defined(DEBUG)

#include "Defines.h"
#include "Log.h"
#include "Singleton.h"
#include "interface/IObject.h"

#include <chrono>
#include <map>

#define SEED_LEAK_MAX			128

#define LeakReportPrint			pLeakReport->Print();

namespace Seed {

/// Leak Reporter
class SEED_CORE_API LeakReport
{
	SEED_DECLARE_SINGLETON(LeakReport)
	SEED_DISABLE_COPY(LeakReport)

	public:
		void Debug(void *ptr)
		{
			void *addr = reinterpret_cast<void *>(ptr);
			fprintf(stdout, "WHAT DELETE: %p\n", addr);
			for (int i = 0; i < SEED_LEAK_MAX; ++i)
			{
				if (arInfo[i].ptrAddr != nullptr)
					fprintf(stdout, "CHECK: %d - %p == %p\n", i, addr, arInfo[i].ptrAddr);
			}
		}

		template <class T>
		T *LogNew(T *ptr, const char *call, const char *file, int line, const char *func)
		{
			return (T *)LogNew((void *)ptr, call, file, line, func);
		}

		void *LogNew(void *ptr, const char *call, const char *file, int line, const char *func)
		{
			for (int i = 0; i < SEED_LEAK_MAX; ++i)
			{
				if (arInfo[i].ptrAddr == nullptr)
				{
					arInfo[i].ptrAddr = (void *)ptr;

					strncpy(arInfo[i].strCall, call, 64);
					strncpy(arInfo[i].strFile, file, 128);
					strncpy(arInfo[i].strFunc, func, 256);

					arInfo[i].iLine = line;
					arInfo[i].iFrame = 0;

					//fprintf(stdout, "== New: [%p] %s:%d: %s -> %s\n", arInfo[i].ptrAddr, arInfo[i].strFile, arInfo[i].iLine, arInfo[i].strFunc, arInfo[i].strCall);
					break;
				}
			}

			return ptr;
		}

		template <class T>
		void LogDelete(T *ptr)
		{
			void *addr = reinterpret_cast<void *>(ptr);

			for (int i = 0; i < SEED_LEAK_MAX; ++i)
			{
				if (addr == arInfo[i].ptrAddr)
				{
					//fprintf(stdout, "== Delete: [%p] %s:%d: %s -> %s\n", arInfo[i].ptrAddr, arInfo[i].strFile, arInfo[i].iLine, arInfo[i].strFunc, arInfo[i].strCall);
					arInfo[i].ptrAddr = nullptr;

					memset(arInfo[i].strCall, '\0', 64);
					memset(arInfo[i].strFile, '\0', 128);
					memset(arInfo[i].strFunc, '\0', 256);

					arInfo[i].iLine = 0;
					arInfo[i].iFrame = 0;
					break;
				}
			}
		}

		void Print();

	private:
		struct PointerInfo
		{
			void *ptrAddr;
			char strCall[64];
			char strFile[128];
			char strFunc[256];
			int  iLine;
			u64  iFrame;
		};

		PointerInfo arInfo[SEED_LEAK_MAX];
};

#define pLeakReport Seed::LeakReport::GetInstance()

} // namespace

#else

#define LeakReportPrint

#endif // DEBUG

#endif // __LEAK_REPORT_H__
