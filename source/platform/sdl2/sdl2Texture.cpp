/*
* Copyright (c) 2013, Seed Developers
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

#if defined(BUILD_SDL2)

#include "Texture.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "Log.h"
#include "Screen.h"
#include "RendererDevice.h"
#include "Configuration.h"
#include "Memory.h"
#include "SDL2/render/SDL_sysrender.h"
#include <soil/SOIL.h>
#include <soil/image_helper.h>

#define TAG "[Texture] "

namespace Seed { namespace SDL2 {

IResource *TextureResourceLoader(const String &filename, ResourceManager *res)
{
	auto image = sdNew(Texture());
	image->Load(filename, res);

	return image;
}

Texture::Texture()
	: pTexture(NULL)
	, pData(NULL)
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
		sdFree(pData);

	pTexture = NULL;
	pData = NULL;

	iBytesPerPixel = 0;
	iPitch = 0;
	iAtlasWidth = 0;
	iAtlasHeight = 0;
}

bool Texture::Load(const String &filename, ResourceManager *res)
{
	if (ITexture::Load(filename, res))
	{
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

				unsigned char *resampled = (unsigned char*)sdAlloc(channels * width * height);
				up_scale_image(pData, iWidth, iHeight, channels, resampled, width, height);

				SOIL_free_image_data(pData);
				pData = resampled;
				iAtlasWidth = width;
				iAtlasHeight = height;
			}
		}

		// Create a surface
		SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(pData, iAtlasWidth, iAtlasHeight, 16, 16*2, 0x0f00, 0x00f0, 0x000f, 0xf000);

		// Removes the unecessary raw image
		sdDelete(pData);
		ITexture::Unload();

		// Create a Texture
		pTexture = SDL_CreateTextureFromSurface(pScreen->GetRenderer(), surface);

		pData = static_cast<u8 *>(pTexture->pixels);

		iBytesPerPixel = channels;
		iPitch = SEED_ROUND_UP(iAtlasWidth, 32);
		pRendererDevice->TextureRequest(this);
		bLoaded = true;

		SDL_FreeSurface(surface);
	}
	else
	{
		Log(TAG "ERROR: Could not find/load texture %s.", filename.c_str());
	}

	return bLoaded;
}

bool Texture::Load(const String &desc, u32 width, u32 height, Color *buffer, u32 atlasWidth, u32 atlasHeight, bool copy)
{
	if (buffer)
	{
#ifndef _MSC_VER
		SEED_ASSERT_MSG(SEED_ALIGN_FLOOR(buffer, 32) == (u8 *)buffer, "ERROR: User texture buffer MUST BE 32bits aligned!");
		SEED_ASSERT_MSG(SEED_ROUND_UP(width, 32) == width, "ERROR: User texture scanline MUST BE 32bits aligned - pitch/stride!");
#endif
	}

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
		iPitch = SEED_ROUND_UP(width, 32); // FIXME: parametized?

		if (copy)
		{
			pData = (u8 *)sdAlloc(iAtlasWidth * iAtlasHeight * iBytesPerPixel);
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
		sdFree(pData);

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
		sdFree(pData);

	if (pTexture)
		sdDelete(pTexture);

	pTexture = NULL;
	bLoaded = false;

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
	if (!pTexture)
		return;

	/* Here p is the address to the pixel we want to retrieve */
	u8 *p = (u8 *)pData + y * iPitch + x * iBytesPerPixel;

	switch (iBytesPerPixel)
	{
		case 3:
		{
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = px.r;
				p[1] = px.b;
				p[2] = px.g;
			}
			else
			{
				p[0] = px.g;
				p[1] = px.b;
				p[2] = px.r;
			}
		}
		break;

		case 4:
		{
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = px.r;
				p[1] = px.b;
				p[2] = px.g;
				p[3] = px.a;
			}
			else
			{
				p[0] = px.a;
				p[1] = px.g;
				p[2] = px.b;
				p[3] = px.r;
			}
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
	if (!pTexture)
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
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				px.r = p[0];
				px.g = p[1];
				px.b = p[2];
				px.a = 255;
			}
			else
			{
				px.a = 255;
				px.b = p[0];
				px.g = p[1];
				px.r = p[2];
			}
		}
		break;

		case 4:
		{
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				px.r = p[0];
				px.g = p[1];
				px.b = p[2];
				px.a = p[3];
			}
			else
			{
				px.a = p[0];
				px.b = p[1];
				px.g = p[2];
				px.r = p[3];
			}
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
	if (!pTexture)
		return 0;

	if (x >= iWidth)
	{
		x = static_cast<u32>(iAtlasWidth) - 1;
		return this->GetPixelAlpha(x, y);
	}
	else if (y >= iHeight)
	{
		y = static_cast<u32>(iAtlasHeight) - 1;
		return this->GetPixelAlpha(x, y);
	}

	Color px = this->GetPixel(x, y);
	return px.a;
}

u32 Texture::GetUsedMemory() const
{
	return (u32)(IResource::GetUsedMemory() + sizeof(this) + (iHeight * iWidth * iBytesPerPixel));
}

u32 Texture::GetBytesPerPixel() const
{
	return iBytesPerPixel;
}

void Texture::UnloadTexture()
{
	iTextureId = 0;
}

}} // namespace

#endif // BUILD_SDL2
