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

MapLayerMetadata::MapLayerMetadata(Point2u tileSize)
	: pRes(NULL)
	, ptTileSize(tileSize)
{
}

MapLayerMetadata::~MapLayerMetadata()
{
	this->Reset();
}

void MapLayerMetadata::Reset()
{
//	IMetadataObjectVectorIterator it = vObjects.begin();
//	IMetadataObjectVectorIterator end = vObjects.end();
//	for (; it != end; ++it)
//	{
//		Delete(*it);
//	}

//	IMetadataObjectVector().swap(vObjects);

	IMapLayer::Unload(); // to clear them
	pRes = NULL;
}

bool MapLayerMetadata::Load(Reader &reader, ResourceManager *res)
{
	bool ret = false;

	if (this->Unload())
	{
		pRes = res;

		f32 h = reader.ReadF32("height", 0.0f);
		f32 w = reader.ReadF32("width", 0.0f);
		ptMapSize.x = h * ptTileSize.x;
		ptMapSize.y = w * ptTileSize.y;
		ptHalfSize.x = ptMapSize.x * 0.5f;
		ptHalfSize.y = ptMapSize.y * 0.5f;

		this->ReadMapLayer(reader);
		this->ReadProperties(reader);
		this->SetWidth(ptMapSize.x);
		this->SetHeight(ptMapSize.y);

		// overwrite readmaplayer x,y - I don't know if these values are used anyway
		this->SetPosition(-ptHalfSize.x - (ptTileSize.x * 0.5f), -ptHalfSize.y - (ptTileSize.y * 0.5f));

		u32 len = reader.SelectArray("objects");
		this->LoadData(reader, len);
		reader.UnselectArray();

		ret = true;
	}

	return ret;
}

void MapLayerMetadata::LoadData(Reader &reader, u32 len)
{
	for (u32 i = 0; i < len; i++)
	{
		reader.SelectNext();

		IMetadataObject *obj = New(IMetadataObject());
		obj->Load(reader, pRes);
		obj->bMarkForDeletion = true;
		this->Add(obj);
	}
}

void MapLayerMetadata::Render(const Matrix4f &worldTransform)
{
	ISceneObjectVectorIterator it = vChild.begin();
	ISceneObjectVectorIterator end = vChild.end();
	for (; it != end; ++it)
	{
		ISceneObject *obj = (*it);
		obj->Render(worldTransform);
	}
}

MapLayerMetadata *MapLayerMetadata::AsMetadata()
{
	return this;
}

} // namespace
