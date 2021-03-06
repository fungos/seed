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

#include "SoundSource.h"

#if defined(USE_API_OAL)

#include "SoundSystem.h"
#include "Sound.h"
#include "Log.h"
#include "Memory.h"
#include "SeedInit.h"

#define TAG		"[SoundSource] "

namespace Seed { namespace OAL {

SoundSource::SoundSource()
	: iSource(0)
{
}

SoundSource::~SoundSource()
{
	this->Unload();
}

bool SoundSource::OnLoadFinished()
{
	ALenum err = 0;
	if (iSource)
		alDeleteSources(1, &iSource);

	alGenSources(1, &iSource);
	err = alGetError();
	if (err != AL_NO_ERROR)
		Info(TAG "Could not create OpenAL Source: %4x", err);

	auto buffer = static_cast<const ALint *>(pSound->GetData());

	alDistanceModel(AL_LINEAR_DISTANCE);
	alSource3f(iSource, AL_POSITION, GetX(), GetY(), GetZ());
	alSource3f(iSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSource3f(iSource, AL_DIRECTION, 0.0f, 0.0f, 0.0f); // direction == vector forward == transfromMatrix column 2;
	alSourcei(iSource, AL_LOOPING, bLoop);
	alSourcef(iSource, AL_ROLLOFF_FACTOR, 1.0f);
	alSourcef(iSource, AL_MAX_DISTANCE, 1.0f);
	alSourcef(iSource, AL_REFERENCE_DISTANCE, 1.0f);
	alSourcef(iSource, AL_PITCH, 1.0f);
	alSourcef(iSource, AL_GAIN, 1.0f);
	alSourcei(iSource, AL_BUFFER, *buffer);

	this->SetVolume(fVolume);

	return true;
}

bool SoundSource::OnUnloadRequest()
{
	if (Private::bInitialized)
		pSoundSystem->Remove(this);

	if (iSource)
		alDeleteSources(1, &iSource);

	return true;
}

void SoundSource::SetVolume(f32 vol)
{
	ISoundSource::SetVolume(vol);
	alSourcef(iSource, AL_GAIN, fVolume * pSoundSystem->GetSfxVolume());
}

void SoundSource::UpdateVolume()
{
	alSourcef(iSource, AL_GAIN, fVolume * pSoundSystem->GetSfxVolume());
}

SoundSource *SoundSource::Clone() const
{
	auto obj = sdNew(SoundSource);

	// TODO: TEST
	this->DoClone(obj);
	obj->OnLoadFinished();

	return obj;
}

void SoundSource::SetLoop(bool b)
{
	ISoundSource::SetLoop(b);
	alSourcei(iSource, AL_LOOPING, b);
}

void SoundSource::Stop(Seconds s)
{
	UNUSED(s)

	alSourceStop(iSource);
	nState = eSoundSourceState::Stop;
}

void SoundSource::Play()
{
	alSourceStop(iSource);
	alSourcePlay(iSource);
	nState = eSoundSourceState::Playing;
}

void SoundSource::Resume()
{
	if (nState == eSoundSourceState::Pause || nState == eSoundSourceState::Paused)
	{
		alSourcePlay(iSource);
		nState = eSoundSourceState::Play;
	}
}

void SoundSource::Update(Seconds dt)
{
	UNUSED(dt);

	if (bTransformationChanged)
	{
		alSource3f(iSource, AL_POSITION, GetX(), GetY(), GetZ());
		ITransformable::UpdateTransform();
	}
}

}} // namespace

#endif // USE_API_OAL
