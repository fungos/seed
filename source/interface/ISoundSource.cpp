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

/*! \file ISoundSource.cpp
	\author	Danny Angelo Carminati Grein
	\brief A sund source interface to play 2D and 3D sounds
*/

#include "interface/ISoundSource.h"
#include "Sound.h"
#include "SoundSystem.h"
#include "Timer.h"

#define TAG		"[ISoundSource] "

namespace Seed {

ISoundSource::ISoundSource()
	: cPosition()
	, cVelocity()
	, cOrientation()
	, fVolume(1.0f)
	, fFadeTime(0.0f)
	, fStartFadeTime(0)
	, eState(SourceNone)
	, bLoop(FALSE)
{
}

ISoundSource::~ISoundSource()
{
}

void ISoundSource::Load(const String &filename)
{
	this->Load(filename, pResourceManager);
}

void ISoundSource::Load(const String &filename, ResourceManager *res)
{
	UNUSED(filename);
	UNUSED(res);
	SEED_ABSTRACT_METHOD;
}

void ISoundSource::Unload()
{
	eState = SourceNone;
}

void ISoundSource::SetPosition(f32 x, f32 y, f32 z)
{
	Vector3f v(x, y, z);
	this->cPosition = v;
}

void ISoundSource::SetPosition(Vector3f vec)
{
	this->cPosition = vec;
}

void ISoundSource::GetPosition(Vector3f *vec) const
{
	*vec = this->cPosition;
}

void ISoundSource::SetVelocity(f32 x, f32 y, f32 z)
{
	Vector3f v(x, y, z);
	this->cVelocity = v;
}

void ISoundSource::SetVelocity(Vector3f vec)
{
	this->cVelocity = vec;
}

void ISoundSource::GetVelocity(Vector3f *vec) const
{
	*vec = this->cVelocity;
}

void ISoundSource::SetOrientation(f32 x, f32 y, f32 z)
{
	Vector3f v(x, y, z);
	this->cOrientation = v;
}

void ISoundSource::SetOrientation(Vector3f vec)
{
	this->cOrientation = vec;
}

void ISoundSource::GetOrientation(Vector3f *vec) const
{
	*vec = this->cOrientation;
}

void ISoundSource::SetVolume(f32 vol)
{
	ASSERT_MSG((vol >= 0 || vol <= 1.0f), "Source volume must be between 0 and 1");
	this->fVolume = vol;
}

f32 ISoundSource::GetVolume() const
{
	return this->fVolume;
}

void ISoundSource::UpdateVolume()
{
	SEED_ABSTRACT_METHOD;
}

bool ISoundSource::IsPlaying() const
{
	return ((eState != SourceStopped) &&
			(eState != SourceStop) &&
			(eState != SourcePause) &&
			(eState != SourcePaused) &&
			(eState != SourceNone));
}

void ISoundSource::Play()
{
	if (eState != SourcePlaying && eState != SourcePlayStarted)
		eState = SourcePlay;
}

void ISoundSource::Stop(f32 ms)
{
	UNUSED(ms);
	eState = SourceStop;
}

void ISoundSource::Pause()
{
	if (this->IsPlaying())
	{
		eState = SourcePause;
	}
}

void ISoundSource::Resume()
{
	if (eState == SourcePause || eState == SourcePaused)
	{
		eState = SourcePlay;
	}
}

void ISoundSource::FadeOut(f32 ms)
{
	if (eState != SourceFadingOut)
		eState = SourceFadeOut;

	this->fStartFadeTime = pTimer->GetMilliseconds();
	this->fFadeTime = ms;
}

void ISoundSource::FadeIn(f32 ms)
{
	if (eState != SourceFadingIn)
		eState = SourceFadeIn;

	this->fStartFadeTime = pTimer->GetMilliseconds();
	this->fFadeTime = ms;
}

void ISoundSource::SetLoop(bool b)
{
	this->bLoop = b;
}

bool ISoundSource::IsLoop() const
{
	return this->bLoop;
}

eSoundSourceState ISoundSource::GetState() const
{
	return this->eState;
}

const char *ISoundSource::GetObjectName() const
{
	return "ISoundSource";
}

int ISoundSource::GetObjectType() const
{
	return Seed::ObjectSoundSource;
}

} // namespace
