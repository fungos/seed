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

#ifndef __ENUM_H__
#define __ENUM_H__

namespace Seed {

enum class eProjection
{
	Orthogonal,
	Perspective
};

enum class eShutdownReason
{
	None,
	CloseRequested
};

enum class eLanguage
{
	en_US,
	pt_BR,
	es_ES,
	de_DE,
	ja_JP,
	fr_FR,
	cn_CN,
	Maximum
};

enum class eReaderType
{
	Default,
	Json = Default
};

enum class eRendererDeviceType
{
	Auto,
	OpenGLES1,
	OpenGL1x,
	OpenGL2x,
	OpenGL3x,
	OpenGL4x,
	OpenGLAny,
	DirectX8,
	DirectX9,
	DirectX10,
	DirectX11,
	DirectXAny
};

enum class eTextureCompression
{
	None,
	RGB_PVRTC_2BPPV1
};

enum class eModifier
{
	None			= 0x0000,
	ShiftLeft		= 0x0001,
	ShiftRight		= 0x0002,
	Shift			= (ShiftLeft | ShiftRight),
	ControlLeft		= 0x0040,
	ControlRight	= 0x0080,
	Control			= (ControlLeft | ControlRight),
	AltLeft			= 0x0100,
	AltRight		= 0x0200,
	Alt				= (AltLeft | AltRight),
	MetaLeft		= 0x0400,
	MetaRight		= 0x0800,
	Meta			= (MetaLeft | MetaRight),
	NumLock			= 0x1000,
	CapsLock		= 0x2000,
	ScrollLock		= 0x4000
};

/* Yes, it is entirely based on SDL, until now it is the most complete Keyboard mapping I ever found in an API. So, get used to it. */
enum class eKey
{
	None			= 0,
	First			= 0,

	Backspace		= 8,
	Tab				= 9,
	Clear			= 12,
	Return			= 13,
	Enter			= 13,
	Pause			= 19,
	Escape			= 27,

	// Ascii table 1 to 1
	Space			= ' ',
	Exclam			= '!',
	QuoteDouble		= '"',
	NumberSign		= '#',
	Dollar			= '$',
	Percent			= '%',
	Ampersand		= '&',
	Apostrophe		= 39,
	ParenLeft		= '(',
	ParenRight		= ')',
	Asterisk		= '*',
	Plus			= '+',
	Comma			= ',',
	Minus			= '-',
	Period			= '.',
	Slash			= '/',
	Digit0			= '0',
	Digit1			= '1',
	Digit2			= '2',
	Digit3			= '3',
	Digit4			= '4',
	Digit5			= '5',
	Digit6			= '6',
	Digit7			= '7',
	Digit8			= '8',
	Digit9			= '9',
	Colon			= ':',
	Semicolon		= ';',
	Less			= '<',
	Equal			= '=',
	Greater			= '>',
	Question		= '?',
	At				= '@',
	// Both capital and lowercase chars are valid
	A				= 'A',
	B				= 'B',
	C				= 'C',
	D				= 'D',
	E				= 'E',
	F				= 'F',
	G				= 'G',
	H				= 'H',
	I				= 'I',
	J				= 'J',
	K				= 'K',
	L				= 'L',
	M				= 'M',
	N				= 'N',
	O				= 'O',
	P				= 'P',
	Q				= 'Q',
	R				= 'R',
	S				= 'S',
	T				= 'T',
	U				= 'U',
	V				= 'V',
	W				= 'W',
	X				= 'X',
	Y				= 'Y',
	Z				= 'Z',
	BracketLeft		= '[',
	Backslash		= 92,
	BracketRight	= ']',
	Circum			= '^',
	Underscore		= '_',
	QuoteLeft		= '`',
	// a-z 97-122
	Console			= QuoteLeft,
	BraceLeft		= '{',
	Bar				= '|',
	Pipe			= Bar,
	BraceRight		= '}',
	Tilde			= '~',

	Delete			= 127,

	Pad0			= 256,
	Pad1			= 257,
	Pad2			= 258,
	Pad3			= 259,
	Pad4			= 260,
	Pad5			= 261,
	Pad6			= 262,
	Pad7			= 263,
	Pad8			= 264,
	Pad9			= 265,
	PadPeriod		= 266,
	PadDivide		= 267,
	PadMultiply		= 268,
	PadMinus		= 269,
	PadPlus			= 270,
	PadEnter		= 271,
	PadEquals		= 272,

	Up				= 273,
	Down			= 274,
	Right			= 275,
	Left			= 276,
	Insert			= 277,
	Home			= 278,
	End				= 279,
	PageUp			= 280,
	PageDown		= 281,

	F1				= 282,
	F2				= 283,
	F3				= 284,
	F4				= 285,
	F5				= 286,
	F6				= 287,
	F7				= 288,
	F8				= 289,
	F9				= 290,
	F10				= 291,
	F11				= 292,
	F12				= 293,
	F13				= 294,
	F14				= 295,
	F15				= 296,

	NumLock			= 300,
	CapsLock		= 301,
	ScrollLock		= 302,

	ShiftRight		= 303,
	ShiftLeft		= 304,
	ControlRight	= 305,
	ControlLeft		= 306,
	AltRight		= 307,
	AltLeft			= 308,
	MetaRight		= 309,
	MetaLeft		= 310,
	SuperLeft		= 311,		/* Left "Windows" Key */
	SuperRight		= 312,		/* Right "Windows" Key */
	AltGr			= 313,		/* "Mode" Key */
	Menu			= 314,		/* Menu Key */

	PrintScreen		= 316,

	Last
};

enum class eInputButton
{
	None = 0x0,
	Button0 = 0x01,
	Button1 = 0x02,
	Button2 = 0x04,
	Button3 = 0x08,
	Button4 = 0x10,
	Button5 = 0x20,
	Button6 = 0x40,
	Button7 = 0x80,
	Button8 = 0x100,
	Button9 = 0x200,
	Button10 = 0x400,
	Button11 = 0x800,
	Button12 = 0x1000,
	Button13 = 0x2000,
	Button14 = 0x4000,
	Button15 = 0x8000,
	Invalid = None,
	All = 0xFFFF,

	// From here it is just helper enums, may not be portable.
	// Mouse specific mapping
	MouseUp = Button0,
	MouseDown = Button1,
	MouseLeft = Button2,
	MouseRight = Button3,
	MouseMiddle = Button4,

	// Joystick specific mapping
	JoystickUp = Button4,
	JoystickRight = Button5,
	JoystickDown = Button6,
	JoystickLeft = Button7
};

/// Maximum size of the cartridge
/**
Its possible that some platforms does not support some cartridge sizes.
*/
enum class eCartridgeSize
{
	VeryTiny,		/*!< Cartridge has up to 512 bytes of data. */
	Tiny,			/*!< Cartridge has up to 8Kb of data. */
	Small,			/*!< Cartridge has up to 32Kb of data. */
	Normal,			/*!< Cartridge has up to 64Kb of data. */
	Large,			/*!< Cartridge has up to 256Kb bytes of data. */
	Unlimited		/*!< Cartridge has no limit size */
};

/// Possible cartridge return codes
/**
These are the possible error codes returned by the cartridge
*/
enum class eCartridgeError
{
	None,				/*!< No error ocurred. */
	NoCard,				/*!< Card is not present, can also mean that the device is not present in case of a removable device. */
	NotFormatted,		/*!< The card or device is not formatted. */
	DataCorrupt,		/*!< The data is corrupted. */
	FilesystemCorrupt,	/*!< The file system is corrupted, this is a fatal error. */
	DeviceFull,			/*!< Not enough space left on device. */
	InodeFull,			/*!< Not enough inodes left on device. */
	AccessDenied,		/*!< Failed to open a file, it could be busy or the user may not have permissions to open the file. */
	NotInitialized,		/*!< A function was called before the system was prepared or initialized to receive that call. */
	InvalidArgument		/*!< Invalid parameter supplied to a method. */
};

enum class eTextureFilterType
{
	Min,
	Mag
};

enum class eTextureFilter
{
	Linear,
	Nearest
};

enum class eMeshType
{
	Triangles,
	TriangleStrip,
	TriangleFan,
	Line,
	LineStrip,
	LineLoop,
	MeshTypeCount
};

/*
TODO: Remove all this and use shaders.
TODO: Rewrite blending logic to use Blending classes and use http://www.opengl.org/sdk/docs/man/xhtml/glBlendFuncSeparate.xml
TODO: Blending based on http://www.w3.org/TR/2009/WD-SVGCompositing-20090430/ and http://kobalicek.com/data/articles/Fog-Graphics.html (Compositing)

All operations following this will use these variables:

Variables:
	- C is the color component (RGB);
	- A is the alpha component;

Subscripts:
	- r is the rasterized output (result);
	- t is the texture source component input (pixel);
	- d is the texture destiny component input (pixel);
	- f is the color fragment intput (glColor*);

Base formula:
	Cr = Ct * paramSrc + Cd * paramDst
	Ar = At * paramSrc + Ad * paramDst

* REPLACE operations *

Just will replace the rasterized fragment with the newly computed one.

Blend None

Will blend the incoming texture with the background respecting the texture alpha
and not the background alfa.

Cr = Ct * At + Cr * 0
Ar = At * At + Ar * 0

* DECAL operations

Blend DecalOverlay

Cr = Ct * Cr + Cf * 1.0f
Ar = At * Ar + Af * 1.0f

* BLEND operations *

All operations from here are blending between two textures:
- the source texture (currently selected texture);
- the destiny texture or rasterized texture (whatever is in the framebuffer / background);


Blend Default

Cr = Ct * 1.0f + Cd * 1.0f = Ct + Cd
Ar = At * 1.0f + Ad * 1.0f = At + Ad


Blend Merge

It's an average between source and destiny (Morpho's screen):

Cr = Ct * At + Cd * Ad
Ar = At * At + Ad * Ad (this is correct?)


Blend Screen

This is based in this article:
http://gmc.yoyogames.com/index.php?s=321b708f77c5e17d0fca772ef7dcd6f9&showtopic=254433&st=0

Where it try to simulate photo shop screen blending.

Cr = Ct + Cd * (1.0f - Ct)
Ar = At + Ad * (1.0f - At)


* MODULATE operations *

Modulate is applied based in a Texture Pixel with a Color Fragment (glColor*) based
in the following formula:


Blend Overlay

Cr = Ct * Cr + Cf * 1.0f
Ar = At * Ar + Af * 1.0f


Blend Lighten

Cr = Ct * (1.0f - At) + Cf * (1.0f - At)
Ar = At * (1.0f - At) + Af * (1.0f - At)


Blend ColorDodge

Cr = Ct + Cf
Ar = At + Af


Blend ModulateAlpha

Cr = Ct * At +  1 * (1.0f - At)
Ar = At * At + Af * (1.0f - At)


Blend Modulate

Cr = Ct * At + Cf * (1.0f - At)
Ar = At * At + Af * (1.0f - At)


Blend Additive

Cr = Ct * At + Cd
Ar = At * At + Ad

*/
enum class eBlendMode
{
	None = 0,
	Default,
	Merge,
	Screen,
	Overlay,
	Lighten,
	ColorDodge,
	DecalOverlay,
	ModulateAlpha,
	Modulate,
	Additive
};

enum class eBufferTarget
{
	Array,
	ElementArray,
	Count
};

enum class eBufferUsage
{
	NeverChange,
	WillChange,
	EveryFrameChange,
	Count
};

enum class eElementType
{
	Byte,
	Short,
	Int,
	Count
};

enum class ePlayableState
{
	Stopped,
	Paused,
	Playing
};

enum class eJobState
{
	Stopped,
	Running,
	Aborted,
	Completed
};

} // namespace

#endif // __ENUM_H__
