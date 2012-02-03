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

/*! \file EventInputKeyboard.cpp
	\author	Danny Angelo Carminati Grein
	\brief A event from an input keyboard
*/

#include "EventInputKeyboard.h"
#include "Enum.h"


namespace Seed {


EventInputKeyboard::EventInputKeyboard(u32 key, u32 modifier, u32 scan, u32 keyboard)
	: IEventInput(EVENT_INPUT_KEYBOARD)
	, cKey(key)
	, iModifier(modifier)
	, iScancode(scan)
	, iKeyboard(keyboard)
{
}

EventInputKeyboard::~EventInputKeyboard()
{
}

const Key &EventInputKeyboard::GetKey() const
{
	return cKey;
}

u32 EventInputKeyboard::GetModifier() const
{
	return iModifier;
}

u32 EventInputKeyboard::GetScancode() const
{
	return iScancode;
}

u32 EventInputKeyboard::GetKeyboard() const
{
	return iKeyboard;
}

bool EventInputKeyboard::IsShiftPressed() const
{
	return (iModifier & ModifierShift) > 0;
}

bool EventInputKeyboard::IsShiftLeftPressed() const
{
	return (iModifier & Seed::ModifierShiftLeft) > 0;
}

bool EventInputKeyboard::IsShiftRightPressed() const
{
	return (iModifier & Seed::ModifierShiftRight) > 0;
}

bool EventInputKeyboard::IsAltPressed() const
{
	return (iModifier & Seed::ModifierAlt) > 0;
}

bool EventInputKeyboard::IsAltLeftPressed() const
{
	return (iModifier & Seed::KeyAltLeft) > 0;
}

bool EventInputKeyboard::IsAltRightPressed() const
{
	return (iModifier & Seed::ModifierAltRight) > 0;
}

bool EventInputKeyboard::IsControlPressed() const
{
	return (iModifier & Seed::ModifierControl) > 0;
}

bool EventInputKeyboard::IsControlLeftPressed() const
{
	return (iModifier & Seed::ModifierControlLeft) > 0;
}

bool EventInputKeyboard::IsControlRightPressed() const
{
	return (iModifier & Seed::ModifierControlRight) > 0;
}

bool EventInputKeyboard::IsMetaPressed() const
{
	return (iModifier & Seed::ModifierMeta) > 0;
}

bool EventInputKeyboard::IsMetaLeftPressed() const
{
	return (iModifier & Seed::ModifierMetaLeft) > 0;
}

bool EventInputKeyboard::IsMetaRightPressed() const
{
	return (iModifier & Seed::ModifierMetaRight) > 0;
}

bool EventInputKeyboard::IsCapsLockOn() const
{
	return (iModifier & Seed::ModifierCapsLock) > 0;
}

bool EventInputKeyboard::IsNumLockOn() const
{
	return (iModifier & Seed::ModifierNumLock) > 0;
}

bool EventInputKeyboard::IsNumericPad() const
{
	return (cKey.GetValue() >= Seed::KeyPad0 && cKey.GetValue() <= Seed::KeyPadEquals);
}

bool EventInputKeyboard::IsCharacter() const
{
	return cKey.IsCharacter();
}

bool EventInputKeyboard::IsNumber() const
{
	return cKey.IsNumber();
}

bool EventInputKeyboard::IsLetter() const
{
	return cKey.IsLetter();
}

const char *EventInputKeyboard::GetObjectName() const
{
	return "EventInputKeyboard";
}


} // namespace

