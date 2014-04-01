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

#ifndef __QT_INPUT_H__
#define __QT_INPUT_H__

#if defined(BUILD_QT)

#include "interface/IInput.h"
#include "interface/IInputPointer.h"
#include "interface/IInputKeyboard.h"
#include "Singleton.h"

namespace Seed { namespace QT {

class Scene;

/// Qt Input Module
class Input : public IInput, public IInputPointer, public IInputKeyboard
{
	friend class Scene;

	SEED_DECLARE_SINGLETON(Input)
	SEED_DISABLE_COPY(Input)

	public:
		// IInput
		virtual eInputButton GetButtonCode(u32 button) const;
		virtual u32 ConvertButtonFlags(u32 flags);

		virtual bool IsPointer() const;
		virtual bool IsKeyboard() const;

		// IInputPointer
		virtual bool IsPointerEnabled(u16 joystick = 0) const;
		virtual bool IsHold(u32 button, u16 joystick = 0) const;
		virtual bool IsPressed(u32 button, u16 joystick = 0) const;
		virtual bool IsReleased(u32 button, u16 joystick = 0) const;
		virtual f32 GetX(u16 joystick = 0) const;
		virtual f32 GetY(u16 joystick = 0) const;
		virtual f32 GetRelativeX(u16 joystick = 0) const;
		virtual f32 GetRelativeY(u16 joystick = 0) const;
		virtual f32 GetOrientation(u16 joystick = 0) const;
		virtual f32 GetDistance(u16 joystick = 0) const;
		virtual u32 GetSensitivity(u16 joystick = 0) const;
		virtual u32 GetMaximumSensitivity(u16 joystick = 0) const;
		virtual u32 GetMaximumPointers() const;
		virtual void SetSensitivity(u32 sens, u16 joystick = 0);

		// IUpdatable
		virtual bool Update(Seconds dt);

		// IManager
		virtual bool Initialize();
		virtual bool Shutdown();
		virtual bool Reset();

	protected:
		void PointerMove(f32 x, f32 y, u16 joystick = 0);
		void PointerPress(f32 x, f32 y, u32 btn, u16 joystick = 0);
		void PointerRelease(f32 x, f32 y, u32 btn, u16 joystick = 0);
		void KeyboardPress(u32 key, u32 modifier, u16 joystick = 0);
		void KeyboardRelease(u32 key, u32 modifier, u16 joystick = 0);

	protected:
		f32 fX;
		f32 fY;

	private:
		eModifier TranslateModifier(u32 mod);
		eKey TranslateKey(u32 key);
};

#define pInput Seed::QT::Input::GetInstance()

}} // namespace

#else // BUILD_QT
	#error "Include 'Input.h' instead 'platform/qt/qtInput.h' directly."
#endif // BUILD_QT
#endif // __QT_INPUT_H__
