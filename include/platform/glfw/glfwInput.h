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

#ifndef __GLFW_INPUT_H__
#define __GLFW_INPUT_H__

#if defined(BUILD_GLFW)

#include "interface/IInput.h"
#include "interface/IInputPointer.h"
#include "interface/IInputJoystick.h"
#include "interface/IInputKeyboard.h"
#include "Enum.h"
#include "Singleton.h"
#include "glfw/glfw.h"

#define MAX_JOYSTICKS 16

namespace Seed { namespace GLFW {

void GLFWCALL glfwOnKeyCb(int key, int action);
void GLFWCALL glfwOnCharCb(int character, int action);
void GLFWCALL glfwOnMouseButtonCb(int button, int action);
void GLFWCALL glfwOnMouseWheelCb(int pos, int btn);
void GLFWCALL glfwOnMousePosCb(int x, int y);

/// GLFW Input Module
class SEED_CORE_API Input : public IInput, public IInputPointer, public IInputKeyboard, public IInputJoystick
{
	friend void GLFWCALL glfwOnKeyCb(int key, int action);
	friend void GLFWCALL glfwOnCharCb(int character, int action);
	friend void GLFWCALL glfwOnMouseButtonCb(int button, int action);
	friend void GLFWCALL glfwOnMouseWheelCb(int pos, int btn);
	friend void GLFWCALL glfwOnMousePosCb(int x, int y);

	SEED_DECLARE_SINGLETON(Input)
	SEED_DECLARE_MANAGER(Input)
	SEED_DISABLE_COPY(Input)

	public:
		// IInput
		virtual eInputButton GetButtonCode(u32 button) const override;
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

		// IManager
		virtual bool Initialize() override;
		virtual bool Shutdown() override;
		virtual bool Reset() override;

	private:
		eKey GetKeyCode(u32 key) const;
		eModifier GetModifierCode(u32 mod) const;

		void OnKeyCb(int key, int action);
		void OnMouseButtonCb(int button, int action);
		void OnMouseWheelCb(int pos);
		void OnMousePosCb(int x, int y);

	private:
		u32 iJoystickCount;
		f32 fX;
		f32 fY;
		u32 iWheel;
		u32 iOldWheel;
		u32 iOriginX;
		u32 iOriginY;
		u32 iOldX;
		u32 iOldY;
		u32 iX;
		u32 iY;
		u32 iAxesMax;
		u32 iButtonsMax;

		struct glfwJoyInfo {
			bool bIsPresent;
			u32 iAxes;
			u32 iButtons;
		};

		glfwJoyInfo arJoyInfo[MAX_JOYSTICKS];
		u32 iModifiers;
};

#define pInput Seed::GLFW::Input::GetInstance()

}} // namespace

#else // BUILD_GLFW
	#error "Include 'Input.h' instead 'platform/glfw/glfwInput.h' directly."
#endif // BUILD_GLFW
#endif // __GLFW_INPUT_H__
