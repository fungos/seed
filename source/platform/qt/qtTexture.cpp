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

#if defined(BUILD_QT)

#include "Texture.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "Log.h"
#include "Screen.h"

#define TAG "[Texture] "

namespace Seed { namespace QT {

IResource *TextureResourceLoader(const String &filename, ResourceManager *res)
{
	UNUSED(res);

	Texture *image = new Texture();
	image->Load(filename, res);

	return image;
}

Texture::Texture()
	: iAtlasWidth(0)
	, iAtlasHeight(0)
{
}

Texture::~Texture()
{
	this->Unload();
}

bool Texture::Unload()
{
	this->Reset();
	return true;
}

void Texture::Reset()
{
	this->UnloadTexture();

	fWidth = 0;
	fHeight = 0;
	iWidth = 0;
	iHeight = 0;
	iAtlasWidth = 0;
	iAtlasHeight = 0;
}

bool Texture::Load(const String &filename, ResourceManager *res)
{
	UNUSED(res);
	ASSERT_NULL(res);

	if (this->Unload())
	{
		image = QImage(filename);

		if (image.isNull())
		{
			return false;
		}

		image = image.convertToFormat(QImage::Format_ARGB32);

		iWidth = image.width();
		iHeight = image.height();

		iAtlasWidth = iWidth;
		iAtlasHeight = iHeight;
		//iHalfWidth = iWidth >> 1;
		//iHalfHeight = iHeight >> 1;

		fWidth = (f32)iWidth / (f32)pScreen->GetWidth();
		fHeight = (f32)iHeight / (f32)pScreen->GetHeight();
	}

	return true;
}

bool Texture::Load(u32 width, u32 height, uPixel *buffer, u32 texWidth, u32 texHeight)
{
	UNUSED(texWidth);
	UNUSED(texHeight);

	ASSERT_MSG((void *)ROUND_UP(buffer, 32) == (void *)buffer, "ERROR: User texture buffer MUST BE 32bits aligned!");
	ASSERT_MSG(ROUND_UP(width, 32) == width, "ERROR: User texture scanline MUST BE 32bits aligned - pitch/stride!");

	if (this->Unload())
	{
		image = QImage((uchar *)buffer, width, height, QImage::Format_ARGB32);

		if (image.isNull())
		{
			return false;
		}

		iWidth = width;
		iHeight = height;

		iAtlasWidth = iWidth;
		iAtlasHeight = iHeight;
		//iHalfWidth = iWidth >> 1;
		//iHalfHeight = iHeight >> 1;

		fWidth = (f32)iWidth / (f32)pScreen->GetWidth();
		fHeight = (f32)iHeight / (f32)pScreen->GetHeight();
	}

	return true;
}

const void *Texture::GetData() const
{
	return image.bits();
}

void Texture::PutPixel(u32 x, u32 y, uPixel px)
{
	u8 mR = uPixel.argb.r;
	u8 mG = uPixel.argb.g;
	u8 mB = uPixel.argb.b;
	u8 mA = uPixel.argb.a;

	image.setPixel(x, y, qRgba(mR, mG, mB, mA));
}

uPixel Texture::GetPixel(u32 x, u32 y) const
{
	QRgb px = image.pixel(x, y);
	return uPixel(qRed(px), qGreen(px), qBlue(px), qAlpha(px));
}

u8 Texture::GetPixelAlpha(u32 x, u32 y) const
{
	QRgb px = image.pixel(x, y);
	return (u8)(qAlpha(px));
}

u32 Texture::GetUsedMemory() const
{
	return image.byteCount() + sizeof(this);
}

/*
int Texture::LoadTexture()
{
	bool a = !image.isNull();
	bool b = (iTextureId == 0);
	if (a && b)
	{
		glGenTextures(1, &iTextureId);
		glBindTexture(GL_TEXTURE_2D, iTextureId);

		if (nMinFilter == Seed::TextureFilterLinear)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (nMinFilter == Seed::TextureFilterNearest)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		if (nMagFilter == Seed::TextureFilterLinear)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (nMagFilter == Seed::TextureFilterNearest)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Works on x86, so probably works on all little-endian systems.
		// Does it work on big-endian systems?
		glTexImage2D(GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return this->iTextureId;
}
*/

void *Texture::GetTextureName()
{
	return &iTextureId;
}

void Texture::UnloadTexture()
{
	if (iTextureId)
		glDeleteTextures(1, &iTextureId);
	iTextureId = 0;
}

u32 Texture::GetBytesPerPixel() const
{
	return 4;
}

void Texture::Update(uPixel *buffer)
{
	UNUSED(buffer);
}

u32 Texture::GetAtlasWidth() const
{
	return iAtlasWidth;
}

u32 Texture::GetAtlasHeight() const
{
	return iAtlasHeight;
}

}} // namespace

#endif // BUILD_QT
