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

#define TILE(x, y) pTileData[(x) + (ptMapSize.y * (y))]

namespace Seed {

MapLayerTiled::MapLayerTiled()
	: pTileData(NULL)
	, pVertex(NULL)
	, pElements(NULL)
	, pTileSet(NULL)
	, cVertexBuffer()
	, cElementBuffer()
	, iDataLen(0)
	, ptTileSize(0, 0)
	, ptMapSize(0, 0)
	, ptMapSizeHalf(0.0f, 0.0f)
	, bRebuildMesh(false)
{
	cVertexBuffer.Configure(BufferUsageNeverChange);
	cElementBuffer.Configure(BufferUsageNeverChange, ElementTypeInt);
}

MapLayerTiled::~MapLayerTiled()
{
	pRendererDevice->DestroyHardwareBuffer(&cElementBuffer);
	pRendererDevice->DestroyHardwareBuffer(&cVertexBuffer);

	this->Unload();
}

bool MapLayerTiled::Unload()
{
	Free(pElements)
	Free(pVertex);
	Free(pTileData);
	pTileSet = NULL;

	return true;
}

bool MapLayerTiled::Load(Reader &reader, ResourceManager *res)
{
	UNUSED(res)

	this->Unload();

	u32 len = reader.SelectArray("data");
	this->LoadData(reader, len);
	reader.UnselectArray();

	// map size is in tiles and is only important to tile based maps, so we read it here
	this->SetMapSize(Point2u(reader.ReadU32("width", 0), reader.ReadU32("height", 0)));
	// read the generic map info in base class
	this->ReadMapLayer(reader);
	this->ReadProperties(reader);

	return true;
}

void MapLayerTiled::LoadData(Reader &reader, u32 len)
{
	if (len > 0)
	{
		if (iDataLen != len)
		{
			Free(pTileData);
			pTileData = (u32 *)Alloc(sizeof(u32) * len);
		}

		for (u32 i = 0; i < len; i++)
		{
			reader.SelectNext();
			pTileData[i] = reader.GetU32(0);
		}
	}

	if (!len)
		Free(pTileData);

	iDataLen = len;
	bRebuildMesh = true;
}

void MapLayerTiled::Update(f32 dt)
{
	UNUSED(dt)
	if (bRebuildMesh && pTileSet && pTileData)
	{
		this->SetWidth(ptMapSize.x * ptTileSize.x);
		this->SetHeight(ptMapSize.y * ptTileSize.y);

		u32 vertexAmount = ptMapSize.x * ptMapSize.y * 4;
		u32 elementAmount = ptMapSize.x * ptMapSize.y * 6;

		if (bResizeMap)
		{
			Free(pElements);
			Free(pVertex);

			pVertex = (sVertex *)Alloc(sizeof(sVertex) * vertexAmount);
			pElements = (u32 *)Alloc(sizeof(u32) * elementAmount);
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

		f32 curY = -ptMapSizeHalf.x;
		for (u32 y = 0; y < ptMapSize.y; y++)
		{
			f32 curX = -ptMapSizeHalf.y;
			for (u32 x = 0; x < ptMapSize.x; x++)
			{
				const Rect4f *uv = pTileSet->GetTileUV(pTileData[i]);

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

				i++;
				v += 4;
				e += 6;
				curX++;
			}
			curY++;
		}

		cVertexBuffer.SetData(pVertex, vertexAmount);
		cElementBuffer.SetData(pElements, elementAmount);

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
		ePacketFlags flags = FlagNone;//static_cast<ePacketFlags>((pConfiguration->bDebugSprite ? FlagWireframe : FlagNone));

		RendererPacket packet;
		packet.nMeshType = Seed::Triangles;
		packet.pVertexBuffer = &cVertexBuffer;
		packet.pElementBuffer = &cElementBuffer;
		packet.pTexture = pTileSet->GetTexture();
		packet.nBlendMode = eBlendOperation;
		packet.pTransform = &worldTransform;
		packet.cColor = cColor;
		packet.iFlags = flags;
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

u32 MapLayerTiled::GetTileAt(Vector3f pos) const
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

