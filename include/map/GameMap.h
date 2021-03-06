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

#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__

#include "SceneNode.h"
#include "interface/IResource.h"
#include <glm/vec2.hpp>

namespace Seed {

class IMapLayer;
class TileSet;

SEED_DECLARE_CONTAINER(Vector, IMapLayer)
SEED_DECLARE_CONTAINER(Vector, TileSet)

ISceneObject *FactoryGameMap();

class SEED_CORE_API GameMap : public ISceneObject
{
	SEED_DISABLE_COPY(GameMap)
	SEED_DECLARE_RTTI(GameMap, ISceneObject)

	public:
		GameMap();
		virtual ~GameMap();

		IMapLayer *GetLayerAt(u32 index);
		IMapLayer *GetLayerByName(const String &name);

		u32 AddLayer(IMapLayer *layer);

		TileSet *GetTileSet(const String &name);
		void AddTileSet(TileSet *tileset);

		int GetLayerCount() const;
		const String GetProperty(const String &property) const;
		void SetProperty(const String &key, const String &value = "");

		void SetTileSize(uvec2 tileSize);
		void SetMapSize(uvec2 mapSize);

		// SceneNode
		virtual void Update(Seconds dt) override;
		virtual void Render(const mat4 &worldTransform) override;
		virtual void Reset() override; // call Unload

		// IDataObject
		virtual bool Write(Writer &writer) override;
		virtual bool Unload() override;
		virtual GameMap *Clone() const override;
		virtual void Set(Reader &reader) override;

	protected:
		void ReadProperties(Reader &reader);
		void WriteProperties(Writer &writer);

		u32 AddLayerTiled();
		u32 AddLayerMetadata(uvec2 tileSize);
		u32 AddLayerMosaic();
		bool LoadTiled(Reader &reader);
		bool WriteTiled(Writer &writer);

	protected:
		enum class eLayerType
		{
			Tiled,
			Metadata,
			Mosaic,
			Max
		};

		SceneNode cMapLayers;
		IMapLayerVector vLayers;
		TileSetVector vTileSets;
		Map<String, String> mProperties;

		uvec2 ptMapSize;
		uvec2 ptTileSize;

		bool bLoaded;
};

} // namespace

#endif // __MAP_H__
