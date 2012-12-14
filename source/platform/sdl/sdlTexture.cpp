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
#include "Configuration.h"

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
		Free(pData);

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
#if defined(EMSCRIPTEN) || defined(__FLASHPLAYER)
	if (1)
	{
		SDL_Surface *tmp = IMG_Load(filename.c_str());
		if (!tmp)
		{
			Log(TAG "Could not load image file: %s", filename.c_str());
			Info(TAG "IMG_Load ERROR: %s\n", IMG_GetError());
			SEED_ASSERT(false);
		}
#else
	if (ITexture::Load(filename, res))
	{
		SDL_RWops *rwops = SDL_RWFromConstMem(pFile->GetData(), pFile->GetSize());

		size_t extpos = SDL_strlen(pFile->GetName().c_str());
		char *ext = const_cast<char *>(pFile->GetName().c_str()) - 3;
		ext = &ext[extpos];

		u32 format = PNG;
		if (!SDL_strcasecmp(pImageFormatTable[TGA], ext))
			format = TGA;
		else if (!SDL_strcasecmp(pImageFormatTable[JPG], ext))
			format = JPG;

		SDL_Surface *tmp = IMG_LoadTyped_RW(rwops, 1, const_cast<char *>(pImageFormatTable[format]));

		if (!tmp)
		{
			Info(TAG "IMG_Load_RW ERROR: %s\n", IMG_GetError());

			if (format == PNG)
				Info(TAG "Make sure that libpng12-0.dll and zlib1.dll are in the exact same folder than this application binary.");

			SEED_ASSERT(false);
		}
#endif
		if (tmp->format->BitsPerPixel != 32)
		{
			SDL_SetAlpha(tmp, 0, SDL_ALPHA_OPAQUE);
		}

		pSurface = SDL_DisplayFormatAlpha(tmp);
		SEED_ASSERT(pSurface);
		SDL_FreeSurface(tmp);

		iWidth = pSurface->w;
		iHeight = pSurface->h;

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

bool Texture::Load(const String &desc, u32 width, u32 height, Color *buffer, u32 atlasWidth, u32 atlasHeight, bool copy)
{
	if (buffer)
	{
		SEED_ASSERT_MSG(ALIGN_FLOOR(buffer, 32) == (u8 *)buffer, "ERROR: User texture buffer MUST BE 32bits aligned!");
		SEED_ASSERT_MSG(ROUND_UP(width, 32) == width, "ERROR: User texture scanline MUST BE 32bits aligned - pitch/stride!");
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
		iPitch = ROUND_UP(width, 32); // FIXME: parametized?

		if (copy)
		{
			pData = (u8 *)Alloc(iAtlasWidth * iAtlasHeight * iBytesPerPixel);
			memcpy(pData, buffer, iAtlasWidth * iAtlasHeight * iBytesPerPixel);
		}
		else
		{
			pData = buffer;
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
		Free(pData);

	bCopy = false;
}

void Texture::Update(Color *data)
{
	//this->UnloadTexture();
	//pRendererDevice->TextureRequest(this, &iTextureId);
	pData = data;
	if (data)
		pRendererDevice->TextureDataUpdate(this);
}

bool Texture::Unload()
{
	ITexture::Unload();

	if (bLoaded)
		this->UnloadTexture();

	if (bCopy)
		Free(pData);

	if (pSurface)
		SDL_FreeSurface(pSurface);

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
void Texture::PutPixel(u32 x, u32 y, const Color &px)
{
	if (!pSurface)
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
	if (!pSurface)
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
