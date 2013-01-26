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

#include <map>

#define SEED_LEAK_MAX			128

#define New(T)					pLeakReport->LogNew((new T), #T, __FILE__, __LINE__, __FUNC__)
#define Delete(ptr)				{ if (ptr) pLeakReport->LogDelete(ptr); ptr = NULL; }
#define NewArray(T, L)			pLeakReport->LogNew((new T[L]), #T, __FILE__, __LINE__, __FUNC__)
#define DeleteArray(ptr)		{ if (ptr) pLeakReport->LogDelete(ptr, true); ptr = NULL; }
#define Alloc(S)				malloc(S)
#define sFree(ptr)				{ if (ptr) free(ptr); ptr = NULL; }

#define LeakReportPrint			pLeakReport->Print();

namespace Seed {

/// Leak Reporter
class SEED_CORE_API LeakReport
{
	SEED_SINGLETON_DECLARE(LeakReport)

	public:
		template <class T>
		T *LogNew(T* ptr, const char *call, const char *file, int line, const char *func)
		{
			for (int i = 0; i < SEED_LEAK_MAX; ++i)
			{
				if (arInfo[i].ptrAddr == NULL)
				{
					arInfo[i].ptrAddr = ptr;

					strncpy(arInfo[i].strCall, call, 64);
					strncpy(arInfo[i].strFile, file, 128);
					strncpy(arInfo[i].strFunc, func, 256);

					arInfo[i].iLine = line;

					//Log("New: [0x%8x] %s:%d: %s -> %s", arInfo[i].ptrAddr, arInfo[i].strFile, arInfo[i].iLine, arInfo[i].strFunc, arInfo[i].strCall);
					break;
				}
			}

			return ptr;
		}

		template <class T>
		void LogDelete(T *ptr, bool array = false)
		{
			void *addr = (void *)ptr;

			for (int i = 0; i < SEED_LEAK_MAX; ++i)
			{
				if (addr == arInfo[i].ptrAddr)
				{
					//Log("Delete: [0x%8x] %s:%d: %s -> %s", arInfo[i].ptrAddr, arInfo[i].strFile, arInfo[i].iLine, arInfo[i].strFunc, arInfo[i].strCall);
					arInfo[i].ptrAddr = NULL;

					memset(arInfo[i].strCall, '\0', 64);
					memset(arInfo[i].strFile, '\0', 128);
					memset(arInfo[i].strFunc, '\0', 256);

					arInfo[i].iLine = 0;
					break;
				}
			}

			if (ptr)
			{
				if (array)
					delete []ptr;
				else
					delete ptr;

				ptr = NULL;
			}
		}

		void Print();

	private:
		SEED_DISABLE_COPY(LeakReport);

		struct PointerInfo
		{
			void *ptrAddr;
			char strCall[64];
			char strFile[128];
			char strFunc[256];
			int  iLine;
		};

		PointerInfo arInfo[SEED_LEAK_MAX];
};

#define pLeakReport Seed::LeakReport::GetInstance()

} // namespace

#else

#define New(T)					new T
#define Delete(ptr)				{ if (ptr) delete ptr; ptr = NULL; }
#define NewArray(T, L)			new T[L]
#define DeleteArray(ptr)		{ if (ptr) delete []ptr; ptr = NULL; }
#define Alloc(S)				malloc(S)
#define Free(ptr)				free(ptr)

#define LeakReportPrint

#endif // DEBUG

#endif // __LEAK_REPORT_H__
