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

#include "map/TileSet.h"
#include "Texture.h"
#include "Memory.h"

namespace Seed {

TileSet::TileSet()
	: pTexture(nullptr)
	, pTileUV(nullptr)
	, mProperties()
	, mTileProperties()
	, iFirstId(1)
	, iMargin(0)
	, iSpacing(0)
	, ptTileSize(0, 0)
	, ptTiles(0, 0)
{
}

TileSet::~TileSet()
{
	this->Unload();
}

void TileSet::RebuildUVMapping()
{
	auto rInvWidth = 1.0f / pTexture->GetAtlasWidth(); // full width from image, not only frame area
	auto rInvHeight = 1.0f / pTexture->GetAtlasHeight(); // full height from image, not only frame area

	auto pixY = iMargin;
	for (u32 y = 0; y < ptTiles.y; y++)
	{
		auto pixX = iMargin;
		for (u32 x = 0; x < ptTiles.x; x++)
		{
			auto uv = &pTileUV[(x) + (ptTiles.x * (y))];

			uv->x1 = f32((pixX + 0.1f) * rInvWidth);
			uv->x2 = f32((pixX + 0.1f + ptTileSize.x) * rInvWidth);
			uv->y1 = f32((pixY + 0.1f) * rInvHeight);
			uv->y2 = f32((pixY + 0.1f + ptTileSize.y) * rInvHeight);

			pixX += iSpacing + ptTileSize.x;
		}

		pixY += iSpacing + ptTileSize.y;
	}
}

void TileSet::Set(Reader &reader)
{
	sName = reader.ReadString("name", sName.c_str());
	iFirstId = reader.ReadU32("firstgid", iFirstId);
	iMargin = reader.ReadU32("margin", iMargin);
	iSpacing = reader.ReadU32("spacing", iSpacing);
	ptTileSize.x = reader.ReadU32("tileheight", ptTileSize.x);
	ptTileSize.y = reader.ReadU32("tilewidth", ptTileSize.y);

	this->ReadProperties(reader);
	this->ReadTileProperties(reader);

	auto texW = reader.ReadU32("imagewidth", 0);
	auto texH = reader.ReadU32("imageheight", 0);
	auto file = String(reader.ReadString("image"));
	if (texW && texH && file != "")
	{
		sdRelease(pTexture);

		pTexture = static_cast<ITexture *>(pRes->Get(file, ITexture::GetTypeId()));
		pTexture->SetFilter(eTextureFilterType::Min, eTextureFilter::Nearest);
		pTexture->SetFilter(eTextureFilterType::Mag, eTextureFilter::Nearest);
		SEED_WARNING(pTexture->GetWidth() != texW || pTexture->GetHeight() != texH, "%s: TileSet error: texture size is different from specified image size ('%s').", sName.c_str(), file.c_str());

		auto tilesW = texW / ptTileSize.x;
		auto tilesH = texH / ptTileSize.y;

		if (ptTiles.x != tilesW || ptTiles.y != tilesH)
		{
			sdDeleteArray(pTileUV);
			pTileUV = sdNewArray(Rect4f, (tilesW * tilesH));
		}

		ptTiles.x = tilesW;
		ptTiles.y = tilesH;
	}

	SEED_ASSERT_FMT(pTileUV != nullptr, "%s: Could not create texture mapping.", sName.c_str());
	this->RebuildUVMapping();
}

void TileSet::ReadTileProperties(Reader &reader)
{
	if (reader.SelectNode("tileproperties"))
	{
		auto k = 0;
		while (1)
		{
			const char *key = reader.GetKey(k++);
			if (!key)
				break;

			if (reader.SelectNode(key))
			{
				auto kv = atoi(key) + iFirstId;
				auto ks = 0;
				while (1)
				{
					const char *keyStr = reader.GetKey(ks++);
					if (!keyStr)
						break;

					mTileProperties[kv][keyStr] = reader.ReadString(keyStr, "");
				}

				reader.UnselectNode();
			}
		}
		reader.UnselectNode();
	}
}

void TileSet::ReadProperties(Reader &reader)
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

bool TileSet::Load(Reader &reader, ResourceManager *res)
{
	SEED_ASSERT(res);
	if (!this->Unload())
		return false;

	pRes = res;
	this->Set(reader);

	return true;
}

/*
 * Here we will ignore Seed json standard to be compatible with tiled on both read+write.
 */
bool TileSet::Write(Writer &writer)
{
	SEED_ASSERT_FMT(pTexture, "Missing a texture for tileset %s when writing to file.", sName.c_str());

	writer.OpenNode();
		writer.WriteString("name", sName.c_str());
		writer.WriteString("image", pTexture->GetFilename().c_str());
		writer.WriteU32("firstgid", iFirstId);
		writer.WriteU32("margin", iMargin);
		writer.WriteU32("spacing", iSpacing);
		writer.WriteU32("tileheight", ptTileSize.x);
		writer.WriteU32("tilewidth", ptTileSize.y);
		writer.WriteU32("imagewidth", pTexture->GetWidth());
		writer.WriteU32("imageheight", pTexture->GetHeight());
		this->WriteProperties(writer);
		this->WriteTileProperties(writer);
	writer.CloseNode();

	return true;
}

void TileSet::WriteProperties(Writer &writer)
{
	writer.OpenNode("properties");
	for (auto &kv : mProperties)
		writer.WriteString((kv.first).c_str(), (kv.second).c_str());
	writer.CloseNode();
}

void TileSet::WriteTileProperties(Writer &writer)
{
	char tostr[16];
	writer.OpenNode("tileproperties");
	for (auto &kvt : mTileProperties)
	{
		snprintf(tostr, sizeof(tostr), "%d", kvt.first);
		writer.OpenNode(tostr);
		for (auto &kvp : kvt.second)
			writer.WriteString((kvp.first).c_str(), (kvp.second).c_str());
		writer.CloseNode();
	}
	writer.CloseNode();
}

bool TileSet::Unload()
{
	Map<u32, Map<String, String> >().swap(mTileProperties);
	Map<String, String>().swap(mProperties);
	sdDeleteArray(pTileUV);
	sdRelease(pTexture);

	iFirstId = 1;
	iMargin = 0;
	iSpacing = 0;
	ptTileSize = uvec2(0, 0);
	ptTiles = uvec2(0, 0);

	sName = this->GetTypeName();

	return true;
}

TileSet *TileSet::Clone() const
{
	auto obj = sdNew(TileSet);
	obj->GenerateCloneName(sName);

	auto tilesW = pTexture->GetWidth() / ptTileSize.x;
	auto tilesH = pTexture->GetHeight() / ptTileSize.y;

	sdAcquire(pTexture);

	obj->pTileUV = sdNewArray(Rect4f, (tilesW * tilesH));
	for (u32 y = 0; y < ptTiles.y; y++)
	{
		for (u32 x = 0; x < ptTiles.x; x++)
		{
			obj->pTileUV[(x) + (ptTiles.x * (y))] = pTileUV[(x) + (ptTiles.x * (y))];
		}
	}

	obj->pTexture = pTexture;
	obj->pTileUV = pTileUV;
	obj->mProperties = mProperties;
	obj->mTileProperties = mTileProperties;

	obj->iFirstId = iFirstId;
	obj->iMargin = iMargin;
	obj->iSpacing = iSpacing;
	obj->ptTileSize = ptTileSize;

	return obj;
}

u32 TileSet::GetFirstTileId() const
{
	return iFirstId;
}

const Rect4f *TileSet::GetTileUV(u32 tileId) const
{
	Rect4f *ret = nullptr;

	if (tileId)
	{
		SEED_ASSERT_MSG(s32(tileId) - s32(iFirstId) >= 0, "Wrong tile id.");
		ret = &pTileUV[tileId - iFirstId];
		SEED_ASSERT_MSG(ret, "No tile with given id");
	}

	return ret;
}

const ITexture *TileSet::GetTexture() const
{
	return pTexture;
}

void TileSet::SetTexture(ITexture *texture)
{
	SEED_ASSERT(texture);

	if (pTexture)
		pTexture->Release();

	pTexture = texture;
	pTexture->Acquire();
}

void TileSet::SetTileSize(uvec2 tileSize)
{
	ptTileSize = tileSize;
}

void TileSet::SetProperty(const String &key, const String &value)
{
	mProperties[key] = value;
}

const String &TileSet::GetProperty(const String &property) const
{
	return mProperties.at(property);
}

void TileSet::SetTileProperty(u32 tileId, const String &key, const String &value)
{
	mTileProperties[tileId][key] = value;
}

const String &TileSet::GetTileProperty(u32 tileId, const String &property) const
{
	static String empty; /* ugly bastard */
	if (mTileProperties.find(tileId) != mTileProperties.end())
		return mTileProperties.at(tileId).at(property);

	return empty;
}

} // namespace
