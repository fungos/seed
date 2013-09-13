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

#ifndef __OGLES1_RENDERER_DEVICE_H__
#define __OGLES1_RENDERER_DEVICE_H__

#if !defined(BUILD_IOS)
#include "glew/glew.h"
#endif

#if defined(_MSC_VER)
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif

#include "Defines.h"
#include "Enum.h"
#include "Vertex.h"
#include "Container.h"

#if defined(USE_API_OGL)

#include "interface/IRendererDevice.h"

#if defined(BUILD_SDL) && defined(_MSC_VER)
#define NO_SDL_GLEXT	1
#include <SDL/SDL_opengl.h>
#elif defined(BUILD_SDL)
#define NO_SDL_GLEXT	1
#include <SDL/SDL_opengl.h>
#endif

#if defined(BUILD_IOS)
#include <OpenGLES/ES1/gl.h>
#elif defined(__APPLE_CC__)
#include <OpenGL/glext.h>
#else
#include <GL/glext.h>
#endif

namespace Seed {

class ITexture;

namespace OpenGL {

/// OpenGL ES 1.1 and OpenGL 1.5+ Rendering device
class SEED_CORE_API OGLES1RendererDevice : public IRendererDevice
{
	friend class IScreen;
	SEED_DECLARE_CONTAINER(Vector, ITexture)
	SEED_DISABLE_COPY(OGLES1RendererDevice)

	public:
		OGLES1RendererDevice();
		virtual ~OGLES1RendererDevice();

		virtual void Begin() const override;
		virtual void End() const override;

		// Generic operations
		virtual void SetBlendingOperation(eBlendMode mode, const Color &color) const override;
		virtual void UploadData(void *userData) override;
		virtual void BackbufferClear(const Color &color) const override;
		virtual void BackbufferFill(const Color &color) const override;

		// IRendererDevice
		virtual void TextureUnload(ITexture *tex) override;
		virtual void TextureRequest(ITexture *texture) override;
		virtual void TextureRequestAbort(ITexture *texture) override;
		virtual void TextureRequestProcess() const override;
		virtual void TextureDataUpdate(ITexture *texture) override;
		virtual void SetTextureParameters(const ITexture *texture) const override;

		// HardwareBuffer
		virtual void DestroyHardwareBuffer(IHardwareBuffer *buf) const override;

		// Render to Texture support
		virtual u32 CreateFrameBuffer(ITexture *texture = NULL) override;
		virtual void ActivateFrameBuffer(u32 buffer = 0) override;
		virtual void DestroyFrameBuffer(u32 buffer) override;
		virtual u32 CreateDepthBuffer(u32 w, u32 h) override;
		virtual void ActivateDepthBuffer(u32 buffer = 0) override;
		virtual void DestroyDepthBuffer(u32 buffer) override;
		/// Attach a depth buffer to the active frame buffer
		virtual void AttachDepthBuffer(u32 buffer) override;
		virtual bool CheckFrameBufferStatus() const override;

		// Support
		virtual void EnableScissor(bool b) const override;
		virtual void SetScissor(f32 x, f32 y, f32 w, f32 h) const override;
		virtual void SetViewport(f32 x, f32 y, f32 w, f32 h) const override;
		virtual void Enable2D() const override;
		virtual void Disable2D() const override;

		// Features
		virtual bool NeedPowerOfTwoTextures() const override;

		// Debugging
		virtual void DrawRect(f32 x, f32 y, f32 w, f32 h, const Color &color, bool fill = false) const override;
		virtual void DrawCircle(f32 x, f32 y, f32 radius, const Color &color) const override;
		virtual void DrawLines(f32 *points, u32 len, const Color &color) const override;

		// IManager
		virtual bool Initialize() override;
		virtual bool Reset() override;
		virtual bool Shutdown() override;

	protected:
		mutable ITextureVector vTexture;

	private:
		int GetOpenGLBufferUsageType(eBufferUsage usage) const;
		int GetOpenGLBufferTargetType(eBufferTarget type) const;
		int GetOpenGLMeshType(eMeshType type) const;
		int GetOpenGLElementType(eElementType type) const;
		int GetOpenGLElementSizeByType(eElementType type) const;

		bool bHasFrameBuffer : 1;
		bool bNeedPowerOfTwoTexture : 1;
};

}} // namespace

#else // USE_API_OGL
	#error "Include 'RendererDevice.h' instead 'api/ogl/oglES1RendererDevice.h' directly. Or maybe you need set define SEED_ENABLE_OGLES1."
#endif // USE_API_OGL

#endif // __OGLES1_RENDERER_DEVICE_H__
