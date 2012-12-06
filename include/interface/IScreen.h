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

#ifndef __ISCREEN_H__
#define __ISCREEN_H__

#include "IModule.h"
#include "Enum.h"

namespace Seed {

/// Screen interface
/**
Any Screen implementation must implement this interface.
*/
class SEED_CORE_API IScreen : public IModule
{
	public:
		IScreen();
		virtual ~IScreen();

		/// Start fade out
		virtual void FadeOut();

		/// Start fade in
		virtual void FadeIn();

		/// Get the screen heigth
		virtual u32 GetHeight() const;

		/// Get the screen width
		virtual u32 GetWidth() const;

		/// Get the aspect ratio
		virtual f32 GetAspectRatio() const;

		/// Verify if a fade is being done.
		virtual bool IsFading() const;

		/// Change between fullscreen and windowed for platforms that supports it
		virtual void ToggleFullscreen();

		/// Check if the platform support windowed video mode
		virtual bool HasWindowedMode() const;

		/// Check if it is fullscreen
		virtual bool IsFullscreen() const;

		virtual void Update();

		// IModule
		virtual bool IsRequired() const;

		// IObject
		virtual const String GetClassName() const;

	protected:
		bool		bFading;
		f32			fAspectRatio;
		u32			iHeight;
		u32			iWidth;

	private:
		SEED_DISABLE_COPY(IScreen);
};

} // namespace

#endif // __ISCREEN_H__
