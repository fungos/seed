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

#if defined(BUILD_IOS)

#include "Defines.h"
#include "Log.h"

#include "FileSystem.h"
#include "SoundSystem.h"
#include "System.h"
#include "Music.h"
#include "SoundSource.h"
#include "Timer.h"

#include <stdio.h>
#include <algorithm>

#include "platform/ios/iosView.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define TAG "[SoundSystem] "

namespace Seed { namespace iOS {

SEED_SINGLETON_DEFINE(SoundSystem);

SoundSystem::SoundSystem()
	: pDevice(nullptr)
	, pContext(nullptr)
{
	this->Reset();
}

SoundSystem::~SoundSystem()
{
}

bool SoundSystem::Initialize()
{
	Log(TAG "Initializing...");
	bool r = this->Reset();

	pDevice = alcOpenDevice(nullptr);
	if (!pDevice)
	{
		Info(TAG "WARNING: Could not open OpenAL device - running wihtout sound!");
		//SEED_ASSERT(pDevice);
		r = false;
	}
	else
	{
		pContext = alcCreateContext(pDevice, nullptr);
		if (!pContext)
		{
			Info(TAG "WARNING: Could not create OpenAL context - running without sound!");
			//SEED_ASSERT(pContext);
			r = false;
		}
		else
		{
			IManager::Initialize();
			alcMakeContextCurrent(pContext);

			ALCint v1 = 0;
			ALCint v2 = 0;

			alcGetIntegerv(pDevice, ALC_MAJOR_VERSION, 1, &v1);
			alcGetIntegerv(pDevice, ALC_MINOR_VERSION, 1, &v2);

			Info(TAG "OpenAL Version %d.%d", v1, v2);
			Info(TAG "Device: %s.", alcGetString(pDevice, ALC_DEFAULT_DEVICE_SPECIFIER));
			Info(TAG "Extensions: %s.", alcGetString(pDevice, ALC_EXTENSIONS));
		}
	}
	return r;
}

bool SoundSystem::Reset()
{
	if (bInitialized)
	{
		fMusicStartFadeTime = 0.0f;
		fMusicFadeTime = 0.0f;

		for (auto each: vSource)
			each->Stop();

		for (auto each: vSource)
			each->Unload();

		ISoundSourceVector().swap(vSource);

		this->StopMusic();
		pCurrentMusic = nullptr;
		pNewMusic = nullptr;
	}
	return true;
}

bool SoundSystem::Shutdown()
{
	bool r = true;
	if (bInitialized)
	{
		Log(TAG "Terminating...");
		r = this->Reset();

		alcDestroyContext(pContext);
		alcCloseDevice(pDevice);

		IManager::Shutdown();
		Log(TAG "Terminated.");
	}
	return r;
}

bool SoundSystem::Update(Seconds dt)
{
	if (bInitialized && !bPaused)
	{
		this->UpdateSounds(dt);

		if (pNewMusic)
			this->UpdateMusic(dt, pNewMusic);

		if (pCurrentMusic)
			this->UpdateMusic(dt, pCurrentMusic);

		bChanged = false;
	}

	return true;
}

void SoundSystem::UpdateSounds(Seconds dt)
{
	UNUSED(dt);

	if (bChanged)
	{
		for (auto each: vSource)
			each->UpdateVolume();
	}

	for (auto each: vSource)
	{
		auto src = static_cast<SoundSource *>(each);

		eSoundSourceState state = src->GetState();
		if (state == SourceNone)
			continue;

		switch (state)
		{
			case Seed::SourcePlay:
			{
				src->eState = Seed::SourcePlayStarted;
			}
			break;

			case Seed::SourcePlayStarted:
			{
				src->eState = Seed::SourcePlaying;
			}
			break;

			case Seed::SourcePlaying:
			{
				ALint state = 0;
				alGetSourcei(src->iSource, AL_SOURCE_STATE, &state); // Quando volta do fadein essa porra ta ficando STOPPED!
				if (state == AL_STOPPED)
				{
					if (src->bLoop)
					{
						Log(TAG "Sound buffer underrun!");
						alSourcePlay(src->iSource);
					}
					else
					{
						Log(TAG "Source stopped...");
						src->eState = Seed::SourceStopped;
					}
				}
			}
			break;

			case Seed::SourceStop:
			{
				alSourceStop(src->iSource);
				src->eState = Seed::SourceStopped;
			}
			break;

			case Seed::SourceStopped:
			{
				src->eState = Seed::SourceNone;
			}
			break;

			case Seed::SourcePause:
			{
				src->eState = Seed::SourcePaused;
			}
			break;

			case Seed::SourcePaused:
			{
			}
			break;

			case Seed::SourceFadeIn:
			{
				src->eState = Seed::SourceFadingIn;
				alSourcePlay(src->iSource);
				alSourcef(src->iSource, AL_GAIN, 0.1f);
			}
			break;

			case Seed::SourceFadingIn:
			{
				Seconds elapsed = pTimer->GetSeconds() - src->fStartFadeTime;
				f32 volume = ((elapsed * src->fVolume) / src->fFadeTime);
				//Log(TAG "Elapsed: %f Volume: %f", elapsed, volume);

				if (elapsed >= src->fFadeTime)
				{
					src->eState = Seed::SourcePlaying;
					alSourcef(src->iSource, AL_GAIN, src->fVolume * pSoundSystem->GetSfxVolume());
				}
				else
					alSourcef(src->iSource, AL_GAIN, volume * pSoundSystem->GetSfxVolume());
			}
			break;

			case Seed::SourceFadeOut:
			{
				src->eState = Seed::SourceFadingOut;
			}
			break;

			case Seed::SourceFadingOut:
			{
				Seconds elapsed = src->fFadeTime - (pTimer->GetSeconds() - src->fStartFadeTime);
				f32 volume = ((elapsed * src->fVolume) / src->fFadeTime);
				//Log(TAG "Elapsed: %f Volume: %f", elapsed, volume);

				if (elapsed <= 0.0f)
				{
					alSourcef(src->iSource, AL_GAIN, 0.0f);
					src->eState = Seed::SourceStopped;
					alSourceStop(src->iSource);
					alSourcef(src->iSource, AL_GAIN, src->fVolume * pSoundSystem->GetSfxVolume());
				}
				else
					alSourcef(src->iSource, AL_GAIN, volume * pSoundSystem->GetSfxVolume());
			}
			break;

			default:
			break;
		}
	}
}

void SoundSystem::UpdateMusic(Seconds dt, IMusic *m)
{
	Music *mus = static_cast<Music *>(m);
	if (bChanged)
		mus->UpdateVolume();

	mus->Update(dt);

	switch (mus->eState)
	{
		case Seed::MusicPlay:
		{
			mus->eState = Seed::MusicPlayStarted;
		}
		break;

		case Seed::MusicPlayStarted:
		{
			mus->eState = Seed::MusicPlaying;
		}
		break;

		case Seed::MusicPlaying:
		{
		}
		break;

		case Seed::MusicStop:
		{
			mus->eState = Seed::MusicStopped;
		}
		break;

		case Seed::MusicStopped:
		{
			mus->eState = Seed::MusicNone;
			if (mus == pCurrentMusic)
			{
				pCurrentMusic = pNewMusic;
				pNewMusic = nullptr;
			}

			if (mus->bAutoUnload)
			{
				mus->Unload();
			}
			else
			{
				mus->Reset();
			}
		}
		break;

		case Seed::MusicPause:
		{
			mus->eState = Seed::MusicPaused;
		}
		break;

		case Seed::MusicPaused:
		{
		}
		break;

		case Seed::MusicFadeIn:
		{
			mus->eState = Seed::MusicFadingIn;
		}
		break;

		case Seed::MusicFadingIn:
		{
			Seconds elapsed = pTimer->GetSeconds() - fMusicStartFadeTime;
			f32 volume = ((elapsed * mus->fVolume) / fMusicFadeTime);
			Log(TAG "Elapsed: %f Volume: %f", elapsed, volume);

			if (elapsed >= fMusicFadeTime)
			{
				mus->eState = Seed::MusicPlaying;
				mus->FadeVolume(mus->fVolume);
			}
			else
			{
				mus->FadeVolume(volume);
			}
			mus->Update(dt);
		}
		break;

		case Seed::MusicFadeOut:
		{
			mus->eState = Seed::MusicFadingOut;
		}
		break;

		/* FIXME: 2009-15-06 | BUG | SDL | Fadeout / Fadein nao estao funcionando (alSourcef AL_GAIN) */
		case Seed::MusicFadingOut:
		{
			Seconds elapsed = fMusicFadeTime - (pTimer->GetSeconds() - fMusicStartFadeTime);
			f32 volume = ((elapsed * mus->fVolume) / fMusicFadeTime);
			Log(TAG "Elapsed: %f Volume: %f", elapsed, volume);

			if (elapsed <= 0.0f)
			{
				mus->FadeVolume(0.0f);
				mus->eState = Seed::MusicStopped;
			}
			else
			{
				mus->FadeVolume(volume);
			}
		}
		break;

		case Seed::MusicNone:
		default:
		break;
	}
}

void SoundSystem::Pause()
{
	ISoundSystem::Pause();

	for (auto each: vSource)
	{
		auto src = static_cast<SoundSource *>(each);
		src->Pause();
		alSourcePause(src->iSource);
	}

	if (pCurrentMusic)
		static_cast<Music *>(pCurrentMusic)->eState = Seed::MusicPaused;

	if (pNewMusic)
		static_cast<Music *>(pNewMusic)->eState = Seed::MusicPaused;
}

void SoundSystem::Resume()
{
	for (auto each: vSource)
		each->Resume();

	if (pCurrentMusic)
		static_cast<Music *>(pCurrentMusic)->eState = Seed::MusicPlay;

	if (pNewMusic)
		static_cast<Music *>(pNewMusic)->eState = Seed::MusicPlay;

	ISoundSystem::Resume();
}

}} // namespace

#endif // BUILD_IOS
