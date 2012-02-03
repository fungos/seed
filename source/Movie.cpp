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
	: bPlaying(TRUE)
	, fElapsedTime(0.0f)
	, arTimelines()
{
}

Movie::~Movie()
{
	this->Reset();
}

void Movie::AddTimeline(Timeline *timeline)
{
	arTimelines.Add(timeline);
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
		for (u32 i = 0; i < arTimelines.Size(); i++)
		{
			if (bTransformationChanged)
			{
				arTimelines[i]->SetLocalPosition(this->GetLocalX(), this->GetLocalY());
				arTimelines[i]->SetPosition(this->GetX(), this->GetY());
				arTimelines[i]->SetScale(this->GetScaleX(), this->GetScaleY());
				arTimelines[i]->SetRotation(this->GetRotation());
			}

			arTimelines[i]->Update();
		}
	}

	SceneNode<SEED_MOVIE_TIMELINES>::Update(delta);
}

void Movie::Play()
{
	fElapsedTime = 0.0f;
	bPlaying = TRUE;
}

void Movie::Stop()
{
	fElapsedTime = 0.0f;
	bPlaying = FALSE;
}

void Movie::Rewind()
{
	for (u32 i = 0; i < arTimelines.Size(); i++)
	{
		arTimelines[i]->Rewind();
	}
}

void Movie::Reset()
{
	for (u32 i = 0; i < arTimelines.Size(); i++)
		arTimelines[i]->Reset();

	arTimelines.Truncate();
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
