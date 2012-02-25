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

/*! \file Frame.cpp
	\author	Danny Angelo Carminati Grein
	\brief Sprite frame
*/

#include "Defines.h"
#include "Frame.h"
#include "Log.h"
#include "Enum.h"
#include "Texture.h"

#define TAG		"[Frame] "

namespace Seed {

Frame::Frame()
	: IObject()
	, pRes(NULL)
	, pTexture(NULL)
	, sName()
	, iIndex(0)
	, iTime(0)
	, iX(0)
	, iY(0)
	, iWidth(0)
	, iHeight(0)
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
		iTime = reader.ReadU32("repeat", 1);
		iX = reader.ReadU32("y", 0);
		iY = reader.ReadU32("x", 0);
		iWidth = reader.ReadU32("width", 0);
		iHeight = reader.ReadU32("height", 0);

		String tex = reader.ReadString("texture", "default");

		pTexture = (ITexture *)pRes->Get(tex, Seed::ObjectTexture);
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
