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

#ifndef __QT_SCREEN_H__
#define __QT_SCREEN_H__

#include "interface/IScreen.h"

#if defined(BUILD_QT)

#define FADE_OUT_COLOR	0xff
#define FADE_OUT_SOLID	0xff
#define FADE_OUT_TRANS	0x00

#if defined(DEBUG)
#define FADE_INCREMENT	0x04
#else
#define FADE_INCREMENT	0x20
#endif // DEBUG

#include <QPainter>
#include <QGraphicsScene>

#include "Singleton.h"

namespace Seed { namespace QT {

class Scene;
class IRenderer;

/// Qt Screen Module
class Screen : public IScreen
{
	SEED_SINGLETON_DECLARE(Screen)
	public:
		virtual u32 GetHeight() const;
		virtual u32 GetWidth() const;
		virtual void ToggleFullscreen();
		void *GetSurface() const;

		// IScreen
		virtual void Update();

		// IModule
		virtual bool Initialize();
		virtual bool Reset();
		virtual bool Shutdown();

	protected:
		static Scene *pScene;
		u32 surfaceSize;

	private:
		SEED_DISABLE_COPY(Screen);

		void PrepareMode();
		void CreateHardwareSurfaces();
		void DestroyHardwareSurfaces();

#if defined(DEBUG)
		void PrintVideoMode();
#endif // DEBUG

	private:
		u32			iHeight;
		u32			iWidth;
		u32			iFlags;
		u8			iBPP;
		bool		bFullScreen;
};

#define pScreen Seed::QT::Screen::GetInstance()

}} // namespace

#else //._QT_
	#error "Include 'Screen.h' instead 'platform/qt/qtScreen.h' directly."
#endif // BUILD_QT
#endif // __QT_SCREEN_H__
