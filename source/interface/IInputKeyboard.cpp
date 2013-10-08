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
const char *keyName[]
{
	[0] = "KeyNone",

	[8] = "KeyBackspace",
	[9] = "KeyTab",
	[12] = "KeyClear",
	[13] = "KeyReturn/KeyEnter",
	[19] = "KeyPause",
	[27] = "KeyEscape",

	// Ascii table 1 to 1
	[32] = "KeySpace",
	[33] = "KeyExclam",
	[34] = "KeyQuoteDouble",
	[35] = "KeyNumberSign",
	[36] = "KeyDollar",
	[37] = "KeyPercent",
	[38] = "KeyAmpersand",
	[39] = "KeyApostrophe",
	[40] = "KeyParenLeft",
	[41] = "KeyParenRight",
	[42] = "KeyAsterisk",
	[43] = "KeyPlus",
	[44] = "KeyComma",
	[45] = "KeyMinus",
	[46] = "KeyPeriod",
	[47] = "KeySlash",
	[48] = "Key0",
	[49] = "Key1",
	[50] = "Key2",
	[51] = "Key3",
	[52] = "Key4",
	[53] = "Key5",
	[54] = "Key6",
	[55] = "Key7",
	[56] = "Key8",
	[57] = "Key9",
	[58] = "KeyColon",
	[59] = "KeySemicolon",
	[60] = "KeyLess",
	[61] = "KeyEqual",
	[62] = "KeyGreater",
	[63] = "KeyQuestion",

	// Both capital and lowercase chars are valid
	[65] = "KeyA",
	[66] = "KeyB",
	[67] = "KeyC",
	[68] = "KeyD",
	[69] = "KeyE",
	[70] = "KeyF",
	[71] = "KeyG",
	[72] = "KeyH",
	[73] = "KeyI",
	[74] = "KeyJ",
	[75] = "KeyK",
	[76] = "KeyL",
	[77] = "KeyM",
	[78] = "KeyN",
	[79] = "KeyO",
	[80] = "KeyP",
	[81] = "KeyQ",
	[82] = "KeyR",
	[83] = "KeyS",
	[84] = "KeyT",
	[85] = "KeyU",
	[86] = "KeyV",
	[87] = "KeyW",
	[88] = "KeyX",
	[89] = "KeyY",
	[90] = "KeyZ",

	[91] = "KeyBracketLeft",
	[92] = "KeyBackslash",
	[93] = "KeyBracketRight",
	[94] = "KeyCircum",
	[95] = "KeyUnderscore",
	[96] = "KeyQuoteLeft/KeyConsole",

	[97] = "KeyA",
	[98] = "KeyB",
	[99] = "KeyC",
	[100] = "KeyD",
	[101] = "KeyE",
	[102] = "KeyF",
	[103] = "KeyG",
	[104] = "KeyH",
	[105] = "KeyI",
	[106] = "KeyJ",
	[107] = "KeyK",
	[108] = "KeyL",
	[109] = "KeyM",
	[110] = "KeyN",
	[111] = "KeyO",
	[112] = "KeyP",
	[113] = "KeyQ",
	[114] = "KeyR",
	[115] = "KeyS",
	[116] = "KeyT",
	[117] = "KeyU",
	[118] = "KeyV",
	[119] = "KeyW",
	[120] = "KeyX",
	[121] = "KeyY",
	[122] = "KeyZ",

	[123] = "KeyBraceLeft",
	[124] = "KeyBar/KeyPipe",
	[125] = "KeyBraceRight",
	[126] = "KeyTilde",

	[127] = "KeyDelete",

	[256] = "KeyPad0",
	[257] = "KeyPad1",
	[258] = "KeyPad2",
	[259] = "KeyPad3",
	[260] = "KeyPad4",
	[261] = "KeyPad5",
	[262] = "KeyPad6",
	[263] = "KeyPad7",
	[264] = "KeyPad8",
	[265] = "KeyPad9",
	[266] = "KeyPadPeriod",
	[267] = "KeyPadDivide",
	[268] = "KeyPadMultiply",
	[269] = "KeyPadMinus",
	[270] = "KeyPadPlus",
	[271] = "KeyPadEnter",
	[272] = "KeyPadEquals",

	[273] = "KeyUp",
	[274] = "KeyDown",
	[275] = "KeyRight",
	[276] = "KeyLeft",
	[277] = "KeyInsert",
	[278] = "KeyHome",
	[279] = "KeyEnd",
	[280] = "KeyPageUp",
	[281] = "KeyPageDown",

	[282] = "KeyF1",
	[283] = "KeyF2",
	[284] = "KeyF3",
	[285] = "KeyF4",
	[286] = "KeyF5",
	[287] = "KeyF6",
	[289] = "KeyF7",
	[290] = "KeyF8",
	[291] = "KeyF9",
	[292] = "KeyF10",
	[293] = "KeyF11",
	[294] = "KeyF12",
	[295] = "KeyF13",
	[296] = "KeyF14",
	[297] = "KeyF15",

	[300] = "KeyNumLock",
	[301] = "KeyCapsLock",
	[302] = "KeyScrollLock",

	[303] = "KeyShiftRight",
	[304] = "KeyShiftLeft",
	[305] = "KeyControlRight",
	[306] = "KeyControlLeft",
	[307] = "KeyAltRight",
	[308] = "KeyAltLeft",
	[309] = "KeyMetaRight",
	[310] = "KeyMetaLeft",
	[311] = "KeySuperLeft",
	[312] = "KeySuperRight",
	[313] = "KeyAltGr",
	[314] = "KeyMenu",

	[316] = "KeyPrintScreen"

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
