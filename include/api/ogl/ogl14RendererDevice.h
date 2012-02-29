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

#ifndef __OGL14_RENDERER_DEVICE_H__
#define __OGL14_RENDERER_DEVICE_H__

#if defined(_MSC_VER)
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif

#include "Defines.h"
#include "Enum.h"
#include "Vertex.h"

#if defined(USE_API_OGL)

#include "interface/IRendererDevice.h"

#if defined(BUILD_SDL) && defined(_MSC_VER)
#pragma push_macro("Delete")
#pragma push_macro("bool")
#pragma push_macro("SIZE_T")
#undef Delete
#undef bool
#undef SIZE_T
#include <SDL/SDL_opengl.h>
#pragma pop_macro("SIZE_T")
#pragma pop_macro("bool")
#pragma pop_macro("Delete")
#elif defined(BUILD_SDL)
#include <SDL/SDL_opengl.h>
#endif

#if defined(__APPLE_CC__)
#include <OpenGL/glext.h>
#else
#include <GL/glext.h>
#endif

namespace Seed {

class ITexture;

namespace OpenGL {

/// OpenGL 1.4 Rendering device
class SEED_CORE_API OGL14RendererDevice : public IRendererDevice
{
	friend class IScreen;

	public:
		OGL14RendererDevice();
		virtual ~OGL14RendererDevice();

		virtual void Begin() const;
		virtual void End() const;

		// IRendererDevice
		virtual void TextureUnload(ITexture *tex);
		virtual void TextureRequest(ITexture *texture);
		virtual void TextureRequestAbort(ITexture *texture);
		virtual void TextureRequestProcess() const;
		virtual void TextureDataUpdate(ITexture *texture);

		virtual void SetBlendingOperation(eBlendMode mode, PIXEL color) const;
		virtual void UploadData(void *userData);
		virtual void BackbufferClear(const PIXEL color = 0);
		virtual void BackbufferFill(const PIXEL color = 0);

		virtual void SetViewport(const Rect<f32> &area) const;
		virtual void DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, bool fill = false) const;
		virtual void Enable2D() const;
		virtual void Disable2D() const;

		// IModule
		virtual bool Initialize();
		virtual bool Reset();
		virtual bool Shutdown();

	protected:
		mutable Array<ITexture *, 128> arTexture;

	private:
		SEED_DISABLE_COPY(OGL14RendererDevice);
		bool CheckExtension(const char *extName);

		int GetOpenGLMeshType(eMeshType type) const;
};

}} // namespace

#else // USE_API_OGL
	#error "Include 'RendererDevice.h' instead 'api/ogl/ogl14RendererDevice.h' directly."
#endif // USE_API_OGL

#endif // __OGL14_RENDERER_DEVICE_H__
