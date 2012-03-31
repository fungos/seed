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

#include "Music.h"

#if defined(USE_API_OAL)

#include "FileSystem.h"
#include "Log.h"
#include "SoundSystem.h"
#include "Reader.h"

#define TAG "[Music] "

namespace Seed { namespace OAL {

IResource *MusicResourceLoader(const String &filename, ResourceManager *res)
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
	, bLoop(true)
{
	memset(iBuffers, '\0', sizeof(iBuffers));
}

Music::~Music()
{
	this->Unload();
}

bool Music::Load(const String &filename, ResourceManager *res)
{
	SEED_ASSERT(res);

	if (pSoundSystem->IsInitialized() && this->Unload())
	{
		pRes = res;

		ALenum err = AL_NO_ERROR;

		/* prepare openal */
		err = alGetError();
		alGenSources(1, &iSource);
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			Info(TAG "Could not create OpenAL music source (0x%04x).", err);
			bLoaded = false;
			return bLoaded;
		}

		alGenBuffers(OPENAL_MUSIC_BUFFERS, iBuffers);
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			alDeleteSources(1, &iSource);
			memset(iBuffers, '\0', sizeof(iBuffers));
			bLoaded = false;

			Info(TAG "Could not generate OpenAL music buffers (0x%04x).", err);//alGetError());
		}

		// FIXME: FileSystem manipulation must be centralized, rewrite to use File/FileSystem
		if (ov_fopen(const_cast<char *>(filename.c_str()), &oggStream) < 0)
		{
			alDeleteSources(1, &iSource);
			alDeleteBuffers(OPENAL_MUSIC_BUFFERS, iBuffers);
			memset(iBuffers, '\0', sizeof(iBuffers));
			bLoaded = false;

			Info(TAG "Could not open '%s' ogg stream (file does not exist or is not a valid ogg file).", filename.c_str());
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

		bLoaded = true;
	}

	return bLoaded;
}

bool Music::Unload()
{
	if (!bLoaded)
		return true;

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
	bLoaded = false;

	return true;
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

#endif // USE_API_OAL
