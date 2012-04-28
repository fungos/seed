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

#ifndef __PC_RENDERER_DEVICE_H__
#define __PC_RENDERER_DEVICE_H__

#include "Defines.h"

#if defined(BUILD_PC)

#include "Singleton.h"
#include "interface/IRendererDevice.h"

namespace Seed { namespace PC {

/// PC Rendering Device
class SEED_CORE_API RendererDevice : public IRendererDevice
{
	SEED_SINGLETON_DECLARE(RendererDevice)
	public:
		// IRendererDevice
		virtual void TextureUnload(ITexture *tex);
		virtual void TextureRequest(ITexture *texture);
		virtual void TextureRequestAbort(ITexture *texture);
		virtual void TextureRequestProcess() const;
		virtual void TextureDataUpdate(ITexture *texture);

		virtual void SetBlendingOperation(eBlendMode mode, const Color &color) const;
		virtual void UploadData(void *userData);
		virtual void BackbufferClear(const Color &color = Color(0, 0, 0, 0)) const;
		virtual void BackbufferFill(const Color &color = Color(0, 0, 0, 0)) const;

		virtual void Begin() const;
		virtual void End() const;

		virtual void SetViewport(f32 x, f32 y, f32 w, f32 h) const;
		virtual void DrawRect(f32 x, f32 y, f32 w, f32 h, const Color &color, bool fill = false) const;
		virtual void Enable2D() const;
		virtual void Disable2D() const;

		virtual bool NeedPowerOfTwoTextures() const;

		virtual void Update();

		// IModule
		virtual bool Initialize();
		virtual bool Reset();
		virtual bool Shutdown();

	private:
		SEED_DISABLE_COPY(RendererDevice);

		IRendererDevice cNull;
		IRendererDevice	*pApiDevice;
};

#define pRendererDevice Seed::PC::RendererDevice::GetInstance()

}} // namespace

#endif // BUILD_PC

#endif // __PC_RENDERER_DEVICE_H__
