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
void GLFWCALL glfwOnMouseWheelCb(int pos);
void GLFWCALL glfwOnMousePosCb(int x, int y);

/// GLFW Input Module
class SEED_CORE_API Input : public IInput, public IInputPointer, public IInputKeyboard, public IInputJoystick
{
	friend void GLFWCALL glfwOnKeyCb(int key, int action);
	friend void GLFWCALL glfwOnCharCb(int character, int action);
	friend void GLFWCALL glfwOnMouseButtonCb(int button, int action);
	friend void GLFWCALL glfwOnMouseWheelCb(int pos);
	friend void GLFWCALL glfwOnMousePosCb(int x, int y);

	SEED_SINGLETON_DECLARE(Input)
	public:
		// IInput
		virtual Seed::eInputButton GetButtonCode(u32 button) const;
		virtual u32 ConvertButtonFlags(u32 flags);

		virtual bool IsJoystick() const;
		virtual bool IsPointer() const;
		virtual bool IsKeyboard() const;

		// IInputJoystick
		virtual u32 GetMaximumJoysticks() const;

		// IInputKeyboard

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
		virtual bool Update(f32 dt);

		// IModule
		virtual bool Initialize();
		virtual bool Shutdown();
		virtual bool Reset();

	private:
		SEED_DISABLE_COPY(Input);

		Seed::eKey GetKeyCode(u32 key) const;
		Seed::eModifier GetModifierCode(u32 mod) const;

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
		u32 iOriginWheel;
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
		int nModifiers;
};

#define pInput Seed::GLFW::Input::GetInstance()

}} // namespace

#else // BUILD_GLFW
	#error "Include 'Input.h' instead 'platform/glfw/glfwInput.h' directly."
#endif // BUILD_GLFW
#endif // __GLFW_INPUT_H__