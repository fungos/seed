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
	OpenGLES2,
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

// http://msdn.microsoft.com/en-us/library/windows/desktop/ff476882(v=vs.85).aspx
enum class eShaderType
{
	Fetch,		//! Input Assembler
	Vertex,		//! Vertex Shader
	Hull,		//! Hull Shader
	Domain,		//! Domain Shader
	Geometry,	//! Geometry Shader
	Pixel		//! Fragment Shader
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

/* Yes, it is entirely based on SDL2, until now it is the most complete Keyboard mapping I ever found in an API. So, get used to it. */
enum class eKey
{
	None			= 0,
	First			= 0,

	Backspace		= 8,
	Tab				= 9,
	Return			= 13,
	Enter			= 13,
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

	Delete			= 177,

	CapsLock		= 256,

	F1				= 257,
	F2				= 258,
	F3				= 259,
	F4				= 260,
	F5				= 261,
	F6				= 262,
	F7				= 263,
	F8				= 264,
	F9				= 265,
	F10				= 266,
	F11				= 267,
	F12				= 268,

	PrintScreen		= 269,
	ScrollLock		= 270,

	Pause			= 271,
	Insert			= 272,
	Home			= 273,
	PageUp			= 274,
	Blank1			= 275,
	End				= 276,
	PageDown		= 277,

	Right			= 278,
	Left			= 279,
	Down			= 280,
	Up				= 281,

	NumLock			= 282,
	PadDivide		= 283,
	PadMultiply		= 284,
	PadMinus		= 285,
	PadPlus			= 286,
	PadEnter		= 287,

	Pad1			= 288,
	Pad2			= 289,
	Pad3			= 290,
	Pad4			= 291,
	Pad5			= 292,
	Pad6			= 293,
	Pad7			= 294,
	Pad8			= 295,
	Pad9			= 296,
	Pad0			= 297,

	PadPeriod		= 298,

	Blank2			= 299,
	Application		= 300,
	Power			= 301,
	PadEquals		= 302,

	F13				= 303,
	F14				= 304,
	F15				= 305,
	F16				= 306,
	F17				= 307,
	F18				= 308,
	F19				= 309,
	F20				= 310,
	F21				= 311,
	F22				= 312,
	F23				= 313,
	F24				= 314,

	Execute			= 315,
	Help			= 316,
	Menu			= 317,
	Select			= 318,
	Stop			= 319,
	Again			= 320,
	Undo			= 321,
	Cut				= 322,
	Copy			= 323,
	Paste			= 324,
	Find			= 325,
	Mute			= 326,
	VolumeUp		= 327,
	VolumeDown		= 328,

	Blank3			= 329,
	Blank4			= 330,
	Blank5			= 331,

	PadComma		= 332,
	PadEqualsAS400	= 333,

	Blank6			= 334,
	Blank7			= 335,
	Blank8			= 336,
	Blank9			= 337,
	Blank10			= 338,
	Blank11			= 339,
	Blank12			= 340,
	Blank13			= 341,
	Blank14			= 342,
	Blank15			= 343,
	Blank16			= 344,
	Blank17			= 345,
	Blank18			= 346,
	Blank19			= 347,
	Blank20			= 348,
	Blank21			= 349,
	Blank22			= 350,
	Blank23			= 351,

	AalterAse		= 352,
	SysReq			= 353,
	Cancel			= 354,
	Clear			= 355,
	Prior			= 356,
	Return2			= 357,
	Separator		= 358,
	Out				= 359,
	Oper			= 360,
	ClearAgain		= 361,
	Crsel			= 362,
	Exsel			= 363,

	Blank24			= 364,
	Blank25			= 365,
	Blank26			= 366,
	Blank27			= 367,
	Blank28			= 368,
	Blank39			= 369,
	Blank30			= 370,
	Blank31			= 371,
	Blank32			= 372,
	Blank33			= 373,
	Blank34			= 374,

	Pad00			= 375,
	Pad000			= 376,

	ThousandsSeparator	= 377,
	DecimalSeparator	= 378,
	CurrencyUnit		= 379,
	CurrencySubunit		= 380,

	PadLeftParen	= 381,
	PadRightParen	= 382,
	PadLeftBrace	= 383,
	PadRightBrace	= 384,
	PadTab			= 385,
	PadBackspace	= 386,

	PadA	= 387,
	PadB	= 388,
	PadC	= 389,
	PadD	= 390,
	PadE	= 391,
	PadF	= 392,

	PadXor				= 393,
	PadPower			= 394,
	PadPercent			= 395,
	PadLess				= 396,
	PadGreater			= 397,
	PadAmpersand		= 398,
	PadDBLampersand		= 399,
	PadVerticalBar		= 400,
	PadDBLVerticalBar	= 401,
	PadCplon			= 402,
	PadHash				= 403,
	PadSpace			= 404,
	PadAt				= 405,
	PadExclam			= 406,
	PadMemStore			= 407,
	PadMemRecall		= 408,
	PadMemClear			= 409,
	PadMemAdd			= 410,
	PadMemSubtract		= 411,
	PadMemMultiply		= 412,
	PadMemDivide		= 413,
	PadPlusMinus		= 414,
	PadClear			= 415,
	PadClearEntry		= 416,
	PadBinary			= 417,
	PadOctal			= 418,
	PadDecimal			= 419,
	PadHexaDecimal		= 420,

	Blank35		= 421,
	Blank36		= 422,

	LeftCtrl		= 423,
	LeftShift		= 424,
	LeftAlt			= 425,
	LeftGui			= 426,
	RightCtrl		= 427,
	RightShift		= 428,
	RightAlt		= 429,
	RightGui		= 430,

	Mode			= 456,
	AudioNext		= 457,
	AudioPrev		= 458,
	AudioStop		= 459,
	AudioPlay		= 460,
	AudioMute		= 461,
	MediaSelect		= 462,
	Www				= 463,
	Mail			= 464,
	Calculator		= 465,
	Computer		= 466,
	AcSearch		= 467,
	AcHome			= 468,
	AcBack			= 469,
	AcForward		= 470,
	AcStop			= 471,
	AcRefresh		= 472,
	AcBookMarks		= 473,
	BrightnessDown	= 474,
	BrightnessUp	= 475,
	DisplaySwitch	= 476,
	KbDillumToggle	= 477,
	KbDillumDown	= 478,
	KbDillumUp		= 479,
	Eject			= 480,
	Sleep			= 481,

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
