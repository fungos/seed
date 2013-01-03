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

#include "interface/ITexture.h"
#include "Enum.h"
#include "Log.h"
#include "RendererDevice.h"
#include "ResourceManager.h"

namespace Seed {

ITexture::ITexture()
	: pTextureId(NULL)
	, iTextureId(0)
	, nTextureCompression(TextureCompressionNone)
	, pFile(NULL)
	, nMinFilter(Seed::TextureFilterLinear)
	, nMagFilter(Seed::TextureFilterNearest)
	, iWidth(0)
	, iHeight(0)
	, iRenderTargetId(0)
	, iDepthTargetId(0)
{
}

ITexture::~ITexture()
{
	this->Reset();
}

void ITexture::Reset()
{
	Delete(pFile);

	iWidth = 0;
	iHeight = 0;

	nMinFilter = Seed::TextureFilterLinear;
	nMagFilter = Seed::TextureFilterNearest;

	this->DisableRenderTarget();
}

File *ITexture::GetFile()
{
	return pFile;
}

const void *ITexture::GetData() const
{
	SEED_ABSTRACT_METHOD;
	return NULL;
}

void ITexture::PutPixel(u32 x, u32 y, const Color &px)
{
	UNUSED(x)
	UNUSED(y)
	UNUSED(px)

	SEED_ABSTRACT_METHOD;
}

Color ITexture::GetPixel(u32 x, u32 y) const
{
	Color px;
	UNUSED(x)
	UNUSED(y)

	SEED_ABSTRACT_METHOD
	return px;
}

u8 ITexture::GetPixelAlpha(u32 x, u32 y) const
{
	UNUSED(x)
	UNUSED(y)

	SEED_ABSTRACT_METHOD
	return 0;
}

u32 ITexture::GetAtlasWidth() const
{
	return iWidth;
}

u32 ITexture::GetAtlasHeight() const
{
	return iHeight;
}

u32 ITexture::GetWidth() const
{
	return iWidth;
}

u32 ITexture::GetHeight() const
{
	return iHeight;
}

eTextureCompression ITexture::GetCompressionType() const
{
	return nTextureCompression;
}

void ITexture::Close()
{
	Delete(pFile);
}

bool ITexture::Unload()
{
	Delete(pFile);
	return true;
}

bool ITexture::Load(const String &filename, ResourceManager *res)
{
	SEED_ASSERT(res);
	pRes = res;

	bool ret = false;
	if (this->Unload())
	{
		pRes = res;
		sFilename = filename;

		#warning TODO - Move to async file loading
		pFile = New(File(filename));
		ret = (pFile->GetData() != NULL);
	}

	return ret;
}

bool ITexture::Load(const String &desc, u32 width, u32 height, Color *buffer, u32 atlasWidth, u32 atlasHeight, bool copy)
{
	UNUSED(desc)
	UNUSED(width)
	UNUSED(height)
	UNUSED(buffer)
	UNUSED(atlasWidth)
	UNUSED(atlasHeight)
	UNUSED(copy)

	SEED_ABSTRACT_METHOD
	return true;
}

void ITexture::SetFilter(eTextureFilterType type, eTextureFilter filter)
{
	if (type == Seed::TextureFilterTypeMin)
		nMinFilter = filter;
	else
		nMagFilter = filter;
}

eTextureFilter ITexture::GetFilter(eTextureFilterType type) const
{
	eTextureFilter filter = nMinFilter;

	if (type == Seed::TextureFilterTypeMag)
		filter = nMagFilter;

	return filter;
}

u32 ITexture::GetBytesPerPixel() const
{
	return 0;
}

void ITexture::Update(Color *buffer)
{
	UNUSED(buffer);
	SEED_ABSTRACT_METHOD;
}

bool ITexture::EnableRenderTarget(bool useDepthBuffer)
{
	this->DisableRenderTarget();

	bool ret = false;
	if (iTextureId)
	{
		iRenderTargetId = pRendererDevice->CreateFrameBuffer(this);

		if (useDepthBuffer)
		{
			iDepthTargetId = pRendererDevice->CreateDepthBuffer(iWidth, iHeight);
			pRendererDevice->AttachDepthBuffer(iDepthTargetId);
			pRendererDevice->ActivateDepthBuffer();
		}

		if (!(ret = pRendererDevice->CheckFrameBufferStatus()))
			this->DisableRenderTarget();

		pRendererDevice->ActivateFrameBuffer();
	}

	return ret;
}

void ITexture::DisableRenderTarget()
{
	if (iDepthTargetId)
		pRendererDevice->DestroyDepthBuffer(iDepthTargetId);

	if (iRenderTargetId)
		pRendererDevice->DestroyFrameBuffer(iRenderTargetId);
}

u32 ITexture::GetRenderTarget() const
{
	return iRenderTargetId;
}

int ITexture::GetObjectType() const
{
	return Seed::TypeTexture;
}

const String ITexture::GetClassName() const
{
	return "ITexture";
}

} // namespace
