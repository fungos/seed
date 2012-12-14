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

#ifndef __ITEXTURE_H__
#define __ITEXTURE_H__

#include "Defines.h"
#include "interface/IResource.h"
#include "Enum.h"
#include "File.h"

namespace Seed {

/// Texture interface
/**
Base texture class. This IResource must not be instanciable. It has basic information about the texture.
*/
class SEED_CORE_API ITexture : public IResource
{
	public:
		ITexture();
		virtual ~ITexture();

		/// Returns the texture bits data.
		virtual const void *GetData() const = 0;

		/// PutPixel draw a pixel inside the texture.
		virtual void PutPixel(u32 x, u32 y, const Color &px) = 0;

		/// Returns a uPixel color from the texture.
		virtual Color GetPixel(u32 x, u32 y) const = 0;

		/// GetPixelAlpha returns only the alpha component of the pixel.
		virtual u8 GetPixelAlpha(u32 x, u32 y) const = 0;

		/// Gets the full atlas width in pixels.
		virtual u32 GetAtlasWidth() const;

		/// Gets the full atlas height in pixels.
		virtual u32 GetAtlasHeight() const;

		/// Gets the texture width.
		virtual u32 GetWidth() const;

		/// Gets the texture height.
		virtual u32 GetHeight() const;

		/// Specify a filtering to be used with this texture.
		/**
		Be aware that when using texture atlas this filter will affect all sprites that uses it as
		this texture can be shared between many objects.

		\param type Filtering type (TextureFilterTypeMin or TextureFilterTypeMag)
		\param filter Filter to use (TextureFilterLinear or TextureFilterNearest)
		*/
		virtual void SetFilter(eTextureFilterType type, eTextureFilter filter);

		/// Specify if this texture will be used as a Render Target
		/**
		When set to be a Render Target, it will create frame buffer and depth buffer if needed.
		*/
		virtual bool EnableRenderTarget(bool useDepthBuffer = false);

		/// Disable the use of this texture as a render target.
		/**
		Will destroy frame buffer and depth buffer if existent.
		*/
		virtual void DisableRenderTarget();

		/// Get the render target id
		/**
		If the id value is greater than zero, then this is a render target, else it is a normal texture.
		*/
		virtual u32 GetRenderTarget() const;

		virtual eTextureFilter GetFilter(eTextureFilterType type) const;

		virtual File *GetFile();
		virtual u32 GetBytesPerPixel() const;

		virtual eTextureCompression GetCompressionType() const;

		/// Close/free RAM texture data without unloading VRAM texture.
		virtual void Close();
		virtual void Reset();

		// IResource
		virtual bool Unload();
		virtual bool Load(const String &filename, ResourceManager *res = pResourceManager);

		/// Create a texture using a pre-allocated buffer.
		/**
		Create a texture using a pre-allocated buffer provided by the user, the
		Texture class won't deallocate it uppon Unloading, so keep sure to clean your
		buffers when not needed anymore. And the same way, keep sure that the buffer
		will exist while texture exists.

		The buffer MUST BE 32bits aligned and each scanline must be 32bits aligned (stride, pitch)

		\param desc Textual description of the buffer for debug purposes
		\param width Width of the texture
		\param height Height of the texture
		\param buffer Buffer to texture pixels
		\param copy If texture should copy pixel buffer, normally used when the buffer will be destroyed
					after the Load request, but as texture loading is async we can keep a copy temporary
					until the renderer finishes the loading process, then it will be destroyed too.
		*/
		virtual bool Load(const String &desc, u32 width, u32 height, Color *buffer, u32 atlasWidth = 0, u32 atlasHeight = 0, bool copy = false) = 0;

		/// Update the internal state of a dynamic texture with a new buffer (texture created by the user)
		/**
		This method should be called as the user finishes its texture manipulation.
		This is a really expensive method that will keep the internal state of the texture usable by
		the underlaying system.
		In OpenGL/DX it will re-upload the texture to vga and in Nintendo Wii it should swizzle the texture.
		By using dynamic textures, the Seed MAY keep a internal buffer or pointer to your buffer that will
		be used, so be sure to keep your buffer alive until this texture is not needed.
		Keep the Width and Height unchanged, otherwise you must do a Load again.

		NOTE: Update should not be used with temporary buffers (ie. buffers that need be free'd just after Update
			  request. THIS WILL CRASH. Keep your buffer alive!

		NOTE2: To be safe, before destruction do a Update(NULL).
		*/
		virtual void Update(Color *buffer) = 0;

		// IObject
		virtual int GetObjectType() const;
		virtual const String GetClassName() const;

	public:
		void	*pTextureId;
		u32		iTextureId;
		eTextureCompression	nTextureCompression;

	protected:
		File	*pFile;

		eTextureFilter		nMinFilter;
		eTextureFilter		nMagFilter;

		u32		iWidth;
		u32		iHeight;

		u32		iRenderTargetId;
		u32		iDepthTargetId;

	private:
		SEED_DISABLE_COPY(ITexture);
};

} // namespace

#endif // __ITEXTURE_H__
