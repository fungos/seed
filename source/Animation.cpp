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

/*! \file Animation.cpp
	\author	Danny Angelo Carminati Grein
	\brief Sequence of sprite frames
*/

#include "Defines.h"
#include "Animation.h"
#include "Log.h"
#include "Enum.h"
#include "Frame.h"

#define TAG		"[Animation] "

namespace Seed {

Animation::Animation()
	: IObject()
	, ppFrames(NULL)
	, sName()
	, iIndex(0)
	, iFrames(0)
	, iAnimationId(0)
	, iFps(60)
	, bAnimated(true)
	, bLoop(true)
{
}

Animation::~Animation()
{
}

bool Animation::Load(Reader &reader, ResourceManager *res)
{
	ASSERT_NULL(res);

	if (this->Unload())
	{
		pRes = res;
		sName = reader.ReadString("name", "animation");
		bAnimated = reader.ReadBool("animated", true);
		bLoop = reader.ReadBool("loop", true);
		iFps = reader.ReadU32("fps", 60);
		iFrames = reader.SelectArray("frames");
	}

	return true;
}

bool Animation::Unload()
{
	return true;
}

Frame **Animation::GetFrames() const
{
	return ppFrames;
}

u32 Animation::GetFrameCount() const
{
	return iFrames;
}

const char *Animation::GetObjectName() const
{
	return "Animation";
}

int Animation::GetObjectType() const
{
	return Seed::ObjectAnimation;
}

} // namespace
