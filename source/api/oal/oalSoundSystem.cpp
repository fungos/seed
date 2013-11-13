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

#include "SoundSystem.h"

#if defined(USE_API_OAL)

#include "Log.h"
#include "FileSystem.h"
#include "System.h"
#include "ResourceManager.h"
#include "Sound.h"
#include "SoundSource.h"
#include "Music.h"
#include "interface/IMusic.h"
#include "interface/ISoundSource.h"
#include "Timer.h"

#include <stdio.h>
#include <algorithm>

#if defined(__APPLE_CC__)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#define TAG "[SoundSystem] "

#define AUDIO_FRAME_TIME	3
#define AUDIO_DATA_PATH		"/" FILESYSTEM_DEFAULT_PATH

// http://www.gamedev.net/reference/articles/article2031.asp
// http://icculus.org/SDL_sound/
// http://www.devmaster.net/articles.php?catID=6

namespace Seed { namespace OAL {

SEED_SINGLETON_DEFINE(SoundSystem)

SoundSystem::SoundSystem()
	: pDevice(NULL)
	, pContext(NULL)
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

	pDevice = alcOpenDevice(NULL);
	if (!pDevice)
	{
		Info(TAG "WARNING: Could not open OpenAL device - running wihtout sound!");
		//SEED_ASSERT(pDevice);
		r = false;
	}
	else
	{
		pContext = alcCreateContext(pDevice, NULL);
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
		pCurrentMusic = NULL;
		pNewMusic = NULL;
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

bool SoundSystem::Update(f32 dt)
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

void SoundSystem::UpdateSounds(f32 dt)
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
		if (state == eSoundSourceState::None)
			continue;

		switch (state)
		{
			case eSoundSourceState::Play:
			{
				src->nState = eSoundSourceState::PlayStarted;
			}
			break;

			case eSoundSourceState::PlayStarted:
			{
				src->nState = eSoundSourceState::Playing;
			}
			break;

			case eSoundSourceState::Playing:
			{
				ALint alstate = 0;
				alGetSourcei(src->iSource, AL_SOURCE_STATE, &alstate); // Quando volta do fadein essa porra ta ficando STOPPED!
				if (alstate == AL_STOPPED)
				{
					if (src->bLoop)
					{
						Log(TAG "Sound buffer underrun!");
						alSourcePlay(src->iSource);
					}
					else
					{
						Log(TAG "Source stopped...");
						src->nState = eSoundSourceState::Stopped;
					}
				}
			}
			break;

			case eSoundSourceState::Stop:
			{
				alSourceStop(src->iSource);
				src->nState = eSoundSourceState::Stopped;
			}
			break;

			case eSoundSourceState::Stopped:
			{
				src->nState = eSoundSourceState::None;
			}
			break;

			case eSoundSourceState::Pause:
			{
				src->nState = eSoundSourceState::Paused;
			}
			break;

			case eSoundSourceState::Paused:
			{
			}
			break;

			case eSoundSourceState::FadeIn:
			{
				src->nState = eSoundSourceState::FadingIn;
				alSourcePlay(src->iSource);
				alSourcef(src->iSource, AL_GAIN, 0.1f);
			}
			break;

			case eSoundSourceState::FadingIn:
			{
				f32 elapsed = static_cast<f32>(pTimer->GetMilliseconds() - src->fStartFadeTime);
				f32 volume = ((elapsed * src->fVolume) / src->fFadeTime);
				//Log(TAG "Elapsed: %f Volume: %f", elapsed, volume);

				if (elapsed >= src->fFadeTime)
				{
					src->nState = eSoundSourceState::Playing;
					alSourcef(src->iSource, AL_GAIN, src->fVolume * pSoundSystem->GetSfxVolume());
				}
				else
					alSourcef(src->iSource, AL_GAIN, volume * pSoundSystem->GetSfxVolume());
			}
			break;

			case eSoundSourceState::FadeOut:
			{
				src->nState = eSoundSourceState::FadingOut;
			}
			break;

			case eSoundSourceState::FadingOut:
			{
				f32 elapsed = src->fFadeTime - static_cast<f32>(pTimer->GetMilliseconds() - src->fStartFadeTime);
				f32 volume = ((elapsed * src->fVolume) / src->fFadeTime);
				//Log(TAG "Elapsed: %f Volume: %f", elapsed, volume);

				if (elapsed <= 0.0f)
				{
					alSourcef(src->iSource, AL_GAIN, 0.0f);
					src->nState = eSoundSourceState::Stopped;
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

void SoundSystem::UpdateMusic(f32 dt, IMusic *m)
{
	Music *mus = static_cast<Music *>(m);
	if (bChanged)
		mus->UpdateVolume();

	switch (mus->nState)
	{
		case eMusicState::Play:
		{
			mus->nState = eMusicState::PlayStarted;
		}
		break;

		case eMusicState::PlayStarted:
		{
			alSourcePlay(mus->iSource);
			mus->nState = eMusicState::Playing;
		}
		break;

		case eMusicState::Playing:
		{
			ALint state = 0;
			alGetSourcei(mus->iSource, AL_SOURCE_STATE, &state);
			if (state == AL_STOPPED)
			{
				if (mus->bLoop) // underrun happened
				{
					Log(TAG "Music buffer underrun!");
					ALint processed = 0;
					alGetSourcei(mus->iSource, AL_BUFFERS_PROCESSED, &processed);
					alSourcePlay(mus->iSource);
				}
				else // finished or underrung...
				{
					Log(TAG "Music stopped...");
					mus->nState = eMusicState::Stopped;
				}
			}
			else
			{
				mus->Update(dt);
			}
		}
		break;

		case eMusicState::Stop:
		{
			alSourceStop(mus->iSource);
			mus->nState = eMusicState::Stopped;
		}
		break;

		case eMusicState::Stopped:
		{
			mus->nState = eMusicState::None;
			if (mus == pCurrentMusic)
			{
				pCurrentMusic = pNewMusic;
				pNewMusic = NULL;
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

		case eMusicState::Pause:
		{
			mus->nState = eMusicState::Paused;
		}
		break;

		case eMusicState::Paused:
		{
			alSourcePause(mus->iSource);
		}
		break;

		case eMusicState::FadeIn:
		{
			mus->nState = eMusicState::FadingIn;
			alSourcef(mus->iSource, AL_GAIN, 0.1f);
			alSourcePlay(mus->iSource);
		}
		break;

		case eMusicState::FadingIn:
		{
			f32 elapsed = static_cast<f32>(pTimer->GetMilliseconds()) - fMusicStartFadeTime;
			f32 volume = ((elapsed * mus->fVolume) / fMusicFadeTime);
			//Log(TAG "Elapsed: %f Volume: %f", elapsed, volume);

			if (elapsed >= fMusicFadeTime)
			{
				mus->nState = eMusicState::Playing;
				alSourcef(mus->iSource, AL_GAIN, mus->fVolume * pSoundSystem->GetMusicVolume());
			}
			else
			{
				alSourcef(mus->iSource, AL_GAIN, volume * pSoundSystem->GetMusicVolume());
			}
			mus->Update(dt);
		}
		break;

		case eMusicState::FadeOut:
		{
			mus->nState = eMusicState::FadingOut;
		}
		break;

		/* FIXME: Fadeout / Fadein nao estao funcionando (alSourcef AL_GAIN)? Testar... */
		case eMusicState::FadingOut:
		{
			f32 elapsed = fMusicFadeTime - static_cast<f32>(pTimer->GetMilliseconds()) - fMusicStartFadeTime;
			f32 volume = ((elapsed * mus->fVolume) / fMusicFadeTime);
			//Log(TAG "Elapsed: %f Volume: %f", elapsed, volume);

			if (elapsed <= 0.0f)
			{
				alSourcef(mus->iSource, AL_GAIN, 0.0f);
				mus->nState = eMusicState::Stopped;
			}
			else
			{
				alSourcef(mus->iSource, AL_GAIN, volume * pSoundSystem->GetMusicVolume());
			}
		}
		break;

		case eMusicState::None:
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
		static_cast<Music *>(pCurrentMusic)->nState = eMusicState::Paused;

	if (pNewMusic)
		static_cast<Music *>(pNewMusic)->nState = eMusicState::Paused;
}

void SoundSystem::Resume()
{
	for (auto each: vSource)
		each->Resume();

	if (pCurrentMusic)
		static_cast<Music *>(pCurrentMusic)->nState = eMusicState::Play;

	if (pNewMusic)
		static_cast<Music *>(pNewMusic)->nState = eMusicState::Play;

	ISoundSystem::Resume();
}

}} // namespace

#endif // USE_API_OAL
