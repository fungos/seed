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

#include "interface/ISoundSystem.h"
#include "interface/ISoundSource.h"
#include "interface/IMusic.h"
#include "System.h"

namespace Seed {

ISoundSystem::ISoundSystem()
	: pCurrentMusic(nullptr)
	, pNewMusic(nullptr)
	, vSource()
	, fMusicVolume(1.0f)
	, fSfxVolume(1.0f)
	, fMusicVolumeOrig(1.0f)
	, fSfxVolumeOrig(1.0f)
	, fMusicFadeTime(0.0f)
	, fMusicStartFadeTime(0.0f)
	, bMuted(false)
	, bChanged(false)
	, bPaused(false)
{
}

ISoundSystem::~ISoundSystem()
{
}

void ISoundSystem::Add(ISoundSource *src)
{
	vSource += src;
}

void ISoundSystem::Remove(ISoundSource *src)
{
	vSource -= src;
}

void ISoundSystem::SetMusicVolume(f32 volume)
{
	SEED_ASSERT_MSG((volume >= 0 && volume <= 1.0f), "Music volume must be between 0 and 1");
	fMusicVolume = volume;
	bChanged = true;
}

f32 ISoundSystem::GetMusicVolume() const
{
	return fMusicVolume;
}

void ISoundSystem::SetSfxVolume(f32 volume)
{
	SEED_ASSERT_MSG((volume >= 0 && volume <= 1.0f), "Sfx volume must be between 0 and 1");
	fSfxVolume = volume;
	bChanged = true;
}

f32 ISoundSystem::GetSfxVolume() const
{
	return fSfxVolume;
}

void ISoundSystem::Mute()
{
	if (!bMuted)
	{
		fMusicVolumeOrig = fMusicVolume;
		fSfxVolumeOrig = fSfxVolume;
		bMuted = true;
		this->SetMusicVolume(0.0f);
		this->SetSfxVolume(0.0f);
	}
}

void ISoundSystem::Unmute()
{
	if (bMuted)
	{
		bMuted = false;
		this->SetMusicVolume(fMusicVolumeOrig);
		this->SetSfxVolume(fSfxVolumeOrig);
	}
}

void ISoundSystem::PlayMusic(IMusic *mus, Seconds ms)
{
	fMusicFadeTime = ms;
	fMusicStartFadeTime = pTimer->GetSeconds();

	if (pCurrentMusic && mus != pCurrentMusic)
	{
		if (pNewMusic)
			pNewMusic->Stop();

		pNewMusic = mus;

		if (ms)
		{
			pCurrentMusic->nState = eMusicState::FadeOut;
			pNewMusic->nState = eMusicState::FadeIn;
		}
		else
		{
			pCurrentMusic->nState = eMusicState::Stop;
			pNewMusic->nState = eMusicState::Play;
		}
	}
	else
	{
		pCurrentMusic = mus;

		if (ms)
		{
			pCurrentMusic->nState = eMusicState::FadeIn;
		}
		else
		{
			pCurrentMusic->nState = eMusicState::Play;
		}
	}
}

void ISoundSystem::StopMusic(f32 ms, IMusic *mus)
{
	if (mus)
	{
		mus->nState = eMusicState::Stop;

		if (pCurrentMusic == mus)
			pCurrentMusic = nullptr;

		if (pNewMusic == mus)
			pNewMusic = nullptr;

		return;
	}

	if (pCurrentMusic)
	{
		fMusicFadeTime = ms;
		if (ms)
		{
			fMusicStartFadeTime = pTimer->GetSeconds();
			pCurrentMusic->nState = eMusicState::FadeOut;
		}
		else
		{
			pCurrentMusic->nState = eMusicState::Stop;
			pCurrentMusic = nullptr;
		}

		return;
	}
	
	if (pNewMusic)
	{
		fMusicFadeTime = ms;
		if (ms)
		{
			fMusicStartFadeTime = pTimer->GetSeconds();
			pNewMusic->nState = eMusicState::FadeOut;
		}
		else
		{
			pNewMusic->nState = eMusicState::Stop;
			pNewMusic = nullptr;
		}
	}
}

void ISoundSystem::StopSounds()
{
	for (auto obj: vSource)
		obj->Stop();
}

void ISoundSystem::Pause()
{
	bPaused = true;
}

void ISoundSystem::Resume()
{
	bPaused = false;
}

} // namespace
