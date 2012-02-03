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

/*! \file ITexture.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Texture class interface
*/

#ifndef __ITEXTURE_H__
#define __ITEXTURE_H__

#include "Defines.h"
#include "interface/IResource.h"
#include "Enum.h"
#include "File.h"

namespace Seed {

/// ITexture
/**
Base texture class. This IResource must not be instanciable. It has basic information about the texture.
*/
class SEED_CORE_API ITexture : public IResource
{
	public:
		ITexture();
		virtual ~ITexture();

		/// Returns the texture bits data.
		virtual const void *GetData() const;

		/// PutPixel draw a pixel inside the texture.
		virtual void PutPixel(u32 x, u32 y, PIXEL px);

		/// Returns a PIXEL color from the texture.
		virtual PIXEL GetPixel(u32 x, u32 y) const;

		/// GetPixelAlpha returns only the alpha component of the pixel.
		virtual u8 GetPixelAlpha(u32 x, u32 y) const;

		/// Gets the texture width in pixels.
		virtual u32 GetWidthInPixel() const;

		/// Gets the texture height in pixels.
		virtual u32 GetHeightInPixel() const;

		/// Gets the full atlas width in pixels.
		virtual u32 GetAtlasWidthInPixel() const;

		/// Gets the full atlas height in pixels.
		virtual u32 GetAtlasHeightInPixel() const;

		/// Gets the texture width normalized (0.0f - 1.0f).
		virtual f32 GetWidth() const;

		/// Gets the texture height normalized (0.0f - 1.0f).
		virtual f32 GetHeight() const;

		/// Specify a filtering to be used with this texture.
		/**
		Be aware that when using texture atlas this filter will affect all sprites that uses it as
		this texture can be shared between many objects.

		\param type Filtering type (TextureFilterTypeMin or TextureFilterTypeMag)
		\param filter Filter to use (TextureFilterLinear or TextureFilterNearest)
		*/
		virtual void SetFilter(eTextureFilterType type, eTextureFilter filter);

		virtual eTextureFilter GetFilter(eTextureFilterType type) const;

		virtual File *GetFile();
		virtual u32 GetBytesPerPixel() const;

		/// Close/free RAM texture data without unloading VRAM texture.
		virtual void Close();
		virtual void Reset();

		// IResource
		using IResource::Load;
		virtual bool Unload();
		virtual bool Load(const char *filename, ResourceManager *res);

		/// Create a texture using a pre-allocated buffer.
		/**
		Create a texture using a pre-allocated buffer provided by the user, the
		Texture class won't deallocate it uppon Unloading, so keep sure to clean your
		buffers when not needed anymore. And the same way, keep sure that the buffer
		will exist while texture exists.

		The buffer MUST BE 32bits aligned and each scanline must be 32bits aligned (stride, pitch)

		\param width Width of the texture
		\param height Height of the texture
		\param buffer Buffer to texture pixels
		*/
		virtual bool Load(u32 width, u32 height, PIXEL *buffer, u32 atlasWidth = 0, u32 atlasHeight = 0);

		/// Update the internal state of a dynamic texture with a new buffer (texture created by the user)
		/**
		This method should be called as the user finishes its texture manipulation.
		This is a really expensive method that will keep the internal state of the texture usable by
		the underlaying system.
		In OpenGL/DX it will re-upload the texture to vga and in Nintendo Wii it should swizzle the texture.
		By using dynamic textures, the Seed MAY keep a internal buffer or pointer to your buffer that will
		be used, so be sure to keep your buffer alive until this texture is not needed.
		Keep the Width and Height unchanged, otherwise you must do a Load again.
		*/
		virtual void Update(PIXEL *buffer);

		// IObject
		virtual int GetObjectType() const;
		virtual const char *GetObjectName() const;

	public:
		void *pTextureId;
		u32 iTextureId;

	protected:
		File	stFile;

		eTextureFilter		nMinFilter;
		eTextureFilter		nMagFilter;

		u32		iWidth;
		u32		iHeight;

		f32		fWidth;
		f32		fHeight;

	private:
		SEED_DISABLE_COPY(ITexture);
};

} // namespace

#endif // __ITEXTURE_H__
