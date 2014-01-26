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

#include "map/MapLayerTiled.h"
#include "map/TileSet.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "Image.h"
#include "Memory.h"

#define TILE(x, y) pTileData[(x) + (ptMapSize.y * (y))]

namespace Seed {

MapLayerTiled::MapLayerTiled()
	: pTileData(nullptr)
	, pVertex(nullptr)
	, pElements(nullptr)
	, pTileSet(nullptr)
	, cVertexBuffer()
	, cElementBuffer()
	, iDataLen(0)
	, ptTileSize(0, 0)
	, ptMapSize(0, 0)
	, ptMapSizeHalf(0.0f, 0.0f)
	, bRebuildMesh(false)
{
	cVertexBuffer.Configure(eBufferUsage::NeverChange, eElementType::Int);
	cElementBuffer.Configure(eBufferUsage::NeverChange, eElementType::Int);
}

MapLayerTiled::~MapLayerTiled()
{
	pRendererDevice->DestroyHardwareBuffer(&cElementBuffer);
	pRendererDevice->DestroyHardwareBuffer(&cVertexBuffer);

	this->Unload();
}

void MapLayerTiled::Set(Reader &reader)
{
	u32 len = reader.SelectArray("data");
	this->LoadData(reader, len);
	reader.UnselectArray();

	// map size is in tiles and is only important to tile based maps, so we read it here
	this->SetMapSize(Point2u(reader.ReadU32("width", ptMapSize.x), reader.ReadU32("height", ptMapSize.y)));

	// read the generic map info in base class
	this->ReadMapLayer(reader);
	this->ReadProperties(reader);
}

bool MapLayerTiled::Write(Writer &writer)
{
	UNUSED(writer)
	WARNING(IMPL - MapLayerTiled::Writer(...))
	return false;
}

bool MapLayerTiled::Unload()
{
	sdFree(pElements);
	sdFree(pVertex);
	sdFree(pTileData);

	ptTileSize = Point2u(0, 0);
	ptMapSizeHalf = Point2f(0.0f, 0.0f);
	ptMapSize = Point2u(0, 0);
	pTileSet = nullptr;
	sName = this->GetTypeName();

	return IMapLayer::Unload();
}

MapLayerTiled *MapLayerTiled::Clone() const
{
	auto obj = sdNew(MapLayerTiled);
	obj->GenerateCloneName(sName);

	if (iDataLen)
	{
		obj->pTileData = (u32 *)sdAlloc(sizeof(u32) * iDataLen);
		memcpy(obj->pTileData, pTileData, sizeof(u32) * iDataLen);
	}

	u32 vertexAmount = ptMapSize.x * ptMapSize.y * 4;
	u32 elementAmount = ptMapSize.x * ptMapSize.y * 6;

	obj->pVertex = (sVertex *)sdAlloc(sizeof(sVertex) * vertexAmount);
	obj->pElements = (u32 *)sdAlloc(sizeof(u32) * elementAmount);

	memcpy(obj->pVertex, pVertex, sizeof(sVertex) * vertexAmount);
	memcpy(obj->pElements, pElements, sizeof(u32) * elementAmount);

	obj->pTileSet = pTileSet->Clone();
	obj->cVertexBuffer.SetData(obj->pVertex, cVertexBuffer.iLength);
	obj->cElementBuffer.SetData(obj->pElements, cElementBuffer.iLength);

	obj->iDataLen = iDataLen;
	obj->ptTileSize = ptTileSize;
	obj->ptMapSize = ptMapSize;
	obj->ptMapSizeHalf = ptMapSizeHalf;

	obj->bRebuildMesh = bRebuildMesh;
	obj->bResizeMap = bResizeMap;

	for (auto child: vChild)
	{
		auto cln = static_cast<ISceneObject *>(child->Clone());
		obj->vChild += cln;
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

	return obj;
}

void MapLayerTiled::LoadData(Reader &reader, u32 len)
{
	if (len > 0)
	{
		if (iDataLen != len)
		{
			sdFree(pTileData);
			pTileData = (u32 *)sdAlloc(sizeof(u32) * len);
		}

		for (u32 i = 0; i < len; i++)
		{
			reader.SelectNext();
			pTileData[i] = reader.GetU32(0);
		}
	}

	if (!len)
		sdFree(pTileData);

	iDataLen = len;
	bRebuildMesh = true;
}

void MapLayerTiled::Update(Seconds dt)
{
	UNUSED(dt)
	if (bRebuildMesh && pTileSet && pTileData)
	{
		this->SetWidth(static_cast<f32>(ptMapSize.x * ptTileSize.x));
		this->SetHeight(static_cast<f32>(ptMapSize.y * ptTileSize.y));

		u32 vertexAmount = ptMapSize.x * ptMapSize.y * 4;
		u32 elementAmount = ptMapSize.x * ptMapSize.y * 6;

		if (bResizeMap)
		{
			sdFree(pElements);
			sdFree(pVertex);

			pVertex = (sVertex *)sdAlloc(sizeof(sVertex) * vertexAmount);
			pElements = (u32 *)sdAlloc(sizeof(u32) * elementAmount);
		}

		memset(pVertex, '\0', sizeof(sVertex) * vertexAmount);
		memset(pElements, '\0', sizeof(u32) * elementAmount);

		f32 halfTileW = ptTileSize.x * 0.5f;
		f32 halfTileH = ptTileSize.y * 0.5f;
		ptMapSizeHalf.x = ptMapSize.x * 0.5f;
		ptMapSizeHalf.y = ptMapSize.y * 0.5f;

		u32 v = 0;
		u32 e = 0;
		u32 i = 0;
		Color c(255, 255, 255, 255);

		f32 curY = -ptMapSizeHalf.y;
		for (u32 y = 0; y < ptMapSize.y; y++)
		{
			f32 curX = -ptMapSizeHalf.x;
			for (u32 x = 0; x < ptMapSize.x; x++)
			{
				const Rect4f *uv = pTileSet->GetTileUV(pTileData[i]);
				if (uv)
				{
					f32 tx = ptTileSize.x * curX;
					f32 ty = ptTileSize.y * curY;

					pVertex[v + 0].cCoords.x = uv->x1;
					pVertex[v + 0].cCoords.y = uv->y1;
					pVertex[v + 0].cColor = c;
					pVertex[v + 0].cVertex = Vector3f(tx - halfTileW, ty - halfTileH, 1.0f);
					pVertex[v + 1].cCoords.x = uv->x2;
					pVertex[v + 1].cCoords.y = uv->y1;
					pVertex[v + 1].cColor = c;
					pVertex[v + 1].cVertex = Vector3f(tx + halfTileW, ty - halfTileH, 1.0f);
					pVertex[v + 2].cCoords.x = uv->x1;
					pVertex[v + 2].cCoords.y = uv->y2;
					pVertex[v + 2].cColor = c;
					pVertex[v + 2].cVertex = Vector3f(tx - halfTileW, ty + halfTileH, 1.0f);
					pVertex[v + 3].cCoords.x = uv->x2;
					pVertex[v + 3].cCoords.y = uv->y2;
					pVertex[v + 3].cColor = c;
					pVertex[v + 3].cVertex = Vector3f(tx + halfTileW, ty + halfTileH, 1.0f);

					pElements[e + 0] = v + 0;
					pElements[e + 1] = v + 1;
					pElements[e + 2] = v + 2;
					pElements[e + 3] = v + 1;
					pElements[e + 4] = v + 2;
					pElements[e + 5] = v + 3;

					v += 4;
					e += 6;
				}
				i++;
				curX++;
			}
			curY++;
		}

		cVertexBuffer.SetData(pVertex, v);
		cElementBuffer.SetData(pElements, e);

		bRebuildMesh = false;
		bTransformationChanged = true;
	}

	if (this->IsChanged())
		this->UpdateTransform();
}

void MapLayerTiled::Render(const Matrix4f &worldTransform)
{
	if (pTileSet)
	{
		ePacketFlags flags = ePacketFlags::None;//static_cast<ePacketFlags>((pConfiguration->bDebugSprite ? ePacketFlags::Wireframe : ePacketFlags::None));

		RendererPacket packet;
		packet.nMeshType = eMeshType::Triangles;
		packet.pVertexBuffer = &cVertexBuffer;
		packet.pElementBuffer = &cElementBuffer;
		packet.pTexture = pTileSet->GetTexture();
		packet.nBlendMode = nBlendOperation;
		packet.pTransform = &worldTransform;
		packet.cColor = cColor;
		packet.nFlags = flags;
		packet.vPivot = vTransformedPivot;

		pRendererDevice->UploadData(&packet);
	}
}

void MapLayerTiled::SetTileSet(TileSet *tileSet)
{
	pTileSet = tileSet;
	bRebuildMesh = true;
}

void MapLayerTiled::SetTileSize(Point2u tileSize)
{
	ptTileSize = tileSize;
	bRebuildMesh = true;
}

void MapLayerTiled::SetMapSize(Point2u mapSize)
{
	ptMapSize = mapSize;
	bRebuildMesh = true;
	bResizeMap = true;
}

TileSet *MapLayerTiled::GetTileSet()
{
	return pTileSet;
}

void MapLayerTiled::SetTileAt(u32 x, u32 y, u32 tileId)
{
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	if (TILE(x, y) == tileId)
		return;

	TILE(x, y) = tileId;
	bRebuildMesh = true;
}

void MapLayerTiled::SetTileAt(const Vector3f pos, u32 tileId)
{
	s32 x = static_cast<s32>((pos.getX() / ptTileSize.x) + ptMapSizeHalf.x);
	s32 y = static_cast<s32>((pos.getY() / ptTileSize.y) + ptMapSizeHalf.y);

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	if (TILE(x, y) == tileId)
		return;

	TILE(x, y) = tileId;
	bRebuildMesh = true;
}

u32 MapLayerTiled::GetTileAt(const Vector3f &pos) const
{
	s32 x = static_cast<s32>((pos.getX() / ptTileSize.x) + ptMapSizeHalf.x);
	s32 y = static_cast<s32>((pos.getY() / ptTileSize.y) + ptMapSizeHalf.y);

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	return TILE(x, y);
}

MapLayerTiled *MapLayerTiled::AsTiled()
{
	return this;
}

} // namespace

