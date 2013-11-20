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

#ifndef __D3D8_RENDERER_DEVICE_H__
#define __D3D8_RENDERER_DEVICE_H__

#if defined(_MSC_VER)
#pragma comment(lib, "d3d8.lib")
#pragma comment(lib, "d3dx8.lib")
#pragma comment(lib, "version.lib")
#endif

#include "Defines.h"
#include "Enum.h"
#include "Vertex.h"
#include "Container.h"

#if defined(SEED_ENABLE_D3D8)

#include "interface/IRendererDevice.h"

#include <dx8/d3d8.h>
#include <dx8/d3dx8.h>

#define D3DFVF_VERTEXFORMAT (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

namespace Seed {

class ITexture;

namespace DirectX {

/// DirectX 8 Rendering device
class SEED_CORE_API D3D8RendererDevice : public IRendererDevice
{
	friend class IScreen;
	SEED_DECLARE_CONTAINER(Vector, ITexture)
	SEED_DISABLE_COPY(D3D8RendererDevice)

	public:
		D3D8RendererDevice();
		virtual ~D3D8RendererDevice();

		// Generic operations
		virtual void SetBlendingOperation(eBlendMode mode, const Color &color) const override;
		virtual void UploadData(void *userData) override;
		virtual void BackbufferClear(const Color &color) const override;
		virtual void BackbufferFill(const Color &color) const override;

		// Texturing operations
		virtual void TextureRequest(ITexture *texture) override;
		virtual void TextureRequestAbort(ITexture *texture) override;
		virtual void TextureRequestProcess() const override;
		virtual void TextureDataUpdate(ITexture *texture) override;
		virtual void TextureUnload(ITexture *texture) override;
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
		virtual f32 GetHorizontalTexelOffset() const override;
		virtual f32 GetVerticalTexelOffset() const override;
		virtual void Enable2D() const override;
		virtual void Disable2D() const override;
		virtual void Begin() const override;
		virtual void End() const override;

		// Features
		virtual bool NeedPowerOfTwoTextures() const override;

		// Debugging
		virtual void DrawRect(f32 x, f32 y, f32 w, f32 h, const Color &color, bool fill = false) const override;
		virtual void DrawCircle(f32 x, f32 y, f32 radius, const Color &color) const override;
		virtual void DrawLines(f32 *points, u32 len, const Color &color) const override;
//		virtual void DrawPolygon(f32 *vertices, const Color &color) const override;

		// Other
		virtual void Update() override;

		// IManager
		virtual bool Initialize() override;
		virtual bool Reset() override;
		virtual bool Shutdown() override;

	protected:
		mutable ITextureVector vTexture;

	private:
		bool CheckExtension(const char *extName);

		D3DCAPS8		mCaps;
		LPDIRECT3D8		mObject;
		LPDIRECT3DDEVICE8	mDevice;
		D3DPRESENT_PARAMETERS	mParams;
		D3DDISPLAYMODE		mMode;

		bool			bLost : 1;
};

}} // namespace

#else // SEED_ENABLE_D3D8
	#error "Include 'RendererDevice.h' instead 'api/directx/D3D8RendererDevice.h' directly."
#endif // SEED_ENABLE_D3D8

#endif // __D3D8_RENDERER_DEVICE_H__
