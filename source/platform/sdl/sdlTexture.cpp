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

#if defined(BUILD_SDL)

#include "Texture.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "Log.h"
#include "Screen.h"
#include "RendererDevice.h"
#include "SeedInit.h"

#define TAG "[Texture] "

namespace Seed { namespace SDL {

const char *const pImageFormatTable[] = {"TGA", "PNG", "JPG"};
enum eImageFormat
{
	TGA,
	PNG,
	JPG
};

IResource *TextureResourceLoader(const String &filename, ResourceManager *res)
{
	Texture *image = New(Texture());
	image->Load(filename, res);

	return image;
}

Texture::Texture()
	: pSurface(NULL)
	, pData(NULL)
	, iBytesPerPixel(0)
	, iPitch(0)
	, iAtlasWidth(0)
	, iAtlasHeight(0)
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

	if (pSurface)
		SDL_FreeSurface(pSurface);
	pSurface = NULL;
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
		SDL_RWops *rwops = SDL_RWFromConstMem(stFile.GetData(), stFile.GetSize());

		SIZE_T extpos = STRLEN(stFile.GetName().c_str());
		char *ext = const_cast<char *>(stFile.GetName().c_str()) - 3;
		ext = &ext[extpos];

		u32 format = PNG;
		if (!STRCASECMP(pImageFormatTable[TGA], ext))
			format = TGA;
		else if (!STRCASECMP(pImageFormatTable[JPG], ext))
			format = JPG;

		SDL_Surface *tmp = IMG_LoadTyped_RW(rwops, 1, const_cast<char *>(pImageFormatTable[format]));

		if (!tmp)
		{
			Info(TAG "IMG_Load_RW ERROR: %s\n", IMG_GetError());

			if (format == PNG)
				Info(TAG "Make sure that libpng12-0.dll and zlib1.dll are in the exact same folder than this application binary.");

			ASSERT(false);
		}

		if (tmp->format->BitsPerPixel != 32)
		{
			SDL_SetAlpha(tmp, 0, SDL_ALPHA_OPAQUE);
		}

		pSurface = SDL_DisplayFormatAlpha(tmp);
		ASSERT_NULL(pSurface);
		SDL_FreeSurface(tmp);

		iWidth = pSurface->w;
		iHeight = pSurface->h;

		if (pRendererDevice->NeedPowerOfTwoTextures())
		{
			/*
			If the image isn't power of two, we need fix it.
			*/
			u32 width = 1;
			while (width < iWidth)
				width *= 2;

			u32 height = 1;
			while (height < iHeight)
				height *= 2;

			eRendererDeviceType type = pConfiguration->GetRendererDeviceType();
			if (type >= Seed::RendererDeviceOpenGLES1 && type <= Seed::RendererDeviceOpenGL40)
			{
				/*
				HACK:
				When using OpenGL we must pass power of 2 textures to the device, so we check if the texture need fix and create
				a new and correct surface for it. When using DirectX the device will not use SDL Surface to load texture, it will
				load directly from our in memory file and then close it, so we don't need to fix our texture.
				*/

				if (width != iWidth || height != iHeight)
				{
					Log(TAG "WARNING: texture size not optimal, changing from %dx%d to %dx%d", iWidth, iHeight, width, height);

					SDL_Surface *pTempSurface = NULL;
					Uint32 rmask, gmask, bmask, amask;

					#if SDL_BYTEORDER == SDL_BIG_ENDIAN
						rmask = 0xff000000;
						gmask = 0x00ff0000;
						bmask = 0x0000ff00;
						amask = 0x000000ff;
					#else
						rmask = 0x000000ff;
						gmask = 0x0000ff00;
						bmask = 0x00ff0000;
						amask = 0xff000000;
					#endif

					pTempSurface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA , width, height, 32, bmask, gmask, rmask, amask);

					SDL_SetAlpha(pTempSurface, 0, SDL_ALPHA_OPAQUE);
					SDL_SetAlpha(pSurface, 0, SDL_ALPHA_OPAQUE);
					SDL_BlitSurface(pSurface, NULL, pTempSurface, NULL);
					SDL_SetAlpha(pTempSurface, 0, SDL_ALPHA_TRANSPARENT);
					SDL_SetAlpha(pSurface, 0, SDL_ALPHA_TRANSPARENT);

					SDL_FreeSurface(pSurface);
					pSurface = pTempSurface;
				}
			}
		}

		// FIXME: Must divide by res_width , res_height - not by screen width/height
		iAtlasWidth = pSurface->w;
		iAtlasHeight = pSurface->h;

		// Lets keep the iWidth and iHeight the original one so the sprite rect can match it.
		// For texture UV mapping, we use the relation between original W and H and the converted texture W and H.
		//iWidth = pSurface->w;
		//iHeight = pSurface->h;

		iBytesPerPixel = pSurface->format->BytesPerPixel;
		iPitch = pSurface->pitch;
		pData = pSurface->pixels;

		pRendererDevice->TextureRequest(this);

		bLoaded = true;
	}
	else
	{
		Log(TAG "ERROR: Could not find/load texture %s.", filename.c_str());
	}

	return bLoaded;
}

bool Texture::Load(u32 width, u32 height, uPixel *buffer, u32 atlasWidth, u32 atlasHeight)
{
	ASSERT_NULL(buffer);

	ASSERT_MSG(ALIGN_FLOOR(buffer, 32) == (u8 *)buffer, "ERROR: User texture buffer MUST BE 32bits aligned!");
	ASSERT_MSG(ROUND_UP(width, 32) == width, "ERROR: User texture scanline MUST BE 32bits aligned - pitch/stride!");

	if (this->Unload())
	{
		sFilename = "[dynamic sdl texture]";

		iWidth = iAtlasWidth = width;
		iHeight = iAtlasHeight = height;

		if (atlasWidth)
			iAtlasWidth = atlasWidth;

		if (atlasHeight)
			iAtlasHeight = atlasHeight;

		iBytesPerPixel = 4; // FIXME: parametized?
		iPitch = ROUND_UP(width, 32); // FIXME: parametized?
		pData = buffer;

		pRendererDevice->TextureRequest(this);

		bLoaded = true;
	}

	return bLoaded;
}

void Texture::Update(uPixel *data)
{
	//this->UnloadTexture();
	//pRendererDevice->TextureRequest(this, &iTextureId);
	pData = data;
	pRendererDevice->TextureDataUpdate(this);
}

bool Texture::Unload()
{
	this->UnloadTexture();

	if (pSurface)
	{
		SDL_FreeSurface(pSurface);
	}

	pSurface = NULL;
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
void Texture::PutPixel(u32 x, u32 y, uPixel px)
{
	if (!pSurface)
		return;

	/* Here p is the address to the pixel we want to retrieve */
	u8 *p = (u8 *)pData + y * iPitch + x * iBytesPerPixel;

	switch (iBytesPerPixel)
	{
		case 3:
		{
			u8 r = static_cast<u8>(px.argb.r);
			u8 g = static_cast<u8>(px.argb.g);
			u8 b = static_cast<u8>(px.argb.b);

			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = r;
				p[1] = b;
				p[2] = g;
			}
			else
			{
				p[0] = g;
				p[1] = b;
				p[2] = r;
			}
		}
		break;

		case 4:
		{
			*(u32 *)p = px.pixel;
		}
		break;

		default:
		break;
	}
}

// NEED TEST
uPixel Texture::GetPixel(u32 x, u32 y) const
{
	uPixel px;
	if (!pSurface)
		return px;

	/* Here p is the address to the pixel we want to retrieve */
	u8 *p = (u8 *)pData + y * iPitch + x * iBytesPerPixel;

	switch (iBytesPerPixel)
	{
		case 1:
		{
			px.pixel = (u32)*p;
		}
		break;

		case 2:
		{
			px.pixel = (u32)*(u16 *)p;
		}
		break;

		case 3:
		{
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				px.argb.r = p[0] << 16;
				px.argb.g = p[1] << 8;
				px.argb.b = p[2];
				px.argb.a = 255;
			}
			else
			{
				px.argb.r = p[0];
				px.argb.g = p[1] << 8;
				px.argb.b = p[2] << 16;
				px.argb.a = 255;
			}
		}
		break;

		case 4:
		{
			px.pixel = *(u32 *)p;
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
	if (!pSurface)
		return 0;

	if (x >= iWidth)
	{
		x = static_cast<u32>(pSurface->w) - 1;
		return this->GetPixelAlpha(x, y);
	}
	else if (y >= iHeight)
	{
		y = static_cast<u32>(pSurface->h) - 1;
		return this->GetPixelAlpha(x, y);
	}

	u8 a = 255;
	u8 r, g, b;
	if (iBytesPerPixel == 4)
	{
		uPixel px = this->GetPixel(x, y);

		if (pSurface)
			SDL_GetRGBA(px.pixel, pSurface->format, &r, &g, &b, &a);
	}

	return a;
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

#endif // BUILD_SDL
