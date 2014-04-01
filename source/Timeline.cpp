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

#include "Timeline.h"
#include "Sprite.h"
#include "EventMovie.h"
#include "interface/IEventMovieListener.h"
#include "Movie.h"
#include "Keyframe.h"
#include "Log.h"
#include "ResourceManager.h"
#include "SceneObjectFactory.h"
#include "PrefabManager.h"
#include "Memory.h"

#define TAG "[Timeline] "

namespace Seed {

Timeline::Timeline()
	: pRes(nullptr)
	, pParent(nullptr)
	, pObject(nullptr)
	, pListener(nullptr)
	, sName()
	, fElapsedTime(0.0f)
	, fElapsedKeyframeTime(0.0f)
	, iCurrentFrame(0)
	, iKeyframeFrom(0)
	, iKeyframeTo(0)
	, iPriority(0)
	, ptParentPosition()
	, ptParentLocalPosition()
	, ptParentScale()
	, fParentRotation(0.0f)
	, mapKeyframes()
{
}

Timeline::~Timeline()
{
	this->Unload();
}

void Timeline::Reset()
{
	fElapsedTime 			= 0.0f;
	fElapsedKeyframeTime 	= 0.0f;
	iCurrentFrame 			= 0;
	iKeyframeFrom 			= 0;
	iKeyframeTo 			= 0;
	sName					= this->GetTypeName();
	pParent					= nullptr;
}

void Timeline::Rewind()
{
	iCurrentFrame			= 0;
	iKeyframeFrom 			= 0;
	iKeyframeTo 			= 0;
	fElapsedTime 			= 0.0f;
	fElapsedKeyframeTime 	= 0.0f;
}

void Timeline::AddKeyframe(Keyframe *keyframe)
{
	if (mapKeyframes[keyframe->iFrame])
		sdDelete(mapKeyframes[keyframe->iFrame]);

	mapKeyframes[keyframe->iFrame] = keyframe;
}

void Timeline::GotoAndPlay(u32 frame)
{
	iCurrentFrame	= frame;
	iKeyframeFrom	= FindPreviousKeyframe();
	iKeyframeTo		= FindNextKeyframe();
}

void Timeline::GotoAndPlay(const char *strKeyframeName)
{
	iKeyframeFrom	= FindKeyframeByName(strKeyframeName);
	iKeyframeTo		= FindNextKeyframe();
	iCurrentFrame	= iKeyframeFrom;
}

void Timeline::Update()
{
	if (!pObject ||  mapKeyframes.size() < 2)
		return;

	//attempt to get the next keyframe
	if (iKeyframeTo <= 0)
	{
		iKeyframeTo = FindNextKeyframe();
		//end of animation, restart
		if (iKeyframeTo < 0)
		{
			if (pListener)
			{
				EventMovie ev(this, mapKeyframes[iCurrentFrame], static_cast<u32>(iCurrentFrame));
				pListener->OnTimelineFrame(&ev);
				pListener->OnTimelineRestart(&ev);
			}

			iKeyframeFrom = 0;
			iCurrentFrame = 0;
			this->Update();
			return;
		}
	}

	//start interpolating keyframes
	Keyframe *kfFrom	= mapKeyframes[iKeyframeFrom];
	Keyframe *kfTo 		= mapKeyframes[iKeyframeTo];

	SEED_ASSERT_MSG(kfFrom, 	"A keyframe is required at frame 0.");
	SEED_ASSERT_MSG(kfTo, 	"At least two keyframes must be set.");

	if (pListener)
	{
		EventMovie ev(this, kfFrom, static_cast<u32>(iCurrentFrame));
		pListener->OnTimelineFrame(&ev);
	}

	f32 fBegin 		= static_cast<f32>(iCurrentFrame - iKeyframeFrom);
	f32 fDuration 	= static_cast<f32>(iKeyframeTo - iKeyframeFrom);

	// Raptor note: Timeline should only change the position and orientation of the object NEVER its visibility or rendering state
	// pObject->SetVisible(!kfFrom->bBlank);
	if (!kfFrom->bBlank)
	{
		//calculate the interpolated values
		f32 fCurrRot,
			fCurrPosX,
			fCurrPosY,
			fCurrPivotX,
			fCurrPivotY,
			fCurrScaleX,
			fCurrScaleY,
			fCurrR,
			fCurrG,
			fCurrB,
			fCurrA;

		if (kfFrom->bTween && !kfTo->bBlank)
		{
			fCurrRot		= EaseQuadPercent(	fBegin,
												kfFrom->fRotation,
												(kfTo->fRotation - kfFrom->fRotation),
												fDuration,
												kfFrom->fEasing);

			fCurrPosX		= EaseQuadPercent(	fBegin,
												kfFrom->ptPos.x,
												(kfTo->ptPos.x - kfFrom->ptPos.x),
												fDuration,
												kfFrom->fEasing);


			fCurrPosY		= EaseQuadPercent(	fBegin,
												kfFrom->ptPos.y,
												(kfTo->ptPos.y - kfFrom->ptPos.y),
												fDuration,
												kfFrom->fEasing);

			fCurrPivotX		= EaseQuadPercent(	fBegin,
												kfFrom->ptPivot.x,
												(kfTo->ptPivot.x - kfFrom->ptPivot.x),
												fDuration,
												kfFrom->fEasing);


			fCurrPivotY		= EaseQuadPercent(	fBegin,
												kfFrom->ptPivot.y,
												(kfTo->ptPivot.y - kfFrom->ptPivot.y),
												fDuration,
												kfFrom->fEasing);

			fCurrScaleX		= EaseQuadPercent(	fBegin,
												kfFrom->ptScale.x,
												(kfTo->ptScale.x - kfFrom->ptScale.x),
												fDuration,
												kfFrom->fEasing);

			fCurrScaleY		= EaseQuadPercent(	fBegin,
												kfFrom->ptScale.y,
												(kfTo->ptScale.y - kfFrom->ptScale.y),
												fDuration,
												kfFrom->fEasing);

			fCurrR			= EaseQuadPercent(	fBegin,
												(f32)kfFrom->iColorR,
												((f32)kfTo->iColorR - (f32)kfFrom->iColorR),
												fDuration,
												kfFrom->fEasing);

			fCurrG			= EaseQuadPercent(	fBegin,
												(f32)kfFrom->iColorG,
												((f32)kfTo->iColorG - (f32)kfFrom->iColorG),
												fDuration,
												kfFrom->fEasing);

			fCurrB			= EaseQuadPercent(	fBegin,
												(f32)kfFrom->iColorB,
												((f32)kfTo->iColorB - (f32)kfFrom->iColorB),
												fDuration,
												kfFrom->fEasing);

			fCurrA			= EaseQuadPercent(	fBegin,
												(f32)kfFrom->iColorA,
												((f32)kfTo->iColorA - (f32)kfFrom->iColorA),
												fDuration,
												kfFrom->fEasing);
		}
		else
		{
			fCurrRot 		= kfFrom->fRotation;
			fCurrPosX		= kfFrom->ptPos.x;
			fCurrPosY		= kfFrom->ptPos.y;
			fCurrPivotX		= kfFrom->ptPivot.x;
			fCurrPivotY		= kfFrom->ptPivot.y;
			fCurrScaleX		= kfFrom->ptScale.x;
			fCurrScaleY		= kfFrom->ptScale.y;
			fCurrR			= kfFrom->iColorR;
			fCurrG			= kfFrom->iColorG;
			fCurrB			= kfFrom->iColorB;
			fCurrA			= kfFrom->iColorA;
		}

		pObject->SetPosition(fCurrPosX, fCurrPosY);
		pObject->SetPivot(fCurrPivotX, fCurrPivotY);
		pObject->SetRotation(fCurrRot);
		pObject->SetScale(fCurrScaleX, fCurrScaleY);
		pObject->SetColor((u32)fCurrR, (u32)fCurrG, (u32)fCurrB, (u32)fCurrA);
	}

	if ((fBegin / fDuration) >= 1.0f)
	{
		switch (kfTo->iEvent)
		{
			case Keyframe::KeyframeEventNone:
			{
				iKeyframeFrom 	= iKeyframeTo;
				iKeyframeTo 	= FindNextKeyframe();
			}
			break;

			case Keyframe::KeyframeEventStop:
			{
				iKeyframeFrom = iKeyframeTo;
			}
			break;

			case Keyframe::KeyframeEventRestart:
			{
				iKeyframeFrom = 0;
				iKeyframeTo   = -1;
				iCurrentFrame = 0;
				return;
			}
			break;

			case Keyframe::KeyframeEventJumpToFrame:
			{
				iCurrentFrame = kfTo->iFrameToJump;
				iKeyframeFrom = FindPreviousKeyframe();
				iKeyframeTo   = FindNextKeyframe();
				return;
			}
			break;
		}
	}
	else
	{
		switch (kfFrom->iEvent)
		{
			case Keyframe::KeyframeEventStop:
			{
				iKeyframeTo		= iKeyframeFrom;
				iCurrentFrame	= iKeyframeFrom;
				return;
			}
			break;
		}
	}

	// FIXME: when changing to timebased instead of framebased the iCurrentFrame increment must be based on the elapsedtime
	fElapsedKeyframeTime++;
	fElapsedTime++;
	iCurrentFrame++;
}

ISceneObject *Timeline::GetObject() const
{
	return pObject;
}

f32 Timeline::GetWidth() const
{
	f32 r = 0.0f;
	if (pObject)
		r = pObject->GetWidth();

	return r;
}

f32 Timeline::GetHeight() const
{
	f32 r = 0.0f;
	if (pObject)
		r = pObject->GetHeight();

	return r;
}

u32 Timeline::GetCurrentFrame() const
{
	return static_cast<u32>(iCurrentFrame);
}

void Timeline::SetLocalPosition(f32 posX, f32 posY)
{
	ptParentLocalPosition.x = posX;
	ptParentLocalPosition.y = posY;
}

void Timeline::SetPosition(f32 posX, f32 posY)
{
	ptParentPosition.x = posX;
	ptParentPosition.y = posY;
}

void Timeline::SetScale(f32 scaleX, f32 scaleY)
{
	ptParentScale.x = scaleX;
	ptParentScale.y = scaleY;
}

void Timeline::SetRotation(f32 rotation)
{
	fParentRotation = rotation;
}

s32 Timeline::FindKeyframeByName(const String &name)
{
	for (auto each: mapKeyframes)
	{
		Keyframe *obj = each.second;
		if (obj->sName == name)
			return obj->iFrame;
	}

	return 0;
}

s32 Timeline::FindNextKeyframe()
{
	for (auto each: mapKeyframes)
	{
		s32 pos = each.first;
		if (pos > iKeyframeFrom)
			return pos;
	}
	return -1;
}

s32 Timeline::FindPreviousKeyframe()
{
	KeyframeMapIterator it = mapKeyframes.end();
	KeyframeMapIterator beg = mapKeyframes.begin();
	for (; it != beg; --it)
	{
		s32 pos = (*it).first;
		if (pos < iCurrentFrame)
			return pos;
	}

	return -1;
}

void Timeline::SetListener(IEventMovieListener *listener)
{
	pListener = listener;
}

Keyframe *Timeline::GetCurrentKeyframe()
{
	return mapKeyframes[iKeyframeFrom];
}

f32 Timeline::EaseQuadPercent(f32 time, f32 begin, f32 change, f32 duration, f32 easing)
{
	if (duration <= 0.0f)
		return begin;

	if (time <= 0.0)
		return begin;

	if ((time /= duration) >= 1.0f)
		return begin + change;

	// linear tween if percent is 0
	if (!easing)
		return change * time + begin;

	if (easing > 1.0f)
		easing = 1.0f;
	else if (easing < -1.0f)
		easing = -1.0f;

	// ease in if percent is negative
	if (easing < 0.0f)
		return change * time * (time * (-easing) + (1.0f + easing)) + begin;

	// ease out if percent is positive
	return change * time * ((2.0f - time) * easing + (1.0f - easing)) + begin;
}

void Timeline::SetPriority(u32 p)
{
	iPriority = p;
	if (pObject)
		pObject->SetZ(f32(p));
}

u32 Timeline::GetPriority() const
{
	return iPriority;
}

void Timeline::SetObject(ISceneObject *object)
{
	if (pParent)
		pParent->Remove(pObject);

	pObject = object;

	if (pParent && object)
		pParent->Add(pObject);
}

void Timeline::SetParent(Movie *parent)
{
	if (pParent)
		pParent->Remove(pObject);

	pParent = parent;

	if (parent && pObject)
		parent->Add(pObject);
}

Movie *Timeline::GetParent() const
{
	return pParent;
}

bool Timeline::Unload()
{
	while (!mapKeyframes.empty())
	{
		Keyframe *obj = (*mapKeyframes.begin()).second;
		mapKeyframes.erase(mapKeyframes.begin());
		sdDelete(obj);
	}

	if (pParent && pObject)
		pParent->Remove(pObject);
	sdDelete(pObject);

	KeyframeMap().swap(mapKeyframes);

	this->Reset();

	return true;
}

Timeline *Timeline::Clone() const
{
	auto obj = sdNew(Timeline);
	obj->GenerateCloneName(sName);

	obj->pParent = pParent; // TODO: TEST
	obj->pObject = pObject; // TODO: TEST
	obj->pListener = pListener;

	obj->fElapsedTime = fElapsedTime;
	obj->fElapsedKeyframeTime = fElapsedKeyframeTime;
	obj->iCurrentFrame = iCurrentFrame;
	obj->iKeyframeFrom = iKeyframeFrom;
	obj->iKeyframeTo = iKeyframeTo;
	obj->iPriority = iPriority;

	obj->ptParentPosition = ptParentPosition;
	obj->ptParentLocalPosition = ptParentLocalPosition;
	obj->ptParentScale = ptParentScale;
	obj->fParentRotation = fParentRotation;

	for (auto kv: mapKeyframes)
		obj->mapKeyframes[kv.first] = kv.second->Clone();

	return obj;
}

void Timeline::Set(Reader &reader)
{
	sName = reader.ReadString("sName", sName.c_str());
	iPriority = reader.ReadU32("iPriority", iPriority);

	if (reader.SelectNode("cObject"))
	{
		if (pParent && pObject)
			pParent->Remove(pObject);
		sdDelete(pObject);

		pObject = pSceneObjectFactory->Load(reader, pRes);
		reader.UnselectNode();
	}
	else
	{
		auto object = String(reader.ReadString("sObjectPrefab", ""));
		if (object != "")
		{
			if (pParent && pObject)
				pParent->Remove(pObject);
			sdDelete(pObject);

			pObject = static_cast<ISceneObject *>(pPrefabManager->Get(object));
		}
	}

	if (pObject)
		pObject->SetZ(f32(iPriority));

	// incremental keyframes construction, this will merge or overwrite
	u32 keyframes = reader.SelectArray("aKeyframes");
	if (keyframes)
	{
		for (u32 i = 0; i < keyframes; i++)
		{
			reader.SelectNext();

			auto obj = sdNew(Keyframe);
			obj->Load(reader, pRes);

			this->AddKeyframe(obj); // overwrite if already exists
		}
		reader.UnselectArray();
	}
}

bool Timeline::Load(Reader &reader, ResourceManager *res)
{
	if (!this->Unload())
		return true;

	SEED_ASSERT(res);
	pRes = res;

	sName = reader.ReadString("sName", "timeline");
	iPriority = reader.ReadU32("iPriority", 0);

	if (reader.SelectNode("cObject"))
	{
		pObject = pSceneObjectFactory->Load(reader, pRes);
		reader.UnselectNode();
	}
	else
	{
		auto object = String(reader.ReadString("sObjectPrefab", ""));
		SEED_ASSERT_FMT(object != "", "Timeline '%s' does not have an object prefab [sObjectPrefab] set.", sName.c_str());
		pObject = static_cast<ISceneObject *>(pPrefabManager->Get(object));
	}

	SEED_ASSERT_FMT(pObject != nullptr, "Timeline '%s' does not have an scene object.", sName.c_str());
	pObject->SetZ(f32(iPriority));

	u32 keyframes = reader.SelectArray("aKeyframes");
	if (keyframes)
	{
		for (u32 i = 0; i < keyframes; i++)
		{
			reader.SelectNext();

			auto obj = sdNew(Keyframe);
			obj->Load(reader, res);

			u32 frame = 0;
			if (obj->iFrame)
				frame = obj->iFrame;
			else
				frame = i;

			SEED_ASSERT_FMT(mapKeyframes[frame] == nullptr, "Dupicated frame in the timeline '%s'.", sName.c_str());
			mapKeyframes[frame] = obj;
		}
		reader.UnselectArray();
	}

	SEED_WARNING(keyframes == 0, "Timeline '%s': Has no keyframes [aKeyframes]", sName.c_str());

	return true;
}

bool Timeline::Write(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("sType", this->GetTypeName());
		writer.WriteString("sName", sName.c_str());

		writer.WriteS32("iPriority", iPriority);

		if (pObject)
		{
			writer.OpenNode("cObject");
				pObject->Write(writer);
			writer.CloseNode();
		}

		writer.OpenArray("aKeyframes");
			for (auto each: mapKeyframes)
			{
				Keyframe *key = each.second;
				key->Write(writer);
			}
		writer.CloseArray();
	writer.CloseNode();

	return true;
}

} // namespace
