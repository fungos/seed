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

#include "interface/ISoundSystem.h"
#include "interface/IMusic.h"
#include "Timer.h"

namespace Seed {

ISoundSystem::ISoundSystem()
	: fMusicVolume(1.0f)
	, fSfxVolume(1.0f)
	, fMusicVolumeOrig(1.0f)
	, fSfxVolumeOrig(1.0f)
	, bMuted(false)
	, bChanged(false)
	, bPaused(false)
	, arSource()
	, pCurrentMusic(NULL)
	, pNewMusic(NULL)
	, fMusicStartFadeTime(0.0f)
	, fMusicFadeTime(0.0f)
{
}

ISoundSystem::~ISoundSystem()
{
}

void ISoundSystem::Add(ISoundSource *src)
{
	arSource.Add(static_cast<ISoundSource *>(src));
}

void ISoundSystem::Remove(ISoundSource *src)
{
	arSource.Remove(static_cast<ISoundSource *>(src));
}

void ISoundSystem::SetMusicVolume(f32 volume)
{
	ASSERT_MSG((volume >= 0 || volume <= 1.0f), "Music volume must be between 0 and 1");
	fMusicVolume = volume;
	bChanged = true;
}

f32 ISoundSystem::GetMusicVolume() const
{
	return fMusicVolume;
}

void ISoundSystem::SetSfxVolume(f32 volume)
{
	ASSERT_MSG((volume >= 0 || volume <= 1.0f), "Sfx volume must be between 0 and 1");
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

void ISoundSystem::PlayMusic(IMusic *mus, f32 ms)
{
	fMusicFadeTime = ms;
	fMusicStartFadeTime = static_cast<f32>(pTimer->GetMilliseconds());

	if (pCurrentMusic && mus != pCurrentMusic)
	{
		if (pNewMusic)
			pNewMusic->Stop();

		pNewMusic = mus;

		if (ms)
		{
			pCurrentMusic->eState = Seed::MusicFadeOut;
			pNewMusic->eState = Seed::MusicFadeIn;
		}
		else
		{
			pCurrentMusic->eState = Seed::MusicStop;
			pNewMusic->eState = Seed::MusicPlay;
		}
	}
	else
	{
		pCurrentMusic = mus;

		if (ms)
		{
			pCurrentMusic->eState = Seed::MusicFadeIn;
		}
		else
		{
			pCurrentMusic->eState = Seed::MusicPlay;
		}
	}
}

void ISoundSystem::StopMusic(f32 ms, IMusic *mus)
{
	if (mus)
	{
		mus->eState = Seed::MusicStop;

		if (pCurrentMusic == mus)
			pCurrentMusic = NULL;

		if (pNewMusic == mus)
			pNewMusic = NULL;
	}
	else if (pCurrentMusic)
	{
		fMusicFadeTime = ms;
		if (ms)
		{
			fMusicStartFadeTime = static_cast<f32>(pTimer->GetMilliseconds());
			pCurrentMusic->eState = Seed::MusicFadeOut;
		}
		else
		{
			pCurrentMusic->eState = Seed::MusicStop;
		}
	}
}

void ISoundSystem::StopSounds()
{
	for (u32 i = 0; i < arSource.Size(); i++)
	{
		arSource[i]->Stop();
	}
}

void ISoundSystem::Pause()
{
	bPaused = true;
}

void ISoundSystem::Resume()
{
	bPaused = false;
}

const char *ISoundSystem::GetObjectName() const
{
	return "SoundSystem";
}

} // namespace
