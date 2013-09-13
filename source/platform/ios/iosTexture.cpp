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

#if defined(BUILD_IOS)

#include "Texture.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "Log.h"
#include "Screen.h"
#include "File.h"
#include "RendererDevice.h"
#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>

#include <math.h>

#define TAG	"[Texture] "
#define MAX_TEXTURE_SIZE	1024

namespace Seed { namespace iOS {

IResource *TextureResourceLoader(const String &filename, ResourceManager *res)
{
	auto image = New(Texture());
	image->Load(filename, res);

	return image;
}

Texture::Texture()
	: pData(NULL)
	, iBytesPerPixel(4)
	, iPitch(0)
	, iAtlasWidth(0)
	, iAtlasHeight(0)
	//, bCompressed(false)
	, pixelFormat(kTexture2DPixelFormat_RGBA8888)
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

	pData = NULL;

	iBytesPerPixel = 4;
	iPitch = 0;
	iAtlasWidth = 0;
	iAtlasHeight = 0;

	//bCompressed = false;
	pixelFormat = kTexture2DPixelFormat_RGBA8888;
}

bool Texture::Load(const String &filename, ResourceManager *res)
{
	SEED_ASSERT(res);
	if (this->Unload())
	{
		pRes = res;
		sFilename = filename;

		//#if !defined(ENABLE_NATIVE_PVRTC_FORMAT)
		this->LoadPNG(filename.c_str());
		//#else
		//this->LoadPVRTC(filename.c_str());
		//#endif // ENABLE_NATIVE_PVRTC_FORMAT

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
	SEED_ASSERT(buffer);
	SEED_ASSERT_MSG(ALIGN_FLOOR(buffer, 32) == (u8 *)buffer, "ERROR: User texture buffer MUST BE 32bits aligned!");
	SEED_ASSERT_MSG(ROUND_UP(width, 32) == width, "ERROR: User texture scanline MUST BE 32bits aligned - pitch/stride!");

	if (this->Unload())
	{
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

		pixelFormat = kTexture2DPixelFormat_RGBA8888;
		pRendererDevice->TextureRequest(this);
		bLoaded = true;
	}

	return bLoaded;
}

void Texture::Update(Color *data)
{
	pData = data;
	if (data)
		pRendererDevice->TextureDataUpdate(this);
}

bool Texture::Unload()
{
	this->UnloadTexture();
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

void Texture::PutPixel(u32 x, u32 y, const Color &px)
{
#if !defined(ENABLE_NATIVE_PVRTC_FORMAT)
	if (pData || pixelFormat != kTexture2DPixelFormat_RGB565 || pixelFormat != kTexture2DPixelFormat_A8)
	{
		Color *data = static_cast<Color *>(pData);
		data[(y * iWidth) + x] = px; // ja deve ser arrumado em relacao ao atlas
	}
	else
	{
		Log(TAG "PutPixel: Format unsupported.");
	}
#else
	Log(TAG "PutPixel doesn't work with PVRTC!");
#endif // ENABLE_NATIVE_PVRTC_FORMAT
}

Color Texture::GetPixel(u32 x, u32 y) const
{
#if !defined(ENABLE_NATIVE_PVRTC_FORMAT)
	if (!pData)
		return Color(0, 0, 0, 0);

	if (pixelFormat == kTexture2DPixelFormat_RGB565 || pixelFormat == kTexture2DPixelFormat_A8)
	{
		Log(TAG "GetPixel unsuported for format 565 (yet).");
		return Color(0, 0, 0, 0);
	}

	const Color *data = static_cast<const Color *>(pData);
	Color px = data[(y * iWidth) + x]; // ja deve ser arrumado em relacao ao atlas

	return px;
#else
	return Color(0, 0, 0, 0);
#endif // ENABLE_NATIVE_PVRTC_FORMAT
}

u8 Texture::GetPixelAlpha(u32 x, u32 y) const
{
#if !defined(ENABLE_NATIVE_PVRTC_FORMAT)
	if (!pData)
		return 255;

	if (pixelFormat == kTexture2DPixelFormat_RGB565 || pixelFormat == kTexture2DPixelFormat_A8)
	{
		Log(TAG "GetPixel unsuported for format 565 (yet).");
		return 255;
	}

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
#else
	return 255;
#endif // ENABLE_NATIVE_PVRTC_FORMAT
}

u32 Texture::GetUsedMemory() const
{
	return IResource::GetUsedMemory() + sizeof(this) + (iHeight * iWidth * iBytesPerPixel);
}

u32 Texture::GetBytesPerPixel() const
{
	return iBytesPerPixel;
}

void *Texture::GetTextureName() const
{
	return pTextureId;
}

void Texture::UnloadTexture()
{
	if (pTextureId)
	{
		pRendererDevice->TextureUnload(this);
	}
	else
	{
		pRendererDevice->TextureRequestAbort(this);
	}

	Free(pData);
	pTextureId = NULL;
}

void Texture::Close()
{
	stFile.Close();
	Free(pData);
}

// FIXME: 2009-02-15 | Use Width x Height from image. | Danny Angelo Carminati Grein
/*
void Texture::LoadPVRTC(const char *file)
{
	iHeight = 1024;
	iWidth = 1024;
	pFileSystem->Open(file, &stFile);
	pImage = stFile.GetData();
	bCompressed = true;

//#if defined(ENABLE_PRELOAD_TEXTURE)
	this->LoadTexture();
//#endif // ENABLE_PRELOAD_TEXTURE

	iWidth = 1024;
	iHeight = 1024;
	pixelFormat = kTexture2DPixelFormat_RGBA2;
	fWidth = 1024 / pScreen->GetWidth();
	fHeight = 1024 / pScreen->GetHeight();
}
*/

void Texture::LoadPNG(const char *file)
{
	NSUInteger width, height, i;
	CGContextRef context = nil;
	void *data = nil;;
	CGColorSpaceRef colorSpace;
	CGColorSpaceRef srcColorSpace;
	void *tempData;
	unsigned int *inPixel32;
	unsigned short *outPixel16;
	bool hasAlpha;
	CGImageAlphaInfo info;
	CGAffineTransform transform;
	CGSize imageSize;
	eTextureFormat pixelFormat;
	CGImageRef image;
	UIImageOrientation orientation;
	bool sizeToFit = NO;


	NSString *fname = [NSString stringWithCString:file encoding: [NSString defaultCStringEncoding]];
	//NSString *path = [@"" stringByAppendingString:fname];
	UIImage *uiImage = [UIImage imageNamed:fname];

	if (NULL == uiImage)
	{
		Log("WARNING: Image file %s not found!!!!!!", file);
		return;
	}

	image = [uiImage CGImage];
	orientation = [uiImage imageOrientation];

	if (image == NULL) return;

	info = CGImageGetAlphaInfo(image);
	hasAlpha = ((info == kCGImageAlphaPremultipliedLast) || (info == kCGImageAlphaPremultipliedFirst) || (info == kCGImageAlphaLast) || (info == kCGImageAlphaFirst) ? YES : NO);

	srcColorSpace = CGImageGetColorSpace(image);

	// FIXME: Enable support to grayscale pngs.
	SEED_ASSERT_MSG(srcColorSpace != CGColorSpaceCreateDeviceGray(), "WARNING: Grayscale PNG are not supported.\n");

	if (srcColorSpace)
	{
		if (hasAlpha)
		{
			pixelFormat = kTexture2DPixelFormat_RGBA8888;
		}
		else
		{
			pixelFormat = kTexture2DPixelFormat_RGB565;
		}
	}
	else  //NOTE: No colorspace means a mask image
	{
		pixelFormat = kTexture2DPixelFormat_A8;
	}

	imageSize = CGSizeMake(CGImageGetWidth(image), CGImageGetHeight(image));
	transform = CGAffineTransformIdentity;

	iWidth = imageSize.width;
	iHeight = imageSize.height;

	width = imageSize.width;
	if ((width != 1) && (width & (width - 1)))
	{
		i = 1;
		while ((sizeToFit ? 2 * i : i) < width) i *= 2;
		width = i;
	}

	height = imageSize.height;
	if ((height != 1) && (height & (height - 1)))
	{
		i = 1;
		while ((sizeToFit ? 2 * i : i) < height) i *= 2;
		height = i;
	}

	while ((width > MAX_TEXTURE_SIZE) || (height > MAX_TEXTURE_SIZE))
	{
		width /= 2;
		height /= 2;
		transform = CGAffineTransformScale(transform, static_cast<f32>(0.5), static_cast<f32>(0.5));
		imageSize.width *= static_cast<f32>(0.5);
		imageSize.height *= static_cast<f32>(0.5);
	}

	u32 bpc = CGImageGetBitsPerComponent(image);
	u32 bpp = CGImageGetBitsPerPixel(image);
	iBytesPerPixel = bpp / 8;
	data = Alloc(height * width * iBytesPerPixel);
	SEED_ASSERT_MSG(data != NULL, "ERROR: not enought memory - trying to allocate texture buffer.");

	switch (pixelFormat)
	{
		case kTexture2DPixelFormat_RGBA8888:
			colorSpace = CGColorSpaceCreateDeviceRGB();
			context = CGBitmapContextCreate(data, width, height, bpc, iBytesPerPixel * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
			CGColorSpaceRelease(colorSpace);
		break;

		case kTexture2DPixelFormat_RGB565:
			colorSpace = CGColorSpaceCreateDeviceRGB();
			context = CGBitmapContextCreate(data, width, height, bpc, iBytesPerPixel * width, colorSpace, kCGImageAlphaNoneSkipLast | kCGBitmapByteOrder32Big);
			CGColorSpaceRelease(colorSpace);
		break;

		case kTexture2DPixelFormat_A8:
			context = CGBitmapContextCreate(data, width, height, bpc, iBytesPerPixel * width, NULL, kCGImageAlphaOnly);
		break;

		default:
		break;
	}

	CGContextClearRect(context, CGRectMake(0, 0, width, height));
	CGContextTranslateCTM(context, 0, height - imageSize.height);

	if (!CGAffineTransformIsIdentity(transform))
		CGContextConcatCTM(context, transform);

	CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(image), CGImageGetHeight(image)), image);
	if (pixelFormat == kTexture2DPixelFormat_RGB565)
	{
		tempData = Alloc(height * width * 2);
		inPixel32 = (unsigned int *)data;
		outPixel16 = (unsigned short *)tempData;

		for (i = 0; i < width * height; ++i, ++inPixel32)
			*outPixel16++ = ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | ((((*inPixel32 >> 8) & 0xFF) >> 2) << 5) | ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);

		Free(data);
		data = tempData;
	}

	pData = data;
	//bCompressed = false;
	pixelFormat = pixelFormat;

	iAtlasWidth = width;
	iAtlasHeight = height;
	iWidth = imageSize.width;
	iHeight = imageSize.height;
	iPitch = CGImageGetBytesPerRow(image);

	pRendererDevice->TextureRequest(this);

	CGContextRelease(context);

	//Log(">>>>>>>>>>>> MEM: %s == %d == Texture ID: %d", file, (width * height * 4), iTextureId);
}

}} // namespace

#endif // BUILD_IOS
