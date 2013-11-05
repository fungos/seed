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

#include "Defines.h"
#include "Frame.h"
#include "Log.h"
#include "Enum.h"
#include "Texture.h"
#include "Memory.h"
#include "SeedInit.h"

#define TAG		"[Frame] "

namespace Seed {

Frame::Frame()
	: pTexture(nullptr)
	, sName()
	, sTexture()
	, iFps(0)
	, iIndex(0)
	, iX(0)
	, iY(0)
	, iWidth(0)
	, iHeight(0)
	, iHalfWidth(0)
	, iHalfHeight(0)
	, fFrameRate(0.0f)
	, fTexS0(0.0f)
	, fTexS1(0.0f)
	, fTexT0(0.0f)
	, fTexT1(0.0f)
{
}

Frame::~Frame()
{
	this->Unload();
}

void Frame::Configure()
{
	if (iFps)
		fFrameRate = 1.0f / static_cast<f32>(iFps);

	auto rInvWidth = 1.0f / pTexture->GetAtlasWidth(); // full width from image, not only frame area
	auto rInvHeight = 1.0f / pTexture->GetAtlasHeight(); // full height from image, not only frame area

	iHalfWidth = iWidth >> 1;
	iHalfHeight = iHeight >> 1;

	fTexS0 = static_cast<f32>((iX + 0.1f) * rInvWidth);
	fTexS1 = static_cast<f32>((iX + 0.1f + iWidth) * rInvWidth);
	fTexT0 = static_cast<f32>((iY + 0.1f) * rInvHeight);
	fTexT1 = static_cast<f32>((iY + 0.1f + iHeight) * rInvHeight);
}

void Frame::Set(Reader &reader)
{
	sName = reader.ReadString("sName", sName.c_str());

	auto tex = String(reader.ReadString("sResource", sTexture.c_str()));
	if (tex != sTexture)
	{
		sdRelease(pTexture);
		tex = sTexture;
		pTexture = static_cast<ITexture *>(pRes->Get(sTexture, ITexture::GetTypeId()));
		iX = 0;
		iY = 0;
		iWidth = pTexture->GetWidth();
		iHeight = pTexture->GetHeight();
	}

	if (reader.SelectNode("cBoundary"))
	{
		iX = reader.ReadU32("iX", 0);
		iY = reader.ReadU32("iY", 0);
		iWidth = reader.ReadU32("iWidth", iWidth);
		iHeight = reader.ReadU32("iHeight", iHeight);
		reader.UnselectNode();
	}

	iFps = reader.ReadU32("iFps", iFps);

	this->Configure();
}

bool Frame::Load(Reader &reader, ResourceManager *res)
{
	SEED_ASSERT(res);

	if (!this->Unload())
		return false;

	sTexture = reader.ReadString("sResource", sTexture.c_str());
	SEED_ASSERT_MSG(sTexture != "", "sResource not set in frame");

	pTexture = static_cast<ITexture *>(res->Get(sTexture, ITexture::GetTypeId()));
	iX = 0;
	iY = 0;
	iWidth = pTexture->GetWidth();
	iHeight = pTexture->GetHeight();

	this->Set(reader);

	return true;
}

bool Frame::Write(Writer &writer)
{
	auto ret = false;

	writer.OpenNode();
		writer.WriteString("sType", this->GetTypeName());
		writer.WriteString("sName", sName.c_str());
		writer.WriteString("sResource", sTexture.c_str());
		if (iFps)
			writer.WriteU32("iFps", iFps);

		if (iX != 0 && iY != 0 && iWidth != pTexture->GetWidth() && iHeight != pTexture->GetHeight())
		{
			writer.OpenNode("cBoundary");
				writer.WriteU32("x", iX);
				writer.WriteU32("y", iY);
				writer.WriteU32("width", iWidth);
				writer.WriteU32("height", iHeight);
			writer.CloseNode();
		}
	writer.CloseNode();

	return ret;
}

bool Frame::Unload()
{
	if (pTexture)
		sdRelease(pTexture);

	pTexture = nullptr;
	sName = this->GetTypeName();
	sTexture = "";
	iFps = 0;
	iIndex = 0;
	iX = 0;
	iY = 0;
	iWidth = 0;
	iHeight = 0;
	fFrameRate = 0.0f;
	fTexS0 = 0.0f;
	fTexS1 = 0.0f;
	fTexT0 = 0.0f;
	fTexT1 = 0.0f;

	return true;
}

Frame *Frame::Clone() const
{
	auto obj = sdNew(Frame);
	obj->GenerateCloneName(sName);

	sdAcquire(pTexture);
	obj->pTexture = pTexture;
	obj->sTexture = sTexture;
	obj->iFps = iFps;
	obj->iIndex = iIndex;
	obj->iX = iX;
	obj->iY = iY;
	obj->iWidth = iWidth;
	obj->iHeight = iHeight;
	obj->iHalfWidth = iHalfWidth;
	obj->iHalfHeight = iHalfHeight;
	obj->fFrameRate = fFrameRate;
	obj->fTexS0 = fTexS0;
	obj->fTexS1 = fTexS1;
	obj->fTexT0 = fTexT0;
	obj->fTexT1 = fTexT1;

	return obj;
}

} // namespace
