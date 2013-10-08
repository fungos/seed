/*
* Copyright (c) 2013, Seed Developers
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

#ifndef __SDL2_INPUT_H__
#define __SDL2_INPUT_H__

#if defined(BUILD_SDL2)

#include "interface/IInput.h"
#include "interface/IInputPointer.h"
#include "interface/IInputJoystick.h"
#include "interface/IInputKeyboard.h"
#include "Enum.h"
#include "Singleton.h"

#define MAX_JOYSTICKS 32

namespace Seed { namespace SDL2 {

/// SDL Input Module
class SEED_CORE_API Input : public IInput, public IInputPointer, public IInputKeyboard, public IInputJoystick
{
	SEED_SINGLETON_DECLARE(Input)
	public:
		// IInput
		virtual Seed::eInputButton GetButtonCode(u32 button) const override;
		virtual u32 ConvertButtonFlags(u32 flags) override;

		virtual bool IsJoystick() const override;
		virtual bool IsPointer() const override;
		virtual bool IsKeyboard() const override;

		// IInputJoystick
		virtual u32 GetMaximumJoysticks() const override;

		// IInputKeyboard

		// IInputPointer
		virtual bool IsPointerEnabled(u16 joystick = 0) const override;
		virtual bool IsHold(u32 button, u16 joystick = 0) const override;
		virtual bool IsPressed(u32 button, u16 joystick = 0) const override;
		virtual bool IsReleased(u32 button, u16 joystick = 0) const override;

		virtual f32 GetX(u16 joystick = 0) const override;
		virtual f32 GetY(u16 joystick = 0) const override;
		virtual f32 GetRelativeX(u16 joystick = 0) const override;
		virtual f32 GetRelativeY(u16 joystick = 0) const override;
		virtual f32 GetOrientation(u16 joystick = 0) const override;
		virtual f32 GetDistance(u16 joystick = 0) const override;

		virtual u32 GetSensitivity(u16 joystick = 0) const override;
		virtual u32 GetMaximumSensitivity(u16 joystick = 0) const override;
		virtual u32 GetMaximumPointers() const override;
		virtual void SetSensitivity(u32 sens, u16 joystick = 0) override;

		// IUpdatable
		virtual bool Update(f32 dt) override;

		// IModule
		virtual bool Initialize() override;
		virtual bool Shutdown() override;
		virtual bool Reset() override;

	private:
		SEED_DISABLE_COPY(Input);

		Seed::eKey GetKeyCode(u32 key) const;
		Seed::eModifier GetModifierCode(u32 mod) const;

	private:
		u32  iJoystickCount;
		u32 iX;
		u32 iY;

		SDL_Joystick *parJoy[MAX_JOYSTICKS];
};

#define pInput Seed::SDL2::Input::GetInstance()

}} // namespace

#else // BUILD_SDL2
	#error "Include 'Input.h' instead 'platform/sdl/sdlInput.h' directly."
#endif // BUILD_SDL2
#endif // __SDL2_INPUT_H__
