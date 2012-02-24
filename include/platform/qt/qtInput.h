/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file qtInput.h
	\author	Danny Angelo Carminati Grein
	\brief Input Implementation
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

class Input : public IInput, public IInputPointer, public IInputKeyboard
{
	friend class Scene;

	SEED_SINGLETON_DECLARE(Input)
	public:
		// IInput
		virtual eInputButton GetButtonCode(u32 button) const;
		virtual u32 ConvertButtonFlags(u32 flags);

		virtual bool IsPointer() const;
		virtual bool IsKeyboard() const;

		// IInputPointer
		virtual bool IsEnabled(u16 joystick = 0) const;
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
		SEED_DISABLE_COPY(Input);

		eModifier TranslateModifier(u32 mod);
		eKey TranslateKey(u32 key);
};

#define pInput Seed::QT::Input::GetInstance()

}} // namespace

#else // BUILD_QT
	#error "Include 'Input.h' instead 'platform/qt/qtInput.h' directly."
#endif // BUILD_QT
#endif // __QT_INPUT_H__
