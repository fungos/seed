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

#include "Configuration.h"
#include "Memory.h"
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
		sdDelete(pApiDevice);

	pApiDevice = nullptr;
}

bool RendererDevice::Initialize()
{
	if (pApiDevice != &cNull && pApiDevice != nullptr)
		sdDelete(pApiDevice);

	Log(TAG "Initializing...");

	eRendererDeviceType renderDevice = pConfiguration->GetRendererDeviceType();
	if (renderDevice == eRendererDeviceType::Auto)
	{
#if defined(SEED_ENABLE_D3D11) || defined(SEED_ENABLE_D3D10) || defined(SEED_ENABLE_D3D9) || defined(SEED_ENABLE_D3D8)
		renderDevice = Seed::RendererDeviceDirectXAny;
#else
		renderDevice = eRendererDeviceType::OpenGLAny;
#endif
	}

	if (renderDevice == eRendererDeviceType::OpenGLAny)
	{
		char *version = (char *)glGetString(GL_VERSION);
		switch (version[0])
		{
			case '4': renderDevice = eRendererDeviceType::OpenGL4x; break;
			case '3': renderDevice = eRendererDeviceType::OpenGL3x; break;
			case '2': renderDevice = eRendererDeviceType::OpenGL2x; break;
			default: renderDevice = eRendererDeviceType::OpenGL1x; break;
		}
	}


	switch (renderDevice)
	{
#if defined(SEED_ENABLE_OGL20)
		case eRendererDeviceType::OpenGL2x:
		{
			Info(TAG "Creating renderer device OpenGL 2.x");
			pApiDevice = sdNew(Seed::OpenGL::OGL20RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_OGL30)
		case eRendererDeviceType::OpenGL3x:
		{
			Info(TAG "Creating renderer device OpenGL 3.x");
			pApiDevice = sdNew(OGL3xRendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_OGL40)
		case eRendererDeviceType::OpenGL4x:
		{
			Info(TAG "Creating renderer device OpenGL 4.x");
			pApiDevice = sdNew(OGL4xRendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_D3D8)
		case eRendererDeviceType::DirectX8:
		{
			Info(TAG "Creating renderer device DirectX 8.1");
			pApiDevice = sdNew(Seed::DirectX::D3D8RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_D3D9)
		case eRendererDeviceType::DirectX9:
		{
			Info(TAG "Creating renderer device DirectX 9.0c");
			pApiDevice = sdNew(D3D9RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_D3D10)
		case eRendererDeviceType::DirectX10:
		{
			Info(TAG "Creating renderer device DirectX 10");
			pApiDevice = sdNew(D3D10RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_D3D11)
		case eRendererDeviceType::DirectX11:
		{
			Info(TAG "Creating renderer device DirectX 11");
			pApiDevice = sdNew(D3D11RendererDevice());
		}
		break;
#endif

#if defined(SEED_ENABLE_OGLES2)
		case eRendererDeviceType::OpenGLES2:
		{
			Info(TAG "Creating renderer device OpenGLES 2");
			pApiDevice = sdNew(Seed::OpenGL::OGLES2RendererDevice());
		}
		break;
#else
		case eRendererDeviceType::OpenGLES1:
		case eRendererDeviceType::OpenGL1x:

		default:
		{
			Info(TAG "Creating renderer device OpenGL 1.5/ES 1");
			pApiDevice = sdNew(Seed::OpenGL::OGLES1RendererDevice);
		}
		break;
#endif
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
	sdDelete(pApiDevice);
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

void RendererDevice::DestroyHardwareBuffer(IHardwareBuffer *buf) const
{
	pApiDevice->DestroyHardwareBuffer(buf);
}

void RendererDevice::SetBlendingOperation(eBlendMode mode, const Color &color) const
{
	pApiDevice->SetBlendingOperation(mode, color);
}

void RendererDevice::UploadData(void *userData)
{
	pApiDevice->UploadData(userData);
}

void RendererDevice::BackbufferClear(const Color &color) const
{
	pApiDevice->BackbufferClear(color);
}

void RendererDevice::BackbufferFill(const Color &color) const
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

u32 RendererDevice::CreateFrameBuffer(ITexture *texture)
{
	return pApiDevice->CreateFrameBuffer(texture);
}

void RendererDevice::DestroyFrameBuffer(u32 buffer)
{
	pApiDevice->DestroyFrameBuffer(buffer);
}

u32 RendererDevice::CreateDepthBuffer(u32 w, u32 h)
{
	return pApiDevice->CreateDepthBuffer(w, h);
}

void RendererDevice::DestroyDepthBuffer(u32 buffer)
{
	pApiDevice->DestroyDepthBuffer(buffer);
}

void RendererDevice::AttachDepthBuffer(u32 buffer)
{
	pApiDevice->AttachDepthBuffer(buffer);
}

void RendererDevice::ActivateFrameBuffer(u32 buffer)
{
	pApiDevice->ActivateFrameBuffer(buffer);
}

void RendererDevice::ActivateDepthBuffer(u32 buffer)
{
	pApiDevice->ActivateDepthBuffer(buffer);
}

bool RendererDevice::CheckFrameBufferStatus() const
{
	return pApiDevice->CheckFrameBufferStatus();
}

void RendererDevice::EnableScissor(bool b) const
{
	pApiDevice->EnableScissor(b);
}

void RendererDevice::SetScissor(f32 x, f32 y, f32 w, f32 h) const
{
	pApiDevice->SetScissor(x, y, w, h);
}

void RendererDevice::SetViewport(f32 x, f32 y, f32 w, f32 h) const
{
	pApiDevice->SetViewport(x, y, w, h);
}

f32 RendererDevice::GetHorizontalTexelOffset() const
{
	return pApiDevice->GetHorizontalTexelOffset();
}

f32 RendererDevice::GetVerticalTexelOffset() const
{
	return pApiDevice->GetVerticalTexelOffset();
}

void RendererDevice::DrawRect(f32 x, f32 y, f32 w, f32 h, const Color &color, bool fill) const
{
	pApiDevice->DrawRect(x, y, w, h, color, fill);
}

void RendererDevice::DrawCircle(f32 x, f32 y, f32 radius, const Color &color) const
{
	pApiDevice->DrawCircle(x, y, radius, color);
}

void RendererDevice::DrawLines(f32 *points, u32 len, const Color &color) const
{
	pApiDevice->DrawLines(points, len, color);
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
