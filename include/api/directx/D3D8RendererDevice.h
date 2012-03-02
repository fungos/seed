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

#if defined(SEED_ENABLE_D3D8)

#include "interface/IRendererDevice.h"

#pragma push_macro("Delete")
#pragma push_macro("bool")
#pragma push_macro("SIZE_T")
#undef Delete
#if defined(_MSC_VER)
#undef bool
#endif
#undef SIZE_T
#include <dx8/d3d8.h>
#include <dx8/d3dx8.h>
#pragma pop_macro("SIZE_T")
#pragma pop_macro("bool")
#pragma pop_macro("Delete")

#define D3DFVF_VERTEXFORMAT (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

namespace Seed {

class ITexture;

namespace DirectX {

/// DirectX 8 Rendering device
class SEED_CORE_API D3D8RendererDevice : public IRendererDevice
{
	friend class IScreen;

	public:
		D3D8RendererDevice();
		virtual ~D3D8RendererDevice();

		virtual void Begin() const;
		virtual void End() const;

		// IRendererDevice
		virtual void TextureUnload(ITexture *texture);
		virtual void TextureRequest(ITexture *texture);
		virtual void TextureRequestAbort(ITexture *texture);
		virtual void TextureRequestProcess() const;
		virtual void TextureDataUpdate(ITexture *texture);

		virtual void SetBlendingOperation(eBlendMode mode, uPixel color) const;
		virtual void UploadData(void *userData);
		virtual void BackbufferClear(const uPixel color = 0);
		virtual void BackbufferFill(const uPixel color = 0);

		virtual void SetViewport(const Rect4f &area) const;
		virtual void DrawRect(f32 x, f32 y, f32 w, f32 h, uPixel color, bool fill = false) const;
		virtual void Enable2D() const;
		virtual void Disable2D() const;

		virtual void Update();

		// IModule
		virtual bool Initialize();
		virtual bool Reset();
		virtual bool Shutdown();

		// IObject
		virtual const char *GetObjectName() const;

	protected:
		mutable Array<ITexture *, 128> arTexture;

	private:
		SEED_DISABLE_COPY(D3D8RendererDevice);
		bool CheckExtension(const char *extName);

		D3DCAPS8		mCaps;
		LPDIRECT3D8		mObject;
		LPDIRECT3DDEVICE8	mDevice;
		D3DPRESENT_PARAMETERS	mParams;
		D3DDISPLAYMODE		mMode;

		bool			bLost;
};

}} // namespace

#else // SEED_ENABLE_D3D8
	#error "Include 'RendererDevice.h' instead 'api/directx/D3D8RendererDevice.h' directly."
#endif // SEED_ENABLE_D3D8

#endif // __D3D8_RENDERER_DEVICE_H__
