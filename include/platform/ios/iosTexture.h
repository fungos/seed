/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file iosTexture.h
	\author	Danny Angelo Carminati Grein
	\brief Texture Iphone Implementation
*/

#ifndef __IOS_TEXTURE_H__
#define __IOS_TEXTURE_H__

#if defined(BUILD_IOS)

#include "Defines.h"
#include "File.h"
#include "interface/ITexture.h"

#include <OpenGLES/ES1/gl.h>

namespace Seed { namespace iOS {

IResource *TextureResourceLoader(const String &filename, ResourceManager *res = pResourceManager);

class Texture : public ITexture
{
	friend IResource *TextureResourceLoader(const String &filename, ResourceManager *res);
	friend class Sprite;

	public:
		Texture();
		virtual ~Texture();

		// ITexture
		virtual const void *GetData() const;
		virtual void PutPixel(u32 x, u32 y, PIXEL px);
		virtual PIXEL GetPixel(u32 x, u32 y) const;
		virtual u8 GetPixelAlpha(u32 x, u32 y) const;

		virtual u32 GetBytesPerPixel() const;
		virtual void *GetTextureName() const;

		virtual void Update(PIXEL *buffer);
		virtual void Reset();
		virtual bool Load(u32 width, u32 height, PIXEL *buffer, u32 atlasWidth = 0, u32 atlasHeight = 0); // O que acontece no Reload?

		virtual u32 GetAtlasWidth() const;
		virtual u32 GetAtlasHeight() const;

		virtual void Close();

		// IResource
		virtual bool Load(const String &filename, ResourceManager *res = pResourceManager);
		virtual bool Unload();
		virtual u32 GetUsedMemory() const;

	protected:
		void UnloadTexture();

	private:
		SEED_DISABLE_COPY(Texture);

		//void LoadPVRTC(const char *file);
		void LoadPNG(const char *file);

	private:
		enum eTextureFormat
		{
			kTexture2DPixelFormat_Automatic = 0,
			kTexture2DPixelFormat_RGBA8888,
			kTexture2DPixelFormat_RGB565,
			kTexture2DPixelFormat_A8,
			kTexture2DPixelFormat_RGBA2
		};

	private:
		void *pData;

		u32 iBytesPerPixel;
		u32 iPitch;

		u32 iAtlasWidth;
		u32 iAtlasHeight;

		eTextureFormat pixelFormat;
};

}} // namespace

#else // BUILD_IOS
	#error "Include 'Texture.h' instead 'platform/ios/iosTexture.h' directly."
#endif // BUILD_IOS
#endif // __IOS_TEXTURE_H__
