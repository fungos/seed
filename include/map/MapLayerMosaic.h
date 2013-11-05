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

#ifndef __MAPLAYERMOSAIC_H__
#define __MAPLAYERMOSAIC_H__

#include "map/IMapLayer.h"
#include "Point.h"

namespace Seed {

class Sprite;

SEED_DECLARE_CONTAINER(Vector, Sprite)

struct LayerMosaicHeader {

};

class SEED_CORE_API MapLayerMosaic : public IMapLayer
{
	SEED_DISABLE_COPY(MapLayerMosaic)

	public:
		MapLayerMosaic();
		virtual ~MapLayerMosaic();

		virtual void Initialize(Point2u tileSize, u32 count, const LayerMosaicHeader *data);
		virtual void Reset();

		virtual Point2i ViewAt(Point2i pos);

		virtual void SetWrap(bool b);
		virtual bool GetWrap() const;

		virtual u32 GetObjectCount() const;
		virtual Sprite *GetObject(u32 at);

		/// Called to instantiate each metadata object.
		/*! For each object in a metadata layer, this method will be called to construct
			a customized metadata object. You should override this method to instantiate your
			own custom objects. When overriding this method, do not call the base method, as
			it will instantiate a new object and may leak.

			\param entry a struct of Layer Object basic information.
			\return An instance of a custom object from MetadataObject type.
		*/
		virtual Sprite *CreateSprite(const LayerMosaicHeader *entry, u32 prio);

		// IMapLayer
		virtual MapLayerMosaic *AsMosaic();

		virtual void Add(ISceneObject *obj) override;
		virtual void Remove(ISceneObject *obj) override;
		virtual u32 Size() const override;
		virtual ISceneObject *GetChildByName(const String &name) const override;
		virtual ISceneObject *GetChildAt(u32 i) const override;

		// IRenderable
		virtual void Update(f32 delta);
		virtual void Render(const Matrix4f &worldTransform);

		// IDataObject
		virtual bool Unload() override;
		virtual bool Write(Writer &writer) override;
		virtual MapLayerMosaic *Clone() const override;
		virtual void Set(Reader &reader) override;

	private:
		SceneNode cScene;
		SpriteVector vObjects;
		Point2u ptiTileSize;
		bool bWrap : 1;
};

} // namespace

#endif // __MAPLAYERMOSAIC_H__
