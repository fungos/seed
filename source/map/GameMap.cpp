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

#include "map/GameMap.h"
#include "map/IMapLayer.h"
#include "map/MapLayerTiled.h"
#include "map/MapLayerMetadata.h"
#include "map/MapLayerMosaic.h"
#include "map/TileSet.h"
#include "Screen.h"
#include "Memory.h"

namespace Seed {

ISceneObject *FactoryGameMap()
{
	return sdNew(GameMap);
}

GameMap::GameMap()
	: cMapLayers()
	, vLayers()
	, vTileSets()
	, mProperties()
	, ptMapSize(0, 0)
	, ptTileSize(32, 32)
	, bLoaded(false)
{
	cMapLayers.SetParent(this);
}

GameMap::~GameMap()
{
	this->Unload();
}

void GameMap::Set(Reader &reader)
{
	ITransformable::Unserialize(reader);

	sName = reader.ReadString("sName", sName.c_str());
	String resource = reader.ReadString("sResource", "");
	if (resource != "")
	{
		// FIXME: ASYNC
		File f(resource);
		Reader r(&f);
		this->LoadTiled(r);
	}
	else
	{
		if (reader.SelectNode("cGameMap"))
			this->LoadTiled(reader);
	}

	this->bLoaded = true;
}

bool GameMap::Write(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("sName", sName.c_str());
		ITransformable::Serialize(writer);

		writer.OpenNode("cGameMap");
			this->WriteTiled(writer);
		writer.CloseNode();
	writer.CloseNode();

	return true;
}

void GameMap::ReadProperties(Reader &reader)
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

void GameMap::WriteProperties(Writer &writer)
{
	writer.OpenNode("properties");
	for (auto &kv : mProperties)
		writer.WriteString((kv.first).c_str(), (kv.second).c_str());
	writer.CloseNode();
}

bool GameMap::WriteTiled(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("orientation", "orthogonal");
		writer.WriteU32("version", 1);
		writer.WriteU32("width", ptMapSize.x);
		writer.WriteU32("height", ptMapSize.y);
		writer.WriteU32("tilewidth", ptTileSize.x);
		writer.WriteU32("tileheight", ptTileSize.y);

		this->WriteProperties(writer);

		writer.OpenArray("tilesets");
		for (auto obj : vTileSets)
			obj->Write(writer);
		writer.CloseArray();

		writer.OpenArray("layers");
		for (auto obj : vLayers)
			obj->Write(writer);
		writer.CloseArray();

	writer.CloseNode();

	return true;
}

bool GameMap::LoadTiled(Reader &reader)
{
	ptMapSize.x = reader.ReadU32("width", ptMapSize.x);
	ptMapSize.y = reader.ReadU32("height", ptMapSize.y);
	ptTileSize.x = reader.ReadU32("tilewidth", ptTileSize.x);
	ptTileSize.y = reader.ReadU32("tileheight", ptTileSize.y);

//	String ori = reader.ReadString("orientation", "");
//	u32 version = reader.ReadU32("version", 1);

	this->ReadProperties(reader);

	u32 tileSetCount = reader.SelectArray("tilesets");
	for (u32 i = 0; i < tileSetCount; i++)
	{
		reader.SelectNext();

		auto set = sdNew(TileSet);
		set->Load(reader, pRes);

		vTileSets += set;
	}
	reader.UnselectArray();

	u32 layerCount = reader.SelectArray("layers");
	for (u32 i = 0; i < layerCount; i++)
	{
		reader.SelectNext();

		auto layerId = 0;
		auto type = String{reader.ReadString("type", "tilelayer")};
		if (type == "tilelayer")
		{
			layerId = this->AddLayerTiled();
			auto tiled = vLayers[layerId]->AsTiled();
			if (tiled)
			{
				tiled->Load(reader, pRes);
				tiled->SetTileSize(ptTileSize);
				tiled->SetTileSet(vTileSets.at(0));
				//tiled->SetPosition(-100, -100);
				tiled->Update(0.0f);
			}
		}
		else if (type == "objectgroup")
		{
			layerId = this->AddLayerMetadata(ptTileSize);
			auto data = vLayers[layerId]->AsMetadata();
			if (data)
			{
				data->Load(reader, pRes);
			}
		}
//		else if (type == "mosaiclayer")
//		{
//			layerId = this->AddLayerMosaic();
//			MapLayerMosaic *mosaic = arLayers[layerId]->AsMosaic();
//			layer = mosaic;
//			if (mosaic)
//			{
//				const u32 *ptru = static_cast<const u32 *>((void *)&ptr[layers[i].iDataIndex]);
//				const LayerMosaicHeader *ptrd = static_cast<const LayerMosaicHeader *>((void *)&ptru[1]);
//				mosaic->SetWrap(TRUE);
//				mosaic->Initialize(ptiTileSize, ptru[0], ptrd);
//			}
//		}
	}
	reader.UnselectArray();

	this->SetWidth(f32(ptMapSize.x * ptTileSize.x));
	this->SetHeight(f32(ptMapSize.y * ptTileSize.y));

	return true;
}

bool GameMap::Unload()
{
	cMapLayers.Unload();
	IMapLayerVector().swap(vLayers);

	for (auto obj: vTileSets)
		sdDelete(obj);

	TileSetVector().swap(vTileSets);
	mProperties.clear();

	ptMapSize = Point2u(0, 0);
	ptTileSize = Point2u(32, 32);
	sName = this->GetTypeName();

	bLoaded = false;

	return true;
}

void GameMap::Reset()
{
	this->Unload();
}

GameMap *GameMap::Clone() const
{
	auto obj = sdNew(GameMap);
	obj->GenerateCloneName(sName);

	obj->ptMapSize = ptMapSize;
	obj->ptTileSize = ptTileSize;
	obj->mProperties = mProperties;

	for (auto set: vTileSets)
		obj->vTileSets += set->Clone();

	for (auto layer: vLayers)
	{
		auto l = static_cast<IMapLayer *>(layer->Clone());

		obj->vLayers += l;
		obj->cMapLayers.Add(l);
	}

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
	obj->bLoaded = true;

	return obj;
}

void GameMap::Update(Seconds dt)
{
	for (decltype(cMapLayers.Size()) i = 0; i < cMapLayers.Size(); i++)
	{
		cMapLayers.GetChildAt(i)->Update(dt);
	}
	this->UpdateTransform();
}

void GameMap::Render(const Matrix4f &worldTransform)
{
	for (decltype(cMapLayers.Size()) i = 0; i < cMapLayers.Size(); i++)
	{
		if (cMapLayers.GetChildAt(i)->IsVisible())
			cMapLayers.GetChildAt(i)->Render(worldTransform);
	}
}

u32 GameMap::AddLayer(IMapLayer *layer)
{
	auto layerId = vLayers.Size();

	vLayers += layer;
	layer->bMarkForDeletion = true;
	cMapLayers.Add(layer);

	return layerId;
}

u32 GameMap::AddLayerTiled()
{
	auto layerId = vLayers.Size();

	auto layer = sdNew(MapLayerTiled);
	vLayers += layer;
	layer->bMarkForDeletion = true;
	cMapLayers.Add(layer);

	return layerId;
}

u32 GameMap::AddLayerMetadata(Point2u tileSize)
{
	auto layerId = vLayers.Size();

	Point2f size(f32(tileSize.x), f32(tileSize.y));
	auto layer = sdNew(MapLayerMetadata);
	layer->SetTileSize(size);

	vLayers += layer;
	layer->bMarkForDeletion = true;
	cMapLayers.Add(layer);

	return layerId;
}

u32 GameMap::AddLayerMosaic()
{
	auto layerId = vLayers.Size();

	auto layer = sdNew(MapLayerMosaic);
	vLayers += layer;
	layer->bMarkForDeletion = true;
	cMapLayers.Add(layer);

	return layerId;
}

IMapLayer *GameMap::GetLayerAt(u32 index)
{
	return vLayers.at(index);
}

IMapLayer *GameMap::GetLayerByName(const String &name)
{
	IMapLayer *map = nullptr;

	for (auto obj: vLayers)
	{
		if (obj->sName == name)
		{
			map = obj;
			break;
		}
	}

	return map;
}

void GameMap::AddTileSet(TileSet *tileset)
{
	vTileSets += tileset;
}

TileSet *GameMap::GetTileSet(const String &name)
{
	TileSet *ts = nullptr;

	for (auto obj: vTileSets)
	{
		if (obj->sName == name)
		{
			ts = obj;
			break;
		}
	}

	return ts;
}

int GameMap::GetLayerCount() const
{
	return vLayers.Size();
}

void GameMap::SetProperty(const String &key, const String &value)
{
	mProperties[key] = value;
}

const String GameMap::GetProperty(const String &property) const
{
	auto it = mProperties.find(property);
	return mProperties.end() == it ? "" : it->second;
}

void GameMap::SetTileSize(Point2u tileSize)
{
	SEED_ASSERT(!bLoaded);
	ptTileSize = tileSize;
}

void GameMap::SetMapSize(Point2u mapSize)
{
	SEED_ASSERT(!bLoaded);
	ptMapSize = mapSize;
}

} // namespace
