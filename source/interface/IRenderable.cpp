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

#include "interface/IRenderable.h"
#include "Enum.h"
#include "Log.h"
#include <algorithm>

namespace Seed {

IRenderable::IRenderable()
	: eBlendOperation(BlendNone)
	, iColor(255, 255, 255, 255)
	, bColorChanged(false)
	, bVisible(true)
{
}

IRenderable::~IRenderable()
{
	this->Reset();
}

void IRenderable::Reset()
{
	eBlendOperation = BlendNone;
	iColor.pixel = 0;
	bVisible = true;
	bColorChanged = false;
}

void IRenderable::Render()
{
	SEED_ABSTRACT_METHOD;
}

void IRenderable::Update(f32 delta)
{
	UNUSED(delta);
	SEED_ABSTRACT_METHOD;
}

void IRenderable::SetBlending(eBlendMode op)
{
	eBlendOperation = op;
	bColorChanged = true;
}

void IRenderable::SetVisible(bool b)
{
	bVisible = b;
}

bool IRenderable::IsVisible() const
{
	return bVisible;
}

void IRenderable::SetColor(u32 r, u32 g, u32 b, u32 a)
{
	iColor.rgba.r = r;
	iColor.rgba.g = g;
	iColor.rgba.b = b;
	iColor.rgba.a = a;
	bColorChanged = true;
}

void IRenderable::SetColor(f32 r, f32 g, f32 b, f32 a)
{
	iColor.rgba.r = static_cast<u32>(r * 255);
	iColor.rgba.g = static_cast<u32>(g * 255);
	iColor.rgba.b = static_cast<u32>(b * 255);
	iColor.rgba.a = static_cast<u32>(a * 255);
	bColorChanged = true;
}

void IRenderable::SetColor(uPixel px)
{
	iColor = px;
	bColorChanged = true;
}

void IRenderable::SetColor(u32 px)
{
	iColor.pixel = px;
	bColorChanged = true;
}

u32 IRenderable::GetColor() const
{
	return iColor.pixel;
}

void IRenderable::SetBlendingByName(const String &blending)
{
	String name = blending;
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);

	eBlendOperation = BlendNone;
	if (name == "merge")
		eBlendOperation = BlendMerge;
	else if (name == "screen")
		eBlendOperation = BlendScreen;
	else if (name == "overlay")
		eBlendOperation = BlendOverlay;
	else if (name == "lighten")
		eBlendOperation = BlendLighten;
	else if (name == "decaloverlay")
		eBlendOperation = BlendDecalOverlay;
	else if (name == "colordodge")
		eBlendOperation = BlendColorDodge;
	else if (name == "modulatealpha")
		eBlendOperation = BlendModulateAlpha;
	else if (name == "modulate")
		eBlendOperation = BlendModulate;
	else if (name == "addition")
		eBlendOperation = BlendAdditive;

	bColorChanged = true;
}

String IRenderable::GetBlendingName() const
{
	String ret = "None";
	switch (eBlendOperation)
	{
		case BlendMerge:		ret = "Merge";			break;
		case BlendScreen:		ret = "Screen";			break;
		case BlendOverlay:		ret = "Overlay";		break;
		case BlendLighten:		ret = "Lighten";		break;
		case BlendDecalOverlay:	ret = "DecalOverlay";	break;
		case BlendColorDodge:	ret = "ColorDodge";		break;
		case BlendModulateAlpha:ret = "ModulateAlpha";	break;
		case BlendModulate:		ret = "Modulate";		break;
		case BlendAdditive:		ret = "Additive";		break;
		case BlendDefault:
		case BlendNone:
		default:
		break;
	}

	return ret;
}

} // namespace
