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

/*! \file OalOggMusic.cpp
	\author	Danny Angelo Carminati Grein
	\brief Music Implementation for OpenAL + Ogg
*/

#include "Music.h"

#if defined(_OAL_OGG_)

#include "FileSystem.h"
#include "Log.h"
#include "SoundSystem.h"
#include "Reader.h"

#define TAG "[Music] "

namespace Seed { namespace OAL {

IResource *MusicResourceLoader(const char *filename, ResourceManager *res)
{
	UNUSED(res);

	Music *music = New(Music());
	music->Load(filename, res);

	return music;
}

Music::Music()
	: iBuffers()
	, iSource(0)
	, vorbisInfo(NULL)
	, vorbisComment(NULL)
	, oggStream()
	, eFormat(AL_FORMAT_MONO16)
	, bLoop(TRUE)
{
	memset(iBuffers, '\0', sizeof(iBuffers));
}

Music::~Music()
{
	this->Unload();
}

bool Music::Load(const char *filename, ResourceManager *res)
{
	ASSERT_NULL(filename);
	ASSERT_NULL(res);

	if (pSoundSystem->IsInitialized() && this->Unload())
	{
		pFilename = filename;
		pRes = res;

		ALenum err = AL_NO_ERROR;

		/* Open file .music */
		File f(pFilename);
		Reader r(filename);
		SECURITY_CHECK(f.GetData(), "Music data couldn't be opened");

		ReaderPath(volume);
		u32 vol = r.ReadU32(volume);
		fVolume = (vol / 100.0f);

		ReaderPath(file)
		const char *fname = r.ReadString(file);
		ASSERT_NULL(fname);

		/* prepare openal */
		err = alGetError();
		alGenSources(1, &iSource);
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			Info(TAG "Could not create OpenAL music source (0x%04x).", err);
			bLoaded = FALSE;
			return bLoaded;
		}

		alGenBuffers(OPENAL_MUSIC_BUFFERS, iBuffers);
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			alDeleteSources(1, &iSource);
			memset(iBuffers, '\0', sizeof(iBuffers));
			bLoaded = FALSE;

			Info(TAG "Could not generate OpenAL music buffers (0x%04x).", err);//alGetError());
		}

		// FIXME: FileSystem manipulation must be centralized, rewrite to use File/FileSystem
		if (ov_fopen(const_cast<char *>(fname), &oggStream) < 0)
		{
			alDeleteSources(1, &iSource);
			alDeleteBuffers(OPENAL_MUSIC_BUFFERS, iBuffers);
			memset(iBuffers, '\0', sizeof(iBuffers));
			bLoaded = FALSE;

			Info(TAG "Could not open '%s' ogg stream (file does not exist or is not a valid ogg file).", fname);
			return bLoaded;
		}

		vorbisInfo = ov_info(&oggStream, -1);
		vorbisComment = ov_comment(&oggStream, -1);

		if (vorbisInfo->channels > 1)
			eFormat = AL_FORMAT_STEREO16;

		alSource3f(iSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
		alSource3f(iSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		alSource3f(iSource, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
		alSourcef(iSource, AL_ROLLOFF_FACTOR, 0.0);
		alSourcei(iSource, AL_SOURCE_RELATIVE, AL_TRUE);

		this->Reset();

		bLoaded = TRUE;
	}

	return bLoaded;
}

bool Music::Unload()
{
	if (!bLoaded)
		return TRUE;

	eState = Seed::MusicStopped;
	eFormat = AL_FORMAT_MONO16;
	fVolume = 1.0f;

	pSoundSystem->StopMusic(0, this);
	//ALenum err = AL_NO_ERROR;

	if (iSource)
	{
		int queued = 0;
		alGetSourcei(iSource, AL_BUFFERS_QUEUED, &queued);
		//err = alGetError();

		while (queued--)
		{
			ALuint buffer;
			alSourceUnqueueBuffers(iSource, 1, &buffer);
			//err = alGetError();
		}

		alDeleteSources(1, &iSource);
		//err = alGetError();
		iSource = 0;
	}

	alDeleteBuffers(OPENAL_MUSIC_BUFFERS, iBuffers);
	//err = alGetError();
	memset(iBuffers, '\0', sizeof(iBuffers));

	ov_clear(&oggStream);
	bLoaded = FALSE;

	return TRUE;
}

void Music::Reset()
{
	int queued = 0;
	alGetSourcei(iSource, AL_BUFFERS_QUEUED, &queued);

	while (queued--)
	{
		ALuint buffer;
		alSourceUnqueueBuffers(iSource, 1, &buffer);
	}

	this->SetVolume(fVolume);

	ov_raw_seek(&oggStream, 0);
	for (u32 i = 0; i < OPENAL_MUSIC_BUFFERS; i++)
		this->DoStream(iBuffers[i]);

	alSourceQueueBuffers(iSource, OPENAL_MUSIC_BUFFERS, iBuffers);
	//ALenum err = alGetError();
}

bool Music::Update(f32 dt)
{
	UNUSED(dt);

	bool active = true;
	ALint processed = 0;
	alGetSourcei(iSource, AL_BUFFERS_PROCESSED, &processed);

	while (processed--)
	{
		ALuint buffer;

		alSourceUnqueueBuffers(iSource, 1, &buffer);
		active = this->DoStream(buffer);
		if (active || bLoop)
			alSourceQueueBuffers(iSource, 1, &buffer);
	}

	if (!active)
		eState = MusicStopped;

	return active;
}

bool Music::DoStream(ALuint buffer)
{
	return ogg_update_stream(&oggStream, vorbisInfo->rate, eFormat, buffer, bLoop);
}

void Music::SetVolume(f32 vol)
{
	IMusic::SetVolume(vol);
	alSourcef(iSource, AL_GAIN, fVolume * pSoundSystem->GetMusicVolume());
}

void Music::UpdateVolume()
{
	alSourcef(iSource, AL_GAIN, fVolume * pSoundSystem->GetMusicVolume());
}

const void *Music::GetData() const
{
	return iBuffers;
}

}} // namespace

#endif // _OAL_
