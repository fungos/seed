/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

#include "Defines.h"
#include "Frame.h"
#include "Log.h"
#include "Enum.h"
#include "Texture.h"
#include "SeedInit.h"

#define TAG		"[Frame] "

namespace Seed {

Frame::Frame()
	: IObject()
	, pRes(NULL)
	, pTexture(NULL)
	, sName()
	, iIndex(0)
	, iX(0)
	, iY(0)
	, iWidth(0)
	, iHeight(0)
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

bool Frame::Load(Reader &reader, ResourceManager *res)
{
	ASSERT_NULL(res);

	if (this->Unload())
	{
		pRes = res;
		sName = reader.ReadString("name", "frame");

		String tex = reader.ReadString("texture", "default");
		pTexture = (ITexture *)pRes->Get(tex, Seed::ObjectTexture);

		iX = reader.ReadU32("y", 0);
		iY = reader.ReadU32("x", 0);
		iWidth = reader.ReadU32("width", pTexture->GetWidth());
		iHeight = reader.ReadU32("height", pTexture->GetHeight());

		u32 time = reader.ReadU32("repeat", 1);
		fFrameRate = 1.0f / static_cast<f32>(time);

		f32 rInvWidth = 1.0f / pTexture->GetAtlasWidth(); // full width from image, not only frame area
		f32 rInvHeight = 1.0f / pTexture->GetAtlasHeight(); // full height from image, not only frame area

		iHalfWidth = iWidth >> 1;
		iHalfHeight = iHeight >> 1;

		fTexS0 = static_cast<f32>((iX + 0.1f) * rInvWidth);
		fTexS1 = static_cast<f32>((iX + 0.1f + iWidth) * rInvWidth);
		fTexT0 = static_cast<f32>((iY + 0.1f) * rInvHeight);
		fTexT1 = static_cast<f32>((iY + 0.1f + iHeight) * rInvHeight);
	}

	return true;
}

bool Frame::Unload()
{
	if (pRes)
	{
		if (pTexture)
			sRelease(pTexture);

		pRes = NULL;
	}
	return true;
}

const char *Frame::GetObjectName() const
{
	return "Frame";
}

int Frame::GetObjectType() const
{
	return Seed::ObjectFrame;
}

} // namespace
