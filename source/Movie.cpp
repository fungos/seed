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

#define TAG "[Movie] "

namespace Seed {

Movie::Movie()
	: fElapsedTime(0.0f)
	, vTimelines()
	, sName("")
	, bPlaying(true)
{
}

Movie::~Movie()
{
	this->Unload();
}

bool Movie::Unload()
{
	for (s32 i = vTimelines.Size(); i > 0; i--)
		Delete(vTimelines[i]);

	TimelineVector().swap(vTimelines);

	return true;
}

bool Movie::Load(Reader &reader, ResourceManager *res)
{
	SEED_ASSERT(res);

	bool ret = false;

	if (this->Unload())
	{
		sName = reader.ReadString("name", "movie");

		if (reader.SelectNode("position"))
		{
			vPos.setX(reader.ReadF32("x", 0.0f));
			vPos.setY(reader.ReadF32("y", 0.0f));
			reader.UnselectNode();
		}

		if (reader.SelectNode("pivot"))
		{
			vPivot.setX(reader.ReadF32("x", 0.0f));
			vPivot.setY(reader.ReadF32("y", 0.0f));
			reader.UnselectNode();
		}

		if (reader.SelectNode("scale"))
		{
			vScale.setX(reader.ReadF32("x", 1.0f));
			vScale.setY(reader.ReadF32("y", 1.0f));
			vScale.setZ(reader.ReadF32("z", 1.0f));
			reader.UnselectNode();
		}

		if (reader.SelectNode("color"))
		{
			iColor.rgba.r = reader.ReadS32("r", 255);
			iColor.rgba.g = reader.ReadS32("g", 255);
			iColor.rgba.b = reader.ReadS32("b", 255);
			iColor.rgba.a = reader.ReadS32("a", 255);
			reader.UnselectNode();
		}

		vPos.setZ(reader.ReadF32("priority", 0.0f));
		this->SetRotation(reader.ReadF32("rotation", 0.0f));

		u32 timelines = reader.SelectArray("timelines");
		if (timelines)
		{
			for (u32 i = 0; i < timelines; i++)
			{
				reader.SelectNext();

				Timeline *obj = New(Timeline);
				obj->Load(reader, res);
				vTimelines += obj;

				ISceneObject *o = obj->GetObject();
				o->SetParent(this);
				this->Add(o);
			}
			reader.UnselectArray();

			ret = true;
		}
		else
		{
			Log(TAG " WARNING: No timeline found in the movie '%s'", sName.c_str());
		}
	}

	return ret;
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

		//ForEach(TimelineVector, vTimelines,
		TimelineVectorIterator it = vTimelines.begin();
		TimelineVectorIterator end = vTimelines.end();
		for (; it != end; ++it)
		//for (int i = 0; i < vTimelines.Size(); i++)
		{
			Timeline *obj = (*it);
			//Timeline *obj = vTimelines[i];
			if (bTransformationChanged)
			{
				obj->SetLocalPosition(this->GetPivotX(), this->GetPivotY());
				obj->SetPosition(this->GetX(), this->GetY());
				obj->SetScale(this->GetScaleX(), this->GetScaleY());
				obj->SetRotation(this->GetRotation());
			}

			obj->Update();
		}//);
	}

	SceneNode::Update(delta);
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
	ForEach(TimelineVector, vTimelines,
	{
		(*it)->Rewind();
	});
}

void Movie::Reset()
{
	ForEach(TimelineVector, vTimelines,
	{
		(*it)->Reset();
	});
}

const char *Movie::GetObjectName() const
{
	return "Movie";
}

int Movie::GetObjectType() const
{
	return Seed::ObjectMovie;
}

} // namespace
