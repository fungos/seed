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
	"None", // 0
	"", // 1
	"", // 2
	"", // 3
	"", // 4
	"", // 5
	"", // 6
	"", // 7
	"Backspace",
	"Tab",
	"", // 10
	"", // 11
	"Clear",
	"Return/Enter",
	"", // 14
	"", // 15
	"", // 16
	"", // 17
	"", // 18
	"Pause",
	"", // 20
	"", // 21
	"", // 22
	"", // 23
	"", // 24
	"", // 25
	"", // 26
	"Escape",
	"", // 28
	"", // 29
	"", // 30
	"", // 31
	// Ascii table 1 to 1
	"Space",
	"Exclam",
	"QuoteDouble",
	"NumberSign",
	"Dollar",
	"Percent",
	"Ampersand",
	"Apostrophe",
	"ParenLeft",
	"ParenRight",
	"Asterisk",
	"Plus",
	"Comma",
	"Minus",
	"Period",
	"Slash",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Colon",
	"Semicolon",
	"Less",
	"Equal",
	"Greater",
	"Question",
	"", // @ 64
	// Both capital and lowercase chars are valid
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",

	"BracketLeft",
	"Backslash",
	"BracketRight",
	"Circum",
	"Underscore",
	"QuoteLeft/Console",

	"A", // a 97
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z", // z 122

	"BraceLeft",
	"Bar/Pipe",
	"BraceRight",
	"Tilde",

	"", // 127

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
	"Delete",
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

	"CapsLock",

	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",

	"PrintScreen",
	"ScrollLock",

	"Pause",
	"Insert",
	"Home",
	"PageUp",
	"", //275
	"End",
	"PageDown",

	"Right",
	"Left",
	"Down",
	"Up",

	"NumLock",
	"PadDivide",
	"PadMultiply",
	"PadMinus",
	"PadPlus",
	"PadEnter",

	"Pad1",
	"Pad2",
	"Pad3",
	"Pad4",
	"Pad5",
	"Pad6",
	"Pad7",
	"Pad8",
	"Pad9",
	"Pad0",

	"PadPeriod",
	"", //299
	"Application",
	"Power",
	"PadEquals",

	"F13",
	"F14",
	"F15",
	"F16",
	"F17",
	"F18",
	"F19",
	"F20",
	"F21",
	"F22",
	"F23",
	"F24",

	"Execute",
	"Help",
	"Menu",
	"Select",
	"Stop",
	"Again",
	"Undo",
	"Cut",
	"Copy",
	"Paste",
	"Find",
	"Mute",
	"VolumeUp",
	"VolumeDown",
	"", //329
	"", //330
	"", //331
	"PadComma",
	"PadEqualsAS400",

	"", //334
	"", //335
	"", //336
	"", //337
	"", //338
	"", //339
	"", //340
	"", //341
	"", //342
	"", //343
	"", //344
	"", //345
	"", //346
	"", //347
	"", // 348
	"", // 349
	"", // 350
	"", // 351

	"AalterAse",
	"SysReq",
	"Cancel",
	"Clear",
	"Prior",
	"Return2",
	"Separator",
	"Out",
	"Oper",
	"ClearAgain",
	"Crsel",
	"Exsel",

	"", //364
	"", //365
	"", //366
	"", //367
	"", //368
	"", //369
	"", //370
	"", //371
	"", //372
	"", //373
	"", //374

	"Pad00",
	"Pad000",

	"ThousandsSeparator",
	"DecimalSeparator",
	"CurrencyUnit",
	"CurrencySubunit",

	"PadLeftParen",
	"PadRightParen",
	"PadLeftBrace",
	"PadRightBrace",
	"PadTab",
	"PadBackspace",

	"PadA",
	"PadB",
	"PadC",
	"PadD",
	"PadE",
	"PadF",

	"PadXor",
	"PadPower",
	"PadPercent",
	"PadLess",
	"PadGreater",
	"PadAmpersand",
	"PadDBLampersand",
	"PadVerticalBar",
	"PadDBLVerticalBar",
	"PadCplon",
	"PadHash",
	"PadSpace",
	"PadAt",
	"PadExclam",
	"PadMemStore",
	"PadMemRecall",
	"PadMemClear",
	"PadMemAdd",
	"PadMemSubtract",
	"PadMemMultiply",
	"PadMemDivide",
	"PadPlusMinus",
	"PadClear",
	"PadClearEntry",
	"PadBinary",
	"PadOctal",
	"PadDecimal",
	"PadHexaDecimal",

	"", //421
	"", //422

	"LeftCtrl",
	"LeftShift",
	"LeftAlt",
	"LeftGui",
	"RightCtrl",
	"RightShift",
	"RightAlt",
	"RightGui",

	"", //431
	"", //432
	"", //433
	"", //434
	"", //435
	"", //436
	"", //437
	"", //438
	"", //439
	"", //440
	"", //441
	"", //442
	"", //443
	"", //444
	"", //445
	"", //446
	"", //447
	"", //448
	"", //449
	"", //450
	"", //451
	"", //452
	"", //453
	"", //454
	"", //455

	"Mode",
	"AudioNext",
	"AudioPrev",
	"AudioStop",
	"AudioPlay",
	"AudioMute",
	"MediaSelect",
	"Www",
	"Mail",
	"Calculator",
	"Computer",
	"AcSearch",
	"AcHome",
	"AcBack",
	"AcForward",
	"AcStop",
	"AcRefresh",
	"AcBookMarks",
	"BrightnessDown",
	"BrightnessUp",
	"DisplaySwitch",
	"KbDillumToggle",
	"KbDillumDown",
	"KbDillumUp",
	"Eject",
	"Sleep",

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
	Dbg(">>>> Key Press: %s Modifier: 0x%04x", keyName[static_cast<u32>(ev->GetKey())], ev->GetModifier());
#endif

	for (auto each: vKeyboardListeners)
	{
		each->OnInputKeyboardPress(ev);
		if (ev->IsConsumed())
			break;
	}
}

void IInputKeyboard::SendEventKeyboardRelease(const EventInputKeyboard *ev)
{
	SEED_ASSERT(ev);

#if defined(DEBUG)
	Dbg(">>>> Key Release: %s Modifier: 0x%04x", keyName[static_cast<u32>(ev->GetKey())], static_cast<u32>(ev->GetModifier()));
#endif

	for (auto each: vKeyboardListeners)
	{
		each->OnInputKeyboardRelease(ev);
		if (ev->IsConsumed())
			break;
	}
}

} // namespace
