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

#ifndef __QT_SYSTEM_H__
#define __QT_SYSTEM_H__

#if defined(BUILD_QT)

#include <QWidget>
#include "interface/ISystem.h"
#include "Singleton.h"

namespace Seed { namespace QT {

/// Qt System Module
class System : public ISystem
{
	SEED_SINGLETON_DECLARE(System)
	public:
		// ISystem
		virtual void GoToMenu();
		virtual void Sleep();
		virtual void OnHomeCalled();
		virtual void WaitForRetrace(eSystemFrameRate rate);
		virtual void SetFrameRate(eSystemFrameRate rate);
		virtual void HangUp();
		virtual void GoToDataManager();

		virtual eSystemFrameRate GetFrameRate();

		virtual bool IsShuttingDown() const;
		virtual bool IsResetting() const;
		virtual bool IsSleeping() const;

		virtual void DisableHome();
		virtual void EnableHome();
		virtual bool IsHomeEnabled() const;
		virtual bool IsHomeRunning() const;
		virtual bool InitializeHome();

		virtual void WriteOut(const char *msg);
		virtual void WriteErr(const char *msg);
		virtual void SetWidget(QWidget *widget);

		// IUpdatable
		virtual bool Update(f32 dt);

		// IModule
		virtual bool Initialize();
		virtual bool Shutdown();
		virtual bool Reset();

	private:
		SEED_DISABLE_COPY(System);

	private:
		u32 		iRetraceCount;
		eSystemFrameRate iFrameRate;
		bool 		bShutdown;
		f32 		fElapsedTime;
		u64 		iLastFrameTime;
		u64 		iFpsTime;

		QWidget		*pWidget;
};

#define pSystem Seed::QT::System::GetInstance()

}} // namespace

#else // BUILD_QT
	#error "Include 'System.h' instead 'platform/qt/qtSystem.h' directly."
#endif // BUILD_QT
#endif // __QT_SYSTEM_H__
