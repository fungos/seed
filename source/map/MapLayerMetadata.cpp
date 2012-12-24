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
#include "map/IMetadataObject.h"
#include "Screen.h"

namespace Seed {

MapLayerMetadata::MapLayerMetadata()
	: vObjects()
	, ptiTileSize(0, 0)
{
	cScene.SetParent(this);
}

MapLayerMetadata::~MapLayerMetadata()
{
	this->Reset();
}

void MapLayerMetadata::Reset()
{
	IMetadataObjectVectorIterator it = vObjects.begin();
	IMetadataObjectVectorIterator end = vObjects.end();
	for (; it != end; ++it)
	{
		IMetadataObject *obj = (*it);
		Delete(obj);
	}

	IMetadataObjectVector().swap(vObjects);
	cScene.Reset();
}

void MapLayerMetadata::Initialize(Point2u tileSize, u32 count, const LayerObjectHeader *data)
{
	ptiTileSize = tileSize;
	for (u32 i = 0; i < count; i++)
	{
		vObjects += this->CreateObject(&data[i]);
	}
}

IMetadataObject *MapLayerMetadata::CreateObject(const LayerObjectHeader *entry)
{
	#warning FIXME
//	String type = ""; // Str(entry->iTypeId);
	IMetadataObject *obj = NULL;

//	bool physics = false;
//	if (type == "trigger")
//	{
//		obj = New(TriggerObject());
//	}
//	else if (type == "spawner")
//	{
//		obj = New(SpawnerObject());
//	}
//	else
//	{
//		obj = New(CollisionObject(world));
//		physics = true;
//	}

//	obj->SetNameId(entry->iNameId);
//	obj->SetTypeId(entry->iTypeId);
//	obj->LoadProperties(entry->iPropertiesId);
//	obj->SetPosition(entry->fPosX * ptiTileSize.x, entry->fPosY * ptiTileSize.y);
//	obj->SetWidth(entry->fWidth * ptiTileSize.x);
//	obj->SetHeight(entry->fHeight * ptiTileSize.y);

//	if (physics)
//	{
//		((CollisionObject*)obj)->CreateStaticBody(obj->GetX(), obj->GetY() + obj->GetHeight(), obj->GetWidth(), obj->GetHeight());
//	}

	cScene.Add(obj);

	return obj;
}

Point2i MapLayerMetadata::ViewAt(Point2i pos)
{
	cScene.SetPosition(-pos.x / static_cast<f32>(pScreen->GetWidth()), -pos.y / static_cast<f32>(pScreen->GetHeight()));

	return pos;
}

void MapLayerMetadata::Update(f32 dt)
{
	cScene.Update(dt);
}

void MapLayerMetadata::Render(const Matrix4f &worldTransform)
{
	IMetadataObjectVectorIterator it = vObjects.begin();
	IMetadataObjectVectorIterator end = vObjects.end();
	for (; it != end; ++it)
	{
		IMetadataObject *obj = (*it);
		obj->Render(worldTransform);
	}
}

MapLayerMetadata *MapLayerMetadata::AsMetadata()
{
	return this;
}

void MapLayerMetadata::Add(ISceneObject *obj)
{

}

void MapLayerMetadata::Remove(ISceneObject *obj)
{

}

u32 MapLayerMetadata::Size() const
{
	return cScene.Size();
}

ISceneObject *MapLayerMetadata::GetChildAt(u32 i)
{
	return cScene.GetChildAt(i);
}

} // namespace
