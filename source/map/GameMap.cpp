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

namespace Seed {

ISceneObject *FactoryGameMap()
{
	return New(GameMap());
}

GameMap::GameMap()
	: vLayers()
	, ptMapSize(0, 0)
	, ptTileSize(0, 0)
{
	cMapLayers.SetParent(this);
}

GameMap::~GameMap()
{
	this->Unload();
}

bool GameMap::Load(Reader &reader, ResourceManager *res)
{
	SEED_ASSERT(res);

	bool ret = false;
	if (this->Unload())
	{
		ITransformable::Unserialize(reader);
		sName = reader.ReadString("name", "Map");
		String resource = reader.ReadString("sResource", "");
		if (resource != "")
		{
			File f(resource);
			Reader r(&f);
			ret = this->LoadTiled(r, res);
		}
		else
		{
			reader.SelectNode("cGameMap");
			ret = this->LoadTiled(reader, res);
		}
	}

	return ret;
}

bool GameMap::LoadTiled(Reader &reader, ResourceManager *res)
{
	ptMapSize.x = reader.ReadU32("width", 0);
	ptMapSize.y = reader.ReadU32("height", 0);
	ptTileSize.x = reader.ReadU32("tilewidth", 32);
	ptTileSize.y = reader.ReadU32("tileheight", 32);

//	String ori = reader.ReadString("orientation", "");

	u32 tileSetCount = reader.SelectArray("tilesets");
	for (u32 i = 0; i < tileSetCount; i++)
	{
		reader.SelectNext();

		TileSet *set = New(TileSet());
		set->Load(reader, res);

		vTileSets += set;
	}
	reader.UnselectArray();

	u32 layerCount = reader.SelectArray("layers");
	for (u32 i = 0; i < layerCount; i++)
	{
		reader.SelectNext();

		u32 layerId = 0;
		String type = reader.ReadString("type", "tilelayer");
		if (type == "tilelayer")
		{
			layerId = this->AddLayerTiled();
			MapLayerTiled *tiled = vLayers[layerId]->AsTiled();
			if (tiled)
			{
				tiled->SetTileSize(ptTileSize);

				u32 len = reader.SelectArray("data");
				tiled->SetTileData(reader, len);
				reader.UnselectArray();
				tiled->SetOpacity(reader.ReadF32("opacity", 1.0f));
				tiled->SetMapSize(Point2u(reader.ReadU32("width", 0), reader.ReadU32("height", 0)));
				tiled->sName = reader.ReadString("name", "TiledLayer-NoName");
				tiled->SetTileSet(vTileSets.at(0));
				tiled->SetPosition(-100, -100);
				tiled->Update(0.0f);
			}
		}
//		else if (type == "objectslayer")
//		{
//			layerId = this->AddLayerMetadata();
//			MapLayerMetadata *data = vLayers.at(layerId)->AsMetadata();
//			layer = data;
//			if (data)
//			{
//				const u32 *ptru = static_cast<const u32 *>((void *)&ptr[layers[i].iDataIndex]);
//				const LayerObjectHeader *ptrd = static_cast<const LayerObjectHeader *>((void *)&ptru[1]);
//				data->Initialize(ptiTileSize, ptru[0], ptrd);
//			}
//		}
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

	this->SetWidth(ptMapSize.x * ptTileSize.x);
	this->SetHeight(ptMapSize.y * ptTileSize.y);

	return true;
}

bool GameMap::Write(Writer &writer)
{
	#warning Implement map writing
	return true;
}

bool GameMap::Unload()
{
	cMapLayers.Unload();
	IMapLayerVector().swap(vLayers);

	TileSetVectorIterator it = vTileSets.begin();
	TileSetVectorIterator end = vTileSets.end();
	for (; it != end; ++it)
	{
		TileSet *obj = (*it);
		Delete(obj);
	}
	TileSetVector().swap(vTileSets);

	ptMapSize.x = 0;
	ptMapSize.y = 0;
	ptTileSize.x = 0;
	ptTileSize.y = 0;

	return true;
}

void GameMap::Reset()
{
	this->Unload();
}

void GameMap::Update(f32 dt)
{
	for (u32 i = 0; i < cMapLayers.Size(); i++)
	{
		cMapLayers.GetChildAt(i)->Update(dt);
	}
	this->UpdateTransform();
}

void GameMap::Render(const Matrix4f &worldTransform)
{
	for (u32 i = 0; i < cMapLayers.Size(); i++)
	{
		if (cMapLayers.GetChildAt(i)->IsVisible())
			cMapLayers.GetChildAt(i)->Render(worldTransform);
	}
}

u32 GameMap::AddLayerTiled()
{
	u32 layerId = vLayers.Size();

	MapLayerTiled *layer = New(MapLayerTiled());
	vLayers += layer;
	layer->bMarkForDeletion = true;
	cMapLayers.Add(layer);

	return layerId;
}

u32 GameMap::AddLayerMetadata()
{
	u32 layerId = vLayers.Size();

	MapLayerMetadata *layer = New(MapLayerMetadata());
	vLayers += layer;
	layer->bMarkForDeletion = true;
	cMapLayers.Add(layer);

	return layerId;
}

u32 GameMap::AddLayerMosaic()
{
	u32 layerId = vLayers.Size();

	MapLayerMosaic *layer = New(MapLayerMosaic());
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
	IMapLayer *map = NULL;

	IMapLayerVectorIterator it = vLayers.begin();
	IMapLayerVectorIterator end = vLayers.end();
	for (; it != end; ++it)
	{
		IMapLayer *obj = (*it);
		if (obj->sName == name)
		{
			map = obj;
			break;
		}
	}

	return map;
}

int GameMap::GetLayerCount()
{
	return vLayers.Size();
}

const String GameMap::GetClassName() const
{
	return "GameMap";
}

int GameMap::GetObjectType() const
{
	return Seed::TypeGameMap;
}


} // namespace
