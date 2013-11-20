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

#ifndef __SDL_TEXTURE_H__
#define __SDL_TEXTURE_H__

#if defined(BUILD_SDL)

#include "Defines.h"
#include "File.h"
#include "interface/ITexture.h"

namespace Seed { namespace SDL {

IResource *TextureResourceLoader(const String &filename, ResourceManager *res = pResourceManager);

/// SDL Texture
class SEED_CORE_API Texture : public ITexture
{
	friend IResource *TextureResourceLoader(const String &filename, ResourceManager *res);

	SEED_DISABLE_COPY(Texture)

	public:
		Texture();
		virtual ~Texture();

		// ITexture
		virtual const void *GetData() const override;
		virtual void PutPixel(u32 x, u32 y, const Color &px) override;
		virtual Color GetPixel(u32 x, u32 y) const override;
		virtual u8 GetPixelAlpha(u32 x, u32 y) const override;

		virtual u32 GetBytesPerPixel() const override;

		virtual void Update(Color *buffer) override;
		virtual void Close() override;
		virtual void Reset() override;
		virtual bool Load(const String &desc, u32 width, u32 height, Color *buffer = NULL, u32 atlasWidth = 0, u32 atlasHeight = 0, bool copy = false) override; // O que acontece no Reload?

		virtual u32 GetAtlasWidth() const override;
		virtual u32 GetAtlasHeight() const override;

		// IResource
		virtual bool Load(const String &filename, ResourceManager *res = pResourceManager) override;
		virtual bool Unload() override;
		virtual u32 GetUsedMemory() const override;

	protected:
		void UnloadTexture();

	private:
		SDL_Surface *pSurface;

		void *pData;

		u32 iBytesPerPixel;
		u32 iPitch;

		u32 iAtlasWidth;
		u32 iAtlasHeight;

		bool bCopy : 1;
};

}} // namespace

#else // BUILD_SDL
	#error "Include 'Texture.h' instead 'platform/sdl/sdlTexture.h' directly."
#endif // BUILD_SDL
#endif // __SDL_TEXTURE__
