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

#include "EventInputKeyboard.h"
#include "Enum.h"

namespace Seed {

EventInputKeyboard::EventInputKeyboard(u32 key, u32 modifier, u32 scan, u32 keyboard)
	: IEventInput(EventIdKeyboardInput)
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

const String EventInputKeyboard::GetClassName() const
{
	return "EventInputKeyboard";
}

} // namespace

