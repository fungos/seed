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

/*! \file Movie.cpp
	\author	Rafael Eduardo Gonchor
	\brief A collection of timelines that are part of the movie
*/

#include "Movie.h"

namespace Seed {

Movie::Movie()
	: bPlaying(true)
	, fElapsedTime(0.0f)
	, vTimelines()
{
}

Movie::~Movie()
{
	this->Reset();
}

void Movie::AddTimeline(Timeline *timeline)
{
	vTimelines.push_back(timeline);
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

		ForEach(Timeline, vTimelines,
		{
			Timeline *obj = (*it);
			if (bTransformationChanged)
			{
				obj->SetLocalPosition(this->GetLocalX(), this->GetLocalY());
				obj->SetPosition(this->GetX(), this->GetY());
				obj->SetScale(this->GetScaleX(), this->GetScaleY());
				obj->SetRotation(this->GetRotation());
			}

			obj->Update();
		});
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
	ForEach(Timeline, vTimelines,
	{
		(*it)->Rewind();
	});
}

void Movie::Reset()
{
	ForEach(Timeline, vTimelines,
	{
		(*it)->Reset();
	});

	TimelineVector().swap(vTimelines);
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
