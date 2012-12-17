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

#include "SceneNode.h"
#include "Defines.h"
#include "SceneObjectFactory.h"
#include <algorithm>

#define TAG "[SceneNode] "

namespace Seed {

ISceneObject *FactorySceneNode()
{
	return New(SceneNode());
}

SceneNode::SceneNode()
	: vChild()
{
}

SceneNode::~SceneNode()
{
	this->Unload();
}

bool SceneNode::IsNode() const
{
	return true;
}

void SceneNode::Update(f32 dt)
{
	ISceneObjectVectorIterator it = vChild.begin();
	ISceneObjectVectorIterator end = vChild.end();
	for (; it != end; ++it)
	{
		(*it)->Update(dt);
	}

	bTransformationChanged = false;
}

void SceneNode::Render(const Matrix4f &worldTransform)
{
	UNUSED(worldTransform)
}

void SceneNode::Add(ISceneObject *obj)
{
	SEED_ASSERT_MSG(obj, "Cannot add null child to a scene.");
	vChild += obj;
	obj->SetParent(this);
}

void SceneNode::Remove(ISceneObject *obj)
{
	obj->SetParent(NULL);
	vChild -= obj;
}

u32 SceneNode::Size() const
{
	return (u32)vChild.Size();
}

ISceneObject *SceneNode::GetChildAt(u32 i)
{
	return vChild.at(i);
}

ISceneObject *SceneNode::GetChildByName(String name)
{
	ISceneObjectVectorIterator it = vChild.begin();
	ISceneObjectVectorIterator end = vChild.end();
	for (; it != end; ++it)
	{
		ISceneObject *obj = (*it);
		if (obj->sName == name)
			return obj;
	}

	it = vChild.begin();
	end = vChild.end();
	for (; it != end; ++it)
	{
		if ((*it)->GetObjectType() == Seed::TypeScene)
		{
			SceneNode *obj = (SceneNode *)(*it);
			return obj->GetChildByName(name);
		}
	}

	return NULL;
}

bool SceneNode::Load(Reader &reader, ResourceManager *res)
{
	bool ret = false;
	if (this->Unload())
	{
		sName = reader.ReadString("sName", "node");
		u32 objs = reader.SelectArray("aObjects");
		if (objs)
		{
			for (u32 i = 0; i < objs; i++)
			{
				reader.SelectNext();
				IDataObject *obj = pSceneObjectFactory->Load(reader, res);
				vChild += static_cast<ISceneObject *>(obj);
			}
			reader.UnselectArray();
		}

		ret = true;
	}

	return ret;
}

bool SceneNode::Write(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("sType", this->GetClassName().c_str());
		writer.WriteString("sName", sName.c_str());

		writer.OpenArray("aObjects");
		u32 objects  = (u32)vChild.Size();
		for (u32 i = 0; i < objects; i++)
		{
			IDataObject *obj = vChild[i];
			obj->Write(writer);
		}
		writer.CloseArray();
	writer.CloseNode();

	return true;
}

bool SceneNode::Unload()
{
	bool ret = true;

	ISceneObjectVectorIterator it = vChild.begin();
	ISceneObjectVectorIterator end = vChild.end();
	for (; it != end; ++it)
	{
		ISceneObject *obj = (*it);
		obj->Unload();
		if (obj->bMarkForDeletion)
			Delete(obj);
	}

	ISceneObjectVector().swap(vChild);

	return ret;
}

void SceneNode::Reset()
{
	ISceneObjectVectorIterator it = vChild.begin();
	ISceneObjectVectorIterator end = vChild.end();
	for (; it != end; ++it)
	{
		ISceneObject *obj = (*it);
		obj->SetParent(NULL);
	}

	ISceneObjectVector().swap(vChild);
}

const String SceneNode::GetClassName() const
{
	return "SceneNode";
}

int SceneNode::GetObjectType() const
{
	return Seed::TypeScene;
}

} // namespace
