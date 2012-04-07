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

#include "interface/IInputKeyboard.h"
#include "interface/IEventInputKeyboardListener.h"
#include "Log.h"

namespace Seed {

#if defined(DEBUG)
const char *keyName[] = {
	"KeyNone", // 0
	"", // 1
	"", // 2
	"", // 3
	"", // 4
	"", // 5
	"", // 6
	"", // 7
	"KeyBackspace",
	"KeyTab",
	"", // 10
	"", // 11
	"KeyClear",
	"KeyReturn/KeyEnter",
	"", // 14
	"", // 15
	"", // 16
	"", // 17
	"", // 18
	"KeyPause",
	"", // 20
	"", // 21
	"", // 22
	"", // 23
	"", // 24
	"", // 25
	"", // 26
	"KeyEscape",
	"", // 28
	"", // 29
	"", // 30
	"", // 31
	// Ascii table 1 to 1
	"KeySpace",
	"KeyExclam",
	"KeyQuoteDouble",
	"KeyNumberSign",
	"KeyDollar",
	"KeyPercent",
	"KeyAmpersand",
	"KeyApostrophe",
	"KeyParenLeft",
	"KeyParenRight",
	"KeyAsterisk",
	"KeyPlus",
	"KeyComma",
	"KeyMinus",
	"KeyPeriod",
	"KeySlash",
	"Key0",
	"Key1",
	"Key2",
	"Key3",
	"Key4",
	"Key5",
	"Key6",
	"Key7",
	"Key8",
	"Key9",
	"KeyColon",
	"KeySemicolon",
	"KeyLess",
	"KeyEqual",
	"KeyGreater",
	"KeyQuestion",
	"", // @ 64
	// Both capital and lowercase chars are valid
	"KeyA",
	"KeyB",
	"KeyC",
	"KeyD",
	"KeyE",
	"KeyF",
	"KeyG",
	"KeyH",
	"KeyI",
	"KeyJ",
	"KeyK",
	"KeyL",
	"KeyM",
	"KeyN",
	"KeyO",
	"KeyP",
	"KeyQ",
	"KeyR",
	"KeyS",
	"KeyT",
	"KeyU",
	"KeyV",
	"KeyW",
	"KeyX",
	"KeyY",
	"KeyZ",

	"KeyBracketLeft",
	"KeyBackslash",
	"KeyBracketRight",
	"KeyCircum",
	"KeyUnderscore",
	"KeyQuoteLeft/KeyConsole",

	"KeyA", // a 97
	"KeyB",
	"KeyC",
	"KeyD",
	"KeyE",
	"KeyF",
	"KeyG",
	"KeyH",
	"KeyI",
	"KeyJ",
	"KeyK",
	"KeyL",
	"KeyM",
	"KeyN",
	"KeyO",
	"KeyP",
	"KeyQ",
	"KeyR",
	"KeyS",
	"KeyT",
	"KeyU",
	"KeyV",
	"KeyW",
	"KeyX",
	"KeyY",
	"KeyZ", // z 122

	"KeyBraceLeft",
	"KeyBar/KeyPipe",
	"KeyBraceRight",
	"KeyTilde",

	"KeyDelete", // 127

	"", // 128
	"",
	"", // 130
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 140
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 150
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 160
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 170
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 180
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 190
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 200
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 210
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 220
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 230
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 240
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"", // 250
	"",
	"",
	"",
	"",
	"",

	"KeyPad0", // 256
	"KeyPad1",
	"KeyPad2",
	"KeyPad3",
	"KeyPad4",
	"KeyPad5",
	"KeyPad6",
	"KeyPad7",
	"KeyPad8",
	"KeyPad9",
	"KeyPadPeriod",
	"KeyPadDivide",
	"KeyPadMultiply",
	"KeyPadMinus",
	"KeyPadPlus",
	"KeyPadEnter",
	"KeyPadEquals",

	"KeyUp",
	"KeyDown",
	"KeyRight",
	"KeyLeft",
	"KeyInsert",
	"KeyHome",
	"KeyEnd",
	"KeyPageUp",
	"KeyPageDown",

	"KeyF1",
	"KeyF2",
	"KeyF3",
	"KeyF4",
	"KeyF5",
	"KeyF6",
	"KeyF7",
	"KeyF8",
	"KeyF9",
	"KeyF10",
	"KeyF11",
	"KeyF12",
	"KeyF13",
	"KeyF14",
	"KeyF15",

	"", // 297
	"", // 298
	"", // 299

	"KeyNumLock", // 300
	"KeyCapsLock",
	"KeyScrollLock",

	"KeyShiftRight",
	"KeyShiftLeft",
	"KeyControlRight",
	"KeyControlLeft",
	"KeyAltRight",
	"KeyAltLeft",
	"KeyMetaRight",
	"KeyMetaLeft",
	"KeySuperLeft",
	"KeySuperRight",
	"KeyAltGr",
	"KeyMenu",
	"", // 315

	"KeyPrintScreen",

};
#endif


IInputKeyboard::IInputKeyboard()
	: vKeyboardListeners()
{
}

IInputKeyboard::~IInputKeyboard()
{
	IEventInputKeyboardListenerVector().swap(vKeyboardListeners);
}

void IInputKeyboard::AddKeyboardListener(IEventInputKeyboardListener *listener)
{
	vKeyboardListeners += listener;
}

void IInputKeyboard::RemoveKeyboardListener(IEventInputKeyboardListener *listener)
{
	vKeyboardListeners -= listener;
}

void IInputKeyboard::SendEventKeyboardPress(const EventInputKeyboard *ev)
{
	SEED_ASSERT(ev);

#if defined(DEBUG)
	Dbg(">>>> Key Press: %s Modifier: 0x%04x", keyName[ev->GetKey().GetValue()], ev->GetModifier());
#endif

	IEventInputKeyboardListenerVectorIterator it = vKeyboardListeners.begin();
	IEventInputKeyboardListenerVectorIterator end = vKeyboardListeners.end();
	for (; it != end; ++it)
	{
		(*it)->OnInputKeyboardPress(ev);

		if (ev->IsConsumed())
			break;
	}
}

void IInputKeyboard::SendEventKeyboardRelease(const EventInputKeyboard *ev)
{
	SEED_ASSERT(ev);

#if defined(DEBUG)
	Dbg(">>>> Key Release: %s Modifier: 0x%04x", keyName[ev->GetKey().GetValue()], ev->GetModifier());
#endif

	IEventInputKeyboardListenerVectorIterator it = vKeyboardListeners.begin();
	IEventInputKeyboardListenerVectorIterator end = vKeyboardListeners.end();
	for (; it != end; ++it)
	{
		(*it)->OnInputKeyboardRelease(ev);

		if (ev->IsConsumed())
			break;
	}
}

} // namespace
