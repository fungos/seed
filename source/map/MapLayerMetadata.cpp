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

#include "map/MapLayerMetadata.h"
#include "map/MetadataObject.h"
#include "Screen.h"
#include "Memory.h"

namespace Seed {

MapLayerMetadata::MapLayerMetadata()
	: pRes(nullptr)
	, ptTileSize(0.0f, 0.0f)
	, ptSize(0.0f, 0.0f)
	, ptMapSize(0.0f, 0.0f)
	, ptHalfSize(0.0f, 0.0f)
{
}

MapLayerMetadata::~MapLayerMetadata()
{
	this->Reset();
}

void MapLayerMetadata::Reset()
{
	this->Unload();
	pRes = nullptr;
}

bool MapLayerMetadata::Unload()
{
	IMapLayer::Unload();

	// ptTileSize = vec2(0.0f, 0.0f); // Do not clear tilesize.
	ptMapSize = vec2(0.0f, 0.0f);
	ptHalfSize = vec2(0.0f, 0.0f);

	return true;
}

void MapLayerMetadata::Set(Reader &reader)
{
	ptSize.x = reader.ReadF32("width", ptSize.x);
	ptSize.y = reader.ReadF32("height", ptSize.y);
	ptMapSize = ptSize * ptTileSize;
	ptHalfSize = ptMapSize * 0.5f;

	this->ReadMapLayer(reader);
	this->ReadProperties(reader);
	this->SetWidth(ptMapSize.x);
	this->SetHeight(ptMapSize.y);

	// ATTENTION: overwrite readmaplayer x,y - I don't know if these values are used anyway
	this->SetPosition(-ptHalfSize.x - (ptTileSize.x * 0.5f), -ptHalfSize.y - (ptTileSize.y * 0.5f));

	auto len = reader.SelectArray("objects");
	this->LoadData(reader, len);
	reader.UnselectArray();
}

bool MapLayerMetadata::Write(Writer &writer)
{
	// Write should ignore parent position
	auto oldPos = this->GetPosition();
	this->SetPosition(0.0f, 0.0f, 0.0f);

	writer.OpenNode();
		writer.WriteString("type", "objectgroup");

		this->WriteMapLayer(writer);
		writer.WriteF32("width", ptSize.x);
		writer.WriteF32("height", ptSize.y);

		this->WriteProperties(writer);

		writer.OpenArray("objects");
			for (auto obj : vChild)
				obj->Write(writer);
		writer.CloseArray();
	writer.CloseNode();

	this->SetPosition(oldPos);

	return true;
}

MapLayerMetadata *MapLayerMetadata::Clone() const
{
	auto obj = sdNew(MapLayerMetadata);
	obj->GenerateCloneName(sName);

	obj->pRes = pRes;
	obj->ptTileSize = ptTileSize;
	obj->ptMapSize = ptMapSize;
	obj->ptHalfSize = ptHalfSize;

	// ISceneObject
	obj->bMarkForDeletion = true;

	// ITransformable
	obj->pParent = pParent;
	obj->mTransform = mTransform;
	obj->vPos = vPos;
	obj->vPivot = vPivot;
	obj->vTransformedPivot = vTransformedPivot;
	obj->vScale = vScale;
	obj->vBoundingBox = vBoundingBox;
	obj->fRotation = fRotation;
	obj->bTransformationChanged = bTransformationChanged;

	// IRenderable
	obj->nBlendOperation = nBlendOperation;
	obj->cColor = cColor;
	obj->bColorChanged = bColorChanged;
	obj->bVisible = bVisible;

	return obj;
}

void MapLayerMetadata::LoadData(Reader &reader, u32 len)
{
	for (u32 i = 0; i < len; i++)
	{
		reader.SelectNext();

		auto obj = sdNew(MetadataObject);
		obj->Load(reader, pRes);
		obj->bMarkForDeletion = true;
		this->Add(obj);
	}
}

void MapLayerMetadata::SetMapSize(vec2 mapSize)
{
	ptMapSize = mapSize;
	ptHalfSize = mapSize * 0.5f;
}

void MapLayerMetadata::SetTileSize(vec2 tileSize)
{
	ptTileSize = tileSize;
}

void MapLayerMetadata::Render(const mat4 &worldTransform)
{
	for (auto obj: vChild)
		obj->Render(worldTransform);
}

MapLayerMetadata *MapLayerMetadata::AsMetadata()
{
	return this;
}

} // namespace
