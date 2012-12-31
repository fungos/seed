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

#ifndef __EVENT_INPUT_KEYBOARD_H__
#define __EVENT_INPUT_KEYBOARD_H__

#include "interface/IEventInput.h"
#include "Key.h"

namespace Seed {

/// Keyboard Input Event
class SEED_CORE_API EventInputKeyboard : public IEventInput
{
	public:
		EventInputKeyboard(u32 key, u32 modifier, u32 scan, u32 keyboard = 0);
		virtual ~EventInputKeyboard();

		u32 GetScancode() const;
		const Key &GetKey() const;
		u32 GetModifier() const;
		u32	GetKeyboard() const;

		// Modifiers
		bool IsShiftPressed() const;
		bool IsShiftLeftPressed() const;
		bool IsShiftRightPressed() const;

		bool IsAltPressed() const;
		bool IsAltLeftPressed() const;
		bool IsAltRightPressed() const;

		bool IsControlPressed() const;
		bool IsControlLeftPressed() const;
		bool IsControlRightPressed() const;

		bool IsMetaPressed() const;
		bool IsMetaLeftPressed() const;
		bool IsMetaRightPressed() const;

		bool IsCapsLockOn() const;
		bool IsNumLockOn() const;

		bool IsNumericPad() const;

		// Utils
		//! Checks if this key is a character.
		/*!
			\return True if the key is a letter, a number or a whitespace, everything else returns false.
		 */
		bool IsCharacter() const;

		//! Checks if this key is a number.
		/*!
			\return True if the key is a a number from 0 to 9, everything else returns false.
		 */
		bool IsNumber() const;
		//! Checks if this key is a letter.
		/*!
			\return True if the key is a letter between a-z or A-Z, everything else returns false.
		 */
		bool IsLetter() const;

		// IObject
		virtual const String GetClassName() const override;

	protected:
		//u32 iKey;
		Key	cKey;
		u32 iModifier;
		u32 iScancode;
		u32 iKeyboard;

	private:
		SEED_DISABLE_COPY(EventInputKeyboard);
};

} // namespace

#endif // __EVENT_INPUT_KEYBOARD_H__
