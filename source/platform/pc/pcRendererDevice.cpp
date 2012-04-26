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

#include "RendererDevice.h"

#if defined(BUILD_PC)

#include "SeedInit.h"
#define TAG "[RendererDevice] "

namespace Seed { namespace PC {

SEED_SINGLETON_DEFINE(RendererDevice)

RendererDevice::RendererDevice()
	: IRendererDevice()
	, cNull()
	, pApiDevice(&cNull)
{
	cNull.Disable();
}

RendererDevice::~RendererDevice()
{
	if (pApiDevice != &cNull)
		Delete(pApiDevice);

	pApiDevice = NULL;
}

bool RendererDevice::Initialize()
{
	if (pApiDevice != &cNull && pApiDevice != NULL)
		Delete(pApiDevice);

	Log(TAG "Initializing...");

	switch (pConfiguration->GetRendererDeviceType())
	{
#if defined(SEED_ENABLE_OGL20)
		case RendererDeviceOpenGL20:
		{
			Info(TAG "Creating renderer device OpenGL 2.0");
			pApiDevice = New(OGL20RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_OGL30)
		case RendererDeviceOpenGL30:
		{
			Info(TAG "Creating renderer device OpenGL 3.0");
			pApiDevice = New(OGL30RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_OGL40)
		case RendererDeviceOpenGL40:
		{
			Info(TAG "Creating renderer device OpenGL 4.0");
			pApiDevice = New(OGL40RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_D3D8)
		case RendererDeviceDirectX8:
		{
			Info(TAG "Creating renderer device DirectX 8.1");
			pApiDevice = New(Seed::DirectX::D3D8RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_D3D9)
		case RendererDeviceDirectX9:
		{
			Info(TAG "Creating renderer device DirectX 9.0c");
			pApiDevice = New(D3D9RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_D3D10)
		case RendererDeviceDirectX10:
		{
			Info(TAG "Creating renderer device DirectX 10");
			pApiDevice = New(D3D10RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_D3D11)
		case RendererDeviceDirectX11:
		{
			Info(TAG "Creating renderer device DirectX 11");
			pApiDevice = New(D3D11RendererDevice());
		}
		break;
#endif

		case RendererDeviceOpenGLES1:
		case RendererDeviceOpenGL14:
		default:
		{
			Info(TAG "Creating renderer device OpenGL 1.5/ES 1");
			pApiDevice = New(Seed::OpenGL::OGLES1RendererDevice());
		}
		break;
	}

	bool ret = pApiDevice->Initialize();

	return ((pApiDevice != &cNull) && ret);
}

bool RendererDevice::Reset()
{
	return pApiDevice->Reset();
}

bool RendererDevice::Shutdown()
{
	bool ret = pApiDevice->Shutdown();
	Delete(pApiDevice);
	pApiDevice = &cNull;

	return ret;
}

void RendererDevice::TextureRequest(ITexture *texture)
{
	pApiDevice->TextureRequest(texture);
}

void RendererDevice::TextureRequestAbort(ITexture *texture)
{
	pApiDevice->TextureRequestAbort(texture);
}

void RendererDevice::TextureRequestProcess() const
{
	pApiDevice->TextureRequestProcess();
}

void RendererDevice::TextureUnload(ITexture *tex)
{
	pApiDevice->TextureUnload(tex);
}

void RendererDevice::TextureDataUpdate(ITexture *texture)
{
	pApiDevice->TextureDataUpdate(texture);
}

void RendererDevice::SetBlendingOperation(eBlendMode mode, uPixel color) const
{
	pApiDevice->SetBlendingOperation(mode, color);
}

void RendererDevice::UploadData(void *userData)
{
	pApiDevice->UploadData(userData);
}

void RendererDevice::BackbufferClear(const uPixel color)
{
	pApiDevice->BackbufferClear(color);
}

void RendererDevice::BackbufferFill(const uPixel color)
{
	pApiDevice->BackbufferFill(color);
}

void RendererDevice::Begin() const
{
	pApiDevice->Begin();
}

void RendererDevice::End() const
{
	pApiDevice->End();
}

void RendererDevice::SetViewport(f32 x, f32 y, f32 w, f32 h) const
{
	pApiDevice->SetViewport(x, y, w, h);
}

void RendererDevice::DrawRect(f32 x, f32 y, f32 w, f32 h, uPixel color, bool fill) const
{
	pApiDevice->DrawRect(x, y, w, h, color, fill);
}

void RendererDevice::Enable2D() const
{
	pApiDevice->Enable2D();
}

void RendererDevice::Disable2D() const
{
	pApiDevice->Disable2D();
}

bool RendererDevice::NeedPowerOfTwoTextures() const
{
	return pApiDevice->NeedPowerOfTwoTextures();
}

void RendererDevice::Update()
{
	pApiDevice->Update();
}

}} // namespace

#endif // BUILD_PC
