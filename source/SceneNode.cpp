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
#include "PrefabManager.h"
#include "SceneObjectFactory.h"
#include "Memory.h"
#include <algorithm>

#define TAG "[SceneNode] "

namespace Seed {

ISceneObject *FactorySceneNode()
{
	return sdNew(SceneNode);
}

ISceneNode::ISceneNode()
	: vChild()
{
}

ISceneNode::~ISceneNode()
{
	this->Unload();
}

bool ISceneNode::IsNode() const
{
	return true;
}

void ISceneNode::Update(Seconds dt)
{
	for (auto obj: vChild)
		obj->Update(dt);

	this->UpdateTransform();
}

void ISceneNode::Render(const mat4 &worldTransform)
{
	UNUSED(worldTransform)
}

void ISceneNode::Add(ISceneObject *obj)
{
	SEED_ASSERT_MSG(obj, "Cannot add null child to a scene.");
	vChild += obj;
	obj->SetParent(this);
}

void ISceneNode::Remove(ISceneObject *obj)
{
	if (obj == nullptr)
		return;

	obj->SetParent(nullptr);
	vChild -= obj;
}

u32 ISceneNode::Size() const
{
	return (u32)vChild.Size();
}

ISceneObject *ISceneNode::GetChildAt(u32 i) const
{
	return vChild.at(i);
}

ISceneObject *ISceneNode::GetChildByName(const String &name) const
{
	for (auto obj: vChild)
	{
		if (obj->sName == name)
			return obj;
	}

	for (auto each: vChild)
	{
		if (each->IsKindOf<ISceneNode>())
		{
			auto obj = static_cast<ISceneNode *>(each);
			return obj->GetChildByName(name);
		}
	}

	return nullptr;
}

bool ISceneNode::Unload()
{
	auto ret = true;
	for (auto obj: vChild)
	{
//		Log("Unloading scene node %s.", obj->sName.c_str());
		obj->Unload();
		if (obj->bMarkForDeletion)
			sdDelete(obj);
	}

	ISceneObjectVector().swap(vChild);
	sName = this->GetTypeName();

	return ret;
}

void ISceneNode::Dump(u32 level)
{
	for (auto obj: vChild)
	{
		for (decltype(level) i = 0; i < level + 1; i++) fprintf(stdout, "-");
		Log(" %s (Parented: %s, Pos: %d,%d Z: %d)", obj->sName.c_str(), (obj->GetParent() ? "Yes" : "No"), s32(obj->GetX()), s32(obj->GetY()), s32(obj->GetZ()));

		if (obj->IsNode())
			(static_cast<ISceneNode *>(obj))->Dump(level + 1);
	}
}

void ISceneNode::Reset()
{
	for (auto obj: vChild)
		obj->SetParent(nullptr);

	ISceneObjectVector().swap(vChild);
}

void SceneNode::Set(Reader &reader)
{
	sName = reader.ReadString("sName", sName.c_str());
	auto objs = reader.SelectArray("aObjects");
	if (objs)
	{
		for (decltype(objs) i = 0; i < objs; i++)
		{
			reader.SelectNext();
			auto obj = pSceneObjectFactory->Load(reader, pRes);
			vChild += static_cast<ISceneObject *>(obj);
		}
		reader.UnselectArray();
	}
}

bool SceneNode::Write(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("sType", this->GetTypeName());
		writer.WriteString("sName", sName.c_str());

		writer.OpenArray("aObjects");
		auto objects  = vChild.Size();
		for (decltype(objects) i = 0; i < objects; i++)
		{
			auto obj = vChild[i];
			obj->Write(writer);
		}
		writer.CloseArray();
	writer.CloseNode();

	return true;
}

SceneNode *SceneNode::Clone() const
{
	auto obj = sdNew(SceneNode);
	obj->GenerateCloneName(sName);

	for (auto child: vChild)
	{
		auto cln = static_cast<ISceneObject *>(child->Clone());
		obj->vChild += cln;
	}

	return obj;
}

} // namespace
