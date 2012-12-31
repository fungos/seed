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

namespace Seed {

TileSet::TileSet()
	: pTexture(NULL)
	, pTileUV(NULL)
	, mProperties()
	, mTileProperties()
	, iFirstId(1)
	, iMargin(0)
	, iSpacing(0)
	, ptTileSize(0, 0)
{
}

TileSet::~TileSet()
{
	this->Unload();
}

bool TileSet::Load(Reader &reader, ResourceManager *res)
{
	bool ret = false;

	if (this->Unload())
	{
		sName = reader.ReadString("name", "TileSet");
		iFirstId = reader.ReadU32("firstgid", 1);
		iMargin = reader.ReadU32("margin", 0);
		iSpacing = reader.ReadU32("spacing", 0);
		ptTileSize.x = reader.ReadU32("tileheight", 32);
		ptTileSize.y = reader.ReadU32("tilewidth", 32);

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

		if (reader.SelectNode("tileproperties"))
		{
			u32 k = 0;
			while (1)
			{
				const char *key = reader.GetKey(k++);
				if (!key)
					break;

				if (reader.SelectNode(key))
				{
					u32 kv = atoi(key) + iFirstId;
					u32 ks = 0;
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

		u32 texW = reader.ReadU32("imagewidth", 0);
		u32 texH = reader.ReadU32("imageheight", 0);
		String file = reader.ReadString("image");
		pTexture = static_cast<ITexture *>(res->Get(file, Seed::TypeTexture));
		pTexture->SetFilter(Seed::TextureFilterTypeMin, Seed::TextureFilterNearest);
		pTexture->SetFilter(Seed::TextureFilterTypeMag, Seed::TextureFilterNearest);

		ret = (pTexture->GetWidth() == texW && pTexture->GetHeight() == texH);
		if (ret)
		{
			u32 tilesW = texW / ptTileSize.x;
			u32 tilesH = texH / ptTileSize.y;

			f32 rInvWidth = 1.0f / pTexture->GetAtlasWidth(); // full width from image, not only frame area
			f32 rInvHeight = 1.0f / pTexture->GetAtlasHeight(); // full height from image, not only frame area

			pTileUV = NewArray(Rect4f, (tilesW * tilesH));

			u32 pixY = iMargin;
			for (u32 y = 0; y < tilesH; y++)
			{
				u32 pixX = iMargin;
				for (u32 x = 0; x < tilesW; x++)
				{
					Rect4f *uv = &pTileUV[(x) + (tilesW * (y))];

					uv->x1 = static_cast<f32>((pixX + 0.1f) * rInvWidth);
					uv->x2 = static_cast<f32>((pixX + 0.1f + ptTileSize.x) * rInvWidth);
					uv->y1 = static_cast<f32>((pixY + 0.1f) * rInvHeight);
					uv->y2 = static_cast<f32>((pixY + 0.1f + ptTileSize.y) * rInvHeight);

					pixX += iSpacing + ptTileSize.x;
				}

				pixY += iSpacing + ptTileSize.y;
			}
		}
	}

	return ret;
}

bool TileSet::Write(Writer &writer)
{
	#warning Implement TileSet write
	return true;
}

bool TileSet::Unload()
{
	Map<u32, Map<String, String> >().swap(mTileProperties);
	Map<String, String>().swap(mProperties);
	DeleteArray(pTileUV);
	sRelease(pTexture);
	return true;
}

u32 TileSet::GetFirstTileId() const
{
	return iFirstId;
}

const Rect4f *TileSet::GetTileUV(u32 tileId) const
{
	SEED_ASSERT_MSG((s32)tileId - (s32)iFirstId >= 0, "Wrong tile id.");
	Rect4f *ret = &pTileUV[tileId - iFirstId];

	SEED_ASSERT_MSG(ret, "No tile with given id");
	return ret;
}

const ITexture *TileSet::GetTexture() const
{
	return this->pTexture;
}

const String &TileSet::GetProperty(const String &property) const
{
	return mProperties.at(property);
}

const String &TileSet::GetTileProperty(u32 tileId, const String &property) const
{
	static String empty; /* ugly bastard */
	if (mTileProperties.find(tileId) != mTileProperties.end())
		return mTileProperties.at(tileId).at(property);

	return empty;
}

const String TileSet::GetClassName() const
{
	return "TileSet";
}

int TileSet::GetObjectType() const
{
	return Seed::TypeTileSet;
}

} // namespace
