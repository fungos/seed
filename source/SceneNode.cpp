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

/*! \file SceneNode.cpp
	\author	Danny Angelo Carminati Grein
	\brief Scene Node object
*/

#include "SceneNode.h"
#include "Defines.h"
#include <algorithm>

#define TAG "[SceneNode] "

namespace Seed {

SceneNode::SceneNode()
	: vChild()
{
}

SceneNode::~SceneNode()
{
	ISceneObjectVector().swap(vChild);
}

void SceneNode::Reset()
{
	ISceneObjectVector().swap(vChild);
}

bool SceneNode::IsNode() const
{
	return true;
}

void SceneNode::Update(f32 dt)
{
	ForEach(ISceneObject, vChild,
	{
		(*it)->Update(dt);
	});
}

void SceneNode::Render()
{
}

void SceneNode::Add(ISceneObject *obj)
{
	VectorAdd(vChild, obj);
}

void SceneNode::Remove(ISceneObject *obj)
{
	VectorRemove(vChild, obj);
}

u32 SceneNode::Size() const
{
	return vChild.size();
}

ISceneObject *SceneNode::GetChildAt(u32 i)
{
	return vChild.at(i);
}

} // namespace
