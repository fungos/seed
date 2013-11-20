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

#include "Defines.h"

#if defined(USE_API_SOIL)

#include "api/soil/soilTexture.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "Log.h"
#include "Screen.h"
#include "RendererDevice.h"
#include "Configuration.h"
#include <soil/SOIL.h>
#include <soil/image_helper.h>

#define TAG "[Texture] "

namespace Seed { namespace SOIL {

IResource *TextureResourceLoader(const String &filename, ResourceManager *res)
{
	auto image = New(Texture());
	image->Load(filename, res);

	return image;
}

Texture::Texture()
	: pData(NULL)
	, iBytesPerPixel(0)
	, iPitch(0)
	, iAtlasWidth(0)
	, iAtlasHeight(0)
	, bCopy(false)
{
}

Texture::~Texture()
{
	this->Reset();
}

void Texture::Reset()
{
	ITexture::Reset();

	this->UnloadTexture();

	if (bCopy)
		sFree(pData)
	else if (pData)
		SOIL_free_image_data(pData);
	pData = NULL;

	iBytesPerPixel = 0;
	iPitch = 0;
	iAtlasWidth = 0;
	iAtlasHeight = 0;

	bCopy = false;
}

bool Texture::Load(const String &filename, ResourceManager *res)
{
	if (ITexture::Load(filename, res))
	{
//		u32 flags = SOIL_FLAG_MULTIPLY_ALPHA |		// for using (GL_ONE,GL_ONE_MINUS_SRC_ALPHA) blending
//					SOIL_FLAG_DDS_LOAD_DIRECT |		// will load DDS files directly without _ANY_ additional processing
//					SOIL_FLAG_COMPRESS_TO_DXT;		// if the card can display them, will convert RGB to DXT1, RGBA to DXT5

//		if (pRendererDevice->NeedPowerOfTwoTextures())
//			flags |= SOIL_FLAG_POWER_OF_TWO;

		int channels = 4, h = 0, w = 0;
		pData = SOIL_load_image_from_memory(pFile->GetData(), pFile->GetSize(), &w, &h, &channels, 0);
		SEED_ASSERT_MSG(pData, "Could not load texture data.");

		iAtlasWidth = iWidth = w;
		iAtlasHeight = iHeight = h;
		/*
		If the image isn't power of two, we need fix it.
		*/
		if (pRendererDevice->NeedPowerOfTwoTextures())
		{
			u32 width = 1;
			while (width < iWidth)
				width *= 2;

			u32 height = 1;
			while (height < iHeight)
				height *= 2;

			if (width != iWidth || height != iHeight)
			{
				Log(TAG "WARNING: texture size not optimal, changing from %dx%d to %dx%d", iWidth, iHeight, width, height);

				unsigned char *resampled = (unsigned char*)Alloc(channels * width * height);
				up_scale_image(pData, iWidth, iHeight, channels, resampled, width, height);

				SOIL_free_image_data(pData);
				pData = resampled;
				iAtlasWidth = width;
				iAtlasHeight = height;
			}
		}

		iBytesPerPixel = channels;
		iPitch = ROUND_UP(iAtlasWidth, 32);
		pRendererDevice->TextureRequest(this);
		bLoaded = true;
	}
	else
	{
		Log(TAG "ERROR: Could not find/load texture %s.", filename.c_str());
	}

	return bLoaded;
}

bool Texture::Load(const String &desc, u32 width, u32 height, Color *buffer, u32 atlasWidth, u32 atlasHeight, bool copy)
{
	if (this->Unload())
	{
		bCopy = copy;
		sFilename = desc;

		iWidth = iAtlasWidth = width;
		iHeight = iAtlasHeight = height;

		if (atlasWidth)
			iAtlasWidth = atlasWidth;

		if (atlasHeight)
			iAtlasHeight = atlasHeight;

		iBytesPerPixel = sizeof(Color); // FIXME: parametized?
		iPitch = ROUND_UP(width, 32); // FIXME: parametized?
		if (copy)
		{
			pData = (u8 *)Alloc(iAtlasWidth * iAtlasHeight * iBytesPerPixel);
			memcpy(pData, buffer, iAtlasWidth * iAtlasHeight * iBytesPerPixel);
		}
		else
		{
			pData = static_cast<u8 *>((void *)buffer);
		}

		pRendererDevice->TextureRequest(this);

		bLoaded = true;
	}

	return bLoaded;
}

void Texture::Close()
{
	ITexture::Close();

	if (bCopy)
		sFree(pData);

	bCopy = false;
}

void Texture::Update(Color *data)
{
	pData = static_cast<u8 *>((void *)data);
	if (data)
		pRendererDevice->TextureDataUpdate(this);
}

bool Texture::Unload()
{
	ITexture::Unload();

	if (bLoaded)
		this->UnloadTexture();

	if (bCopy)
		sFree(pData);

	if (pData)
		SOIL_free_image_data(pData);

	pData = NULL;
	bLoaded = false;
	bCopy = false;

	return true;
}

const void *Texture::GetData() const
{
	return pData;
}

u32 Texture::GetAtlasWidth() const
{
	return iAtlasWidth;
}

u32 Texture::GetAtlasHeight() const
{
	return iAtlasHeight;
}

// NEED TEST
void Texture::PutPixel(u32 x, u32 y, const Color &px)
{
	if (!pData)
		return;

	/* Here p is the address to the pixel we want to retrieve */
	u8 *p = (u8 *)pData + y * iPitch + x * iBytesPerPixel;

	switch (iBytesPerPixel)
	{
		case 3:
		{
			p[0] = px.g;
			p[1] = px.b;
			p[2] = px.r;
		}
		break;

		case 4:
		{
			p[0] = px.a;
			p[1] = px.g;
			p[2] = px.b;
			p[3] = px.r;
		}
		break;

		default:
		break;
	}
}

// NEED TEST
Color Texture::GetPixel(u32 x, u32 y) const
{
	Color px;
	if (!pData)
		return px;

	/* Here p is the address to the pixel we want to retrieve */
	u8 *p = (u8 *)pData + y * iPitch + x * iBytesPerPixel;

	switch (iBytesPerPixel)
	{
		case 1:
		{
			u8 v = (u8)*p;
			px = Color(v, v, v, 255);
		}
		break;

		case 2:
		{
			u16 v = *(u16 *)p;
			px = Color((v >> 11) & 5, (v >> 5) & 6, v & 5, 255);
		}
		break;

		case 3:
		{
			px.a = 255;
			px.b = p[0];
			px.g = p[1];
			px.r = p[2];
		}
		break;

		case 4:
		{
			px.a = p[0];
			px.b = p[1];
			px.g = p[2];
			px.r = p[3];
		}
		break;

		default:
		break;
	}

	return px;
}

// NEED TEST
u8 Texture::GetPixelAlpha(u32 x, u32 y) const
{
	if (!pData)
		return 0;

	if (x >= iWidth)
	{
		x = iAtlasWidth - 1;
		return this->GetPixelAlpha(x, y);
	}
	else if (y >= iHeight)
	{
		y = iAtlasHeight - 1;
		return this->GetPixelAlpha(x, y);
	}

	Color px = this->GetPixel(x, y);
	return px.a;
}

u32 Texture::GetUsedMemory() const
{
	return IResource::GetUsedMemory() + sizeof(this) + (iHeight * iWidth * iBytesPerPixel);
}

u32 Texture::GetBytesPerPixel() const
{
	return iBytesPerPixel;
}

void Texture::UnloadTexture()
{
	if (iTextureId)
	{
		pRendererDevice->TextureUnload(this);
	}
	else
	{
		pRendererDevice->TextureRequestAbort(this);
	}

	iTextureId = 0;
}

}} // namespace

#endif // USE_API_SOIL
