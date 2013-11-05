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

#include "Movie.h"
#include "Log.h"
#include "Memory.h"

#define TAG "[Movie] "

namespace Seed {

ISceneObject *FactoryMovie()
{
	return sdNew(Movie);
}

Movie::Movie()
	: fElapsedTime(0.0f)
	, vTimelines()
	, bPlaying(true)
{
	TimelineVector().swap(vTimelines);
}

Movie::~Movie()
{
	this->Unload();
}

bool Movie::Unload()
{
	while (!vTimelines.empty())
	{
		Timeline *obj = *vTimelines.begin();
		vTimelines.erase(vTimelines.begin());
		sdDelete(obj);
	}

	TimelineVector().swap(vTimelines);
	sName = this->GetTypeName();

	return true;
}

Movie *Movie::Clone() const
{
	auto obj = sdNew(Movie);
	obj->GenerateCloneName(sName);

	obj->fElapsedTime = fElapsedTime;
	obj->bPlaying = bPlaying;

	for (auto each: vTimelines)
		obj->vTimelines += each->Clone();

	// ISceneObject
	obj->bMarkForDeletion = true;

	// ITransformable
	obj->pParent = pParent;
	obj->mTransform = mTransform;
	obj->vPos = vPos;
	obj->vPivot = vPivot;
	obj->vTransformedPivot = vTransformedPivot;
	obj->vScale = vScale;
	obj->vBoundingBox = vBoundingBox;
	obj->fRotation = fRotation;
	obj->bTransformationChanged = bTransformationChanged;

	// IRenderable
	obj->nBlendOperation = nBlendOperation;
	obj->cColor = cColor;
	obj->bColorChanged = bColorChanged;
	obj->bVisible = bVisible;

	return obj;
}

void Movie::AddTimeline(Timeline *timeline)
{
	vTimelines += timeline;
	timeline->SetParent(this);
	this->Add(timeline->GetObject());
}

void Movie::Update(f32 delta)
{
	if (!bPlaying)
		return;

	fElapsedTime += delta;
	f32 frame = 1 / 60.0f;

	if (fElapsedTime >= frame)
	{
		fElapsedTime -= frame;

		for (auto obj: vTimelines)
		{
			if (bTransformationChanged)
			{
				obj->SetLocalPosition(this->GetPivotX(), this->GetPivotY());
				obj->SetPosition(this->GetX(), this->GetY());
				obj->SetScale(this->GetScaleX(), this->GetScaleY());
				obj->SetRotation(this->GetRotation());
			}

			obj->Update();
		}
	}

	ISceneNode::Update(delta);
}

void Movie::Render(const Matrix4f &)
{
}

void Movie::Play()
{
	fElapsedTime = 0.0f;
	bPlaying = true;
}

void Movie::Stop()
{
	fElapsedTime = 0.0f;
	bPlaying = false;
}

void Movie::Rewind()
{
	for (auto each: vTimelines)
		each->Rewind();
}

void Movie::Reset()
{
	for (auto each: vTimelines)
		each->Reset();
}

void Movie::Set(Reader &reader)
{
	sName = reader.ReadString("sName", sName.c_str());

	ITransformable::Unserialize(reader);
	IRenderable::Unserialize(reader);

	u32 timelines = reader.SelectArray("aTimelines");
	if (timelines)
	{
		for (u32 i = 0; i < timelines; i++)
		{
			reader.SelectNext();

			auto obj = sdNew(Timeline);
			obj->Load(reader, pRes);
			vTimelines += obj;

			auto o = obj->GetObject();
			this->Add(o);
		}
		reader.UnselectArray();
	}
	SEED_WARNING(!vTimelines.Size(), "Movie '%s': Has no timelines [aTimelines].", sName.c_str());
}

bool Movie::Write(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("sType", this->GetTypeName());
		writer.WriteString("sName", sName.c_str());

		ITransformable::Serialize(writer);
		IRenderable::Serialize(writer);

		writer.OpenArray("aTimelines");
		auto lines = u32(vTimelines.Size());
		for (u32 i = 0; i < lines; i++)
		{
			auto line = vTimelines[i];
			line->Write(writer);
		}
		writer.CloseArray();
	writer.CloseNode();

	return true;
}

} // namespace
