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

#include "map/MapLayerMosaic.h"
#include "Sprite.h"
#include "Screen.h"

namespace Seed {

MapLayerMosaic::MapLayerMosaic()
	: vObjects()
	, ptiTileSize(0, 0)
{
	cScene.SetParent(this);
}

MapLayerMosaic::~MapLayerMosaic()
{
	this->Reset();
}

void MapLayerMosaic::Reset()
{
	SpriteVectorIterator it = vObjects.begin();
	SpriteVectorIterator end = vObjects.end();
	for (; it != end; ++it)
	{
		Sprite *obj = (*it);
		Delete(obj);
	}

	SpriteVector().swap(vObjects);
	cScene.Reset();
}

void MapLayerMosaic::Initialize(Point2u tileSize, u32 count, const LayerMosaicHeader *data)
{
	ptiTileSize = tileSize;
	for (u32 i = 0; i < count; i++)
	{
		vObjects += this->CreateSprite(&data[i], i);
	}
}

Sprite *MapLayerMosaic::CreateSprite(const LayerMosaicHeader *entry, u32 prio)
{
	Sprite *obj = NULL;

	#warning FIXME
//	obj = New(SpriteInstantiable());
//	obj->Load(_F(entry->iSpriteFileId));
//	//obj->SetPosition(entry->fPosX * ptiTileSize.x, entry->fPosY * ptiTileSize.y);
//	obj->SetPosition(entry->fPosX, entry->fPosY);
//	obj->SetRotation(entry->fAngle);
//	obj->SetScale(entry->fScaleX, entry->fScaleY);
//	obj->SetPriority(prio);
//	obj->SetVisible(FALSE);
//	//obj->SetPriority(entry->iPriority);
////	obj->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
////	obj->SetBlending(Seed::BlendModulate);
//	// blending
//	// collision

//	cScene.Add(obj);

	return obj;
}

u32 MapLayerMosaic::GetObjectCount() const
{
	return vObjects.Size();
}

Sprite *MapLayerMosaic::GetObject(u32 at)
{
	return vObjects.at(at);
}

Point2i MapLayerMosaic::ViewAt(Point2i pos)
{
	cScene.SetPosition(-pos.x / static_cast<f32>(pScreen->GetWidth()), -pos.y / static_cast<f32>(pScreen->GetHeight()));

	return Point2i(0, 0);
}

void MapLayerMosaic::Update(f32 dt)
{
	cScene.Update(dt);
}

void MapLayerMosaic::Render(const Matrix4f &worldTransform)
{
	SpriteVectorIterator it = vObjects.begin();
	SpriteVectorIterator end = vObjects.end();
	for (; it != end; ++it)
	{
		Sprite *obj = (*it);
		obj->Render(worldTransform);
	}
}

void MapLayerMosaic::SetWrap(bool b)
{
	bWrap = b;
}

bool MapLayerMosaic::GetWrap() const
{
	return bWrap;
}

MapLayerMosaic *MapLayerMosaic::AsMosaic()
{
	return this;
}

void MapLayerMosaic::Add(ISceneObject *obj)
{
	cScene.Add(obj);
}

void MapLayerMosaic::Remove(ISceneObject *obj)
{
	cScene.Remove(obj);
}

u32 MapLayerMosaic::Size() const
{
	return cScene.Size();
}

ISceneObject *MapLayerMosaic::GetChildAt(u32 i)
{
	return cScene.GetChildAt(i);
}

} // namespace
