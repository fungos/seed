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
	: nBlendOperation(eBlendMode::None)
	, cColor(255, 255, 255, 255)
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
	nBlendOperation = eBlendMode::None;
	cColor = Color();
	bVisible = true;
	bColorChanged = false;
}

void IRenderable::Render(const mat4 &worldTransform)
{
	UNUSED(worldTransform);
	SEED_ABSTRACT_METHOD;
}

void IRenderable::Update(Seconds dt)
{
	UNUSED(dt);
	SEED_ABSTRACT_METHOD;
}

void IRenderable::SetBlending(eBlendMode op)
{
	nBlendOperation = op;
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
	cColor.r = u8(r);
	cColor.g = u8(g);
	cColor.b = u8(b);
	cColor.a = u8(a);
	bColorChanged = true;
}

void IRenderable::SetColor(f32 r, f32 g, f32 b, f32 a)
{
	cColor.r = static_cast<u8>(r * 255);
	cColor.g = static_cast<u8>(g * 255);
	cColor.b = static_cast<u8>(b * 255);
	cColor.a = static_cast<u8>(a * 255);
	bColorChanged = true;
}

void IRenderable::SetColor(Color px)
{
	cColor = px;
	bColorChanged = true;
}

Color IRenderable::GetColor() const
{
	return cColor;
}

void IRenderable::SetBlendingByName(const String &blending)
{
	String name = blending;

	nBlendOperation = eBlendMode::None;
	if (StringUtil::Equals(name, "merge"))
		nBlendOperation = eBlendMode::Merge;
	else if (StringUtil::Equals(name, "screen"))
		nBlendOperation = eBlendMode::Screen;
	else if (StringUtil::Equals(name, "overlay"))
		nBlendOperation = eBlendMode::Overlay;
	else if (StringUtil::Equals(name, "lighten"))
		nBlendOperation = eBlendMode::Lighten;
	else if (StringUtil::Equals(name, "decaloverlay"))
		nBlendOperation = eBlendMode::DecalOverlay;
	else if (StringUtil::Equals(name, "colordodge"))
		nBlendOperation = eBlendMode::ColorDodge;
	else if (StringUtil::Equals(name, "modulatealpha"))
		nBlendOperation = eBlendMode::ModulateAlpha;
	else if (StringUtil::Equals(name, "modulate"))
		nBlendOperation = eBlendMode::Modulate;
	else if (StringUtil::Equals(name, "additive"))
		nBlendOperation = eBlendMode::Additive;

	bColorChanged = true;
}

String IRenderable::GetBlendingName() const
{
	String ret = "None";
	switch (nBlendOperation)
	{
		case eBlendMode::Merge:			ret = "Merge";			break;
		case eBlendMode::Screen:		ret = "Screen";			break;
		case eBlendMode::Overlay:		ret = "Overlay";		break;
		case eBlendMode::Lighten:		ret = "Lighten";		break;
		case eBlendMode::DecalOverlay:	ret = "DecalOverlay";	break;
		case eBlendMode::ColorDodge:	ret = "ColorDodge";		break;
		case eBlendMode::ModulateAlpha:	ret = "ModulateAlpha";	break;
		case eBlendMode::Modulate:		ret = "Modulate";		break;
		case eBlendMode::Additive:		ret = "Additive";		break;
		case eBlendMode::Default:
		case eBlendMode::None:
		default:
		break;
	}

	return ret;
}

void IRenderable::Unserialize(Reader &reader)
{
	bVisible = reader.ReadBool("bVisible", bVisible);
	if (reader.SelectNode("cColor"))
	{
		cColor.r = u8(reader.ReadU32("r", cColor.r));
		cColor.g = u8(reader.ReadU32("g", cColor.g));
		cColor.b = u8(reader.ReadU32("b", cColor.b));
		cColor.a = u8(reader.ReadU32("a", cColor.a));
		reader.UnselectNode();
	}

	String blending = reader.ReadString("sBlending", "");
	if (blending != "")
		this->SetBlendingByName(blending);
}

void IRenderable::Serialize(Writer &writer)
{
	if (nBlendOperation != eBlendMode::Default && nBlendOperation != eBlendMode::None)
		writer.WriteString("sBlending", this->GetBlendingName().c_str());

	writer.OpenNode("cColor");
		writer.WriteU32("r", cColor.r);
		writer.WriteU32("g", cColor.g);
		writer.WriteU32("b", cColor.b);
		writer.WriteU32("a", cColor.a);
	writer.CloseNode();
}

} // namespace
