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
#include "Memory.h"

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
	this->Unload();
}

void MapLayerMosaic::Initialize(uvec2 tileSize, u32 count, const LayerMosaicHeader *data)
{
	ptiTileSize = tileSize;
	for (u32 i = 0; i < count; i++)
	{
		vObjects += this->CreateSprite(&data[i], i);
	}
}

bool MapLayerMosaic::Unload()
{
	for (auto obj: vObjects)
		sdDelete(obj);

	SpriteVector().swap(vObjects);
	cScene.Reset();

	return true;
}

void MapLayerMosaic::Set(Reader &reader)
{
	UNUSED(reader)
}

bool MapLayerMosaic::Write(Writer &writer)
{
	UNUSED(writer)
	WARNING(IMPL - MapLayerMosaic::Write(...))

	writer.OpenNode();
		writer.WriteString("type", "mosaiclayer");
	writer.CloseNode();

	return false;
}

MapLayerMosaic *MapLayerMosaic::Clone() const
{
	auto obj = sdNew(MapLayerMosaic);
	obj->GenerateCloneName(sName);

	obj->ptiTileSize = ptiTileSize;
	obj->bWrap = bWrap;

	for (auto each: vObjects)
		obj->vObjects += static_cast<Sprite *>(each->Clone());

	//for (u32 i = 0; i < cScene.Size(); i++)
	//	obj->cScene.Add(static_cast<ISceneObject *>(cScene.GetChildAt(i)->Clone()));
	for (auto each: cScene)
		obj->cScene += static_cast<ISceneObject *>(each->Clone()); // TODO: TEST

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

Sprite *MapLayerMosaic::CreateSprite(const LayerMosaicHeader *entry, u32 prio)
{
	UNUSED(entry)
	UNUSED(prio)
	WARNING(IMPL - MapLayerMosaic::CreateSprite(...) - Reimplementar)

	Sprite *obj = nullptr;

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

ivec2 MapLayerMosaic::ViewAt(ivec2 pos)
{
	cScene.SetPosition(-pos.x / static_cast<f32>(pScreen->GetWidth()), -pos.y / static_cast<f32>(pScreen->GetHeight()));
	return ivec2(0, 0);
}

void MapLayerMosaic::Update(Seconds dt)
{
	cScene.Update(dt);
}

void MapLayerMosaic::Render(const mat4 &worldTransform)
{
	for (auto obj: vObjects)
		obj->Render(worldTransform);
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

ISceneObject *MapLayerMosaic::GetChildByName(const String &name) const
{
	return cScene.GetChildByName(name);
}

ISceneObject *MapLayerMosaic::GetChildAt(u32 i) const
{
	return cScene.GetChildAt(i);
}

} // namespace
