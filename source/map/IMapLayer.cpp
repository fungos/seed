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

#include "map/IMapLayer.h"

namespace Seed {

IMapLayer::IMapLayer()
	: mProperties()
	, fOpacity(1.0f)
{
}

IMapLayer::~IMapLayer()
{
	Map<String, String>().swap(mProperties);
}

void IMapLayer::ReadProperties(Reader &reader)
{
	if (reader.SelectNode("properties"))
	{
		u32 k = 0;
		while (1)
		{
			const char *key = reader.GetKey(k++);
			if (!key)
				break;

			mProperties[key] = reader.ReadString(key, "");
		}
		reader.UnselectNode();
	}
}

void IMapLayer::WriteProperties(Writer &writer)
{
	writer.OpenNode("properties");
	for (auto &kv : mProperties)
		writer.WriteString((kv.first).c_str(), (kv.second).c_str());
	writer.CloseNode();
}

void IMapLayer::ReadMapLayer(Reader &reader)
{
	sName = reader.ReadString("name", sName.c_str());
	this->SetVisible(reader.ReadBool("visible", this->IsVisible()));
	this->SetOpacity(reader.ReadF32("opacity", this->GetOpacity()));
	this->SetPosition(reader.ReadF32("x", this->GetX()), reader.ReadF32("y", this->GetY()));
}

void IMapLayer::WriteMapLayer(Writer &writer)
{
	writer.WriteString("name", sName.c_str());
	writer.WriteBool("visible", this->IsVisible());
	writer.WriteF32("opacity", this->GetOpacity());
	writer.WriteF32("x", this->GetX());
	writer.WriteF32("y", this->GetY());
}

const String &IMapLayer::GetProperty(const String &property) const
{
	return mProperties.at(property);
}

MapLayerTiled *IMapLayer::AsTiled()
{
	return nullptr;
}

MapLayerMosaic *IMapLayer::AsMosaic()
{
	return nullptr;
}

MapLayerMetadata *IMapLayer::AsMetadata()
{
	return nullptr;
}

void IMapLayer::SetOpacity(f32 opacity)
{
	fOpacity = opacity;
}

f32 IMapLayer::GetOpacity() const
{
	return fOpacity;
}

} // namespace
