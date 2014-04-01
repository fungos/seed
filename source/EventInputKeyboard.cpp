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

EventInputKeyboard::EventInputKeyboard(eKey key, eModifier modifier, u32 scan, u32 keyboard)
	: IEventInput()
	, nKey(key)
	, nModifier(modifier)
	, iScancode(scan)
	, iKeyboard(keyboard)
{
}

EventInputKeyboard::~EventInputKeyboard()
{
}

eKey EventInputKeyboard::GetKey() const
{
	return nKey;
}

eModifier EventInputKeyboard::GetModifier() const
{
	return nModifier;
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
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::Shift)) > 0;
}

bool EventInputKeyboard::IsShiftLeftPressed() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::ShiftLeft)) > 0;
}

bool EventInputKeyboard::IsShiftRightPressed() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::ShiftRight)) > 0;
}

bool EventInputKeyboard::IsAltPressed() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::Alt)) > 0;
}

bool EventInputKeyboard::IsAltLeftPressed() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::AltLeft)) > 0;
}

bool EventInputKeyboard::IsAltRightPressed() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::AltRight)) > 0;
}

bool EventInputKeyboard::IsControlPressed() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::Control)) > 0;
}

bool EventInputKeyboard::IsControlLeftPressed() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::ControlLeft)) > 0;
}

bool EventInputKeyboard::IsControlRightPressed() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::ControlRight)) > 0;
}

bool EventInputKeyboard::IsMetaPressed() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::Meta)) > 0;
}

bool EventInputKeyboard::IsMetaLeftPressed() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::MetaLeft)) > 0;
}

bool EventInputKeyboard::IsMetaRightPressed() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::MetaRight)) > 0;
}

bool EventInputKeyboard::IsCapsLockOn() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::CapsLock)) > 0;
}

bool EventInputKeyboard::IsNumLockOn() const
{
	return (static_cast<u32>(nModifier) & static_cast<u32>(eModifier::NumLock)) > 0;
}

bool EventInputKeyboard::IsNumericPad() const
{
	return (static_cast<u32>(nKey) >= static_cast<u32>(eKey::Pad0) && static_cast<u32>(nKey) <= static_cast<u32>(eKey::PadEquals));
}

bool EventInputKeyboard::IsCharacter() const
{
	auto k = Key(nKey);
	return k.IsCharacter();
}

bool EventInputKeyboard::IsNumber() const
{
	auto k = Key(nKey);
	return k.IsNumber();
}

bool EventInputKeyboard::IsLetter() const
{
	auto k = Key(nKey);
	return k.IsLetter();
}

} // namespace

