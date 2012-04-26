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

#include "interface/IRendererDevice.h"
#include "Log.h"

namespace Seed {

IRendererDevice::IRendererDevice()
{
}

IRendererDevice::~IRendererDevice()
{
}

void IRendererDevice::TextureUnload(ITexture *tex)
{
	UNUSED(tex);
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::TextureDataUpdate(ITexture *tex)
{
	UNUSED(tex);
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::SetTextureParameters(ITexture *texture) const
{
	UNUSED(texture);
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::SetBlendingOperation(eBlendMode mode, uPixel color) const
{
	UNUSED(mode);
	UNUSED(color);

	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::UploadData(void *userData)
{
	UNUSED(userData);
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::Begin() const
{
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::End() const
{
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::BackbufferClear(const uPixel color)
{
	UNUSED(color);
	SEED_ABSTRACT_METHOD;
}

bool IRendererDevice::IsRequired() const
{
	return true;
}

void IRendererDevice::TextureRequestAbort(ITexture *texture)
{
	UNUSED(texture);
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::TextureRequest(ITexture *texture)
{
	UNUSED(texture);
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::TextureRequestProcess() const
{
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::BackbufferFill(const uPixel color)
{
	UNUSED(color);
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::SetViewport(f32 x, f32 y, f32 w, f32 h) const
{
	UNUSED(x);
	UNUSED(y);
	UNUSED(w);
	UNUSED(h);
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::DrawRect(f32 x, f32 y, f32 w, f32 h, uPixel color, bool fill) const
{
	UNUSED(x);
	UNUSED(y);
	UNUSED(w);
	UNUSED(h);
	UNUSED(color);
	UNUSED(fill);
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::Enable2D() const
{
	SEED_ABSTRACT_METHOD;
}

void IRendererDevice::Disable2D() const
{
	SEED_ABSTRACT_METHOD;
}

bool IRendererDevice::NeedPowerOfTwoTextures() const
{
	return false;
}

void IRendererDevice::Update()
{
}

const String IRendererDevice::GetObjectName() const
{
	return "IRendererDevice";
}

} // namespace
