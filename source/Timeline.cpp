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
#include "EventMovie.h"
#include "interface/IEventMovieListener.h"
#include "Movie.h"
#include "Keyframe.h"
#include "Sprite.h"

#define TAG "[Timeline] "

namespace Seed {

Timeline::Timeline()
	: pParent(NULL)
	, pObject(NULL)
	, pListener(NULL)
	, sName()
	, fElapsedTime(0.0f)
	, fElapsedKeyframeTime(0.0f)
	, iCurrentFrame(0)
	, iKeyframeFrom(0)
	, iKeyframeTo(0)
{
}

Timeline::~Timeline()
{
	this->Unload();
}

bool Timeline::Unload()
{
	KeyframeMapIterator it = mapKeyframes.begin();
	KeyframeMapIterator end = mapKeyframes.end();
	for (; it != end; ++it)
	{
		Keyframe *obj = (*it).second;
		Delete(obj);
	}

	Delete(pObject);
	KeyframeMap().swap(mapKeyframes);

	sName = "";
	this->Reset();

	return true;
}

bool Timeline::Load(Reader &reader, ResourceManager *res)
{
	ASSERT_NULL(res);

	bool ret = false;

	if (this->Unload())
	{
		sName = reader.ReadString("name", "timeline");
		f32 prio = reader.ReadF32("priority", 0.0f);

		/*
		FIXME: The object here can be any ISceneObject, so we need a way to
		identify the kind of object and a kind of factory that will construct
		the object and return to us (Project)
		*/
		if (reader.SelectNode("object"))
		{
			Reader r(reader);
			Sprite *spt = New(Sprite);
			spt->Load(r, res);
			spt->SetPriority(prio);
			pObject = spt;
			reader.UnselectNode();
		}
		else
		{
			String object = reader.ReadString("object", "");
			ASSERT_MSG(object.length() > 0, "Keyframe does not have an 'object' set ");

			File f(object);
			Reader r(f);
			Sprite *spt = New(Sprite);
			spt->Load(r, res);
			spt->SetPriority(prio);
			pObject = spt;
		}

		u32 keyframes = reader.SelectArray("keyframes");
		ASSERT_MSG(keyframes != 0, "Timeline does not have keyframes.");
		if (keyframes)
		{
			for (u32 i = 0; i < keyframes; i++)
			{
				reader.SelectNext();

				Keyframe *obj = New(Keyframe);
				obj->Load(reader, res);

				u32 frame = 0;
				if (obj->iFrame)
					frame = obj->iFrame;
				else
					frame = i;

				ASSERT_MSG(mapKeyframes[frame] == NULL, "Dupicated frame in the timeline");
				mapKeyframes[frame] = obj;
			}
			reader.UnselectArray();

			ret = true;
		}
		else
		{
			Log(TAG " WARNING: No keyframe found in the timeline '%s'", sName.c_str());
		}
	}

	return ret;
}

void Timeline::Reset()
{
	fElapsedTime 			= 0.0f;
	fElapsedKeyframeTime 	= 0.0f;
	iCurrentFrame 			= 0;
	iKeyframeFrom 			= 0;
	iKeyframeTo 			= 0;
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
		Delete(mapKeyframes[keyframe->iFrame]);

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
	if (!pObject)
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

	ASSERT_MSG(kfFrom, 	"A keyframe is required at frame 0.");
	ASSERT_MSG(kfTo, 	"At least two keyframes must be set.");

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

	//FIXME when changing to timebased instead of framebased the iCurrentFrame increment must
	//be based on the elapsedtime
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
	ForEach(KeyframeMap, mapKeyframes,
	{
		Keyframe *obj = (*it).second;

		if (obj->sName == name)
			return obj->iFrame;
	});

	return 0;
}

s32 Timeline::FindNextKeyframe()
{
	KeyframeMap::iterator it = mapKeyframes.begin();
	KeyframeMap::iterator end = mapKeyframes.end();
	for (; it != end; ++it)
	{

//	ForEach(KeyframeMap, mapKeyframes,
	//{
		s32 pos = (*it).first;
		if (pos > iKeyframeFrom)
			return pos;
	//});
	}
	return -1;
}

s32 Timeline::FindPreviousKeyframe()
{
	ReverseForEach(KeyframeMap, mapKeyframes,
	{
		s32 pos = (*it).first;
		if (pos < iCurrentFrame)
			return pos;
	});

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

} // namespace
