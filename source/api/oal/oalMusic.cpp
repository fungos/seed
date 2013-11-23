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
#include "Memory.h"

#define TAG "[Music] "

#if defined(DEBUG)
//#define AL_CHECK(x)	x; { if (ALenum _e = alGetError() != AL_NO_ERROR) { fprintf(stdout, TAG "Error %d - ", _e); SEED_ASSERT_MSG(false, "Crashing."); }}
#define AL_CHECK(x)	x;
#else
#define AL_CHECK(x)	x;
#endif

namespace Seed { namespace OAL {

IResource *MusicResourceLoader(const String &filename, ResourceManager *res)
{
	auto music = sdNew(Music);
	music->Load(filename, res);

	return music;
}

Music::Music()
	: pFile(NULL)
	, iBuffers()
	, iSource(0)
	, vorbisInfo(NULL)
	, vorbisComment(NULL)
	, oggStream()
	, oggFile()
	, vorbisCb()
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
		sFilename = filename;
		pRes = res;

		ALenum err = AL_NO_ERROR;
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

			Info(TAG "Could not generate OpenAL music buffers (0x%04x).", err);
		}

		// TODO: Now File will load all data to a memory allocated buffer, for music this means something big if we are in a resource limited device.
		//		 We need to make File able to memmap the file contents to a virtual memory address so this will be transparent to the vorbis reader
		//		 as it will be streaming from disk.
		//		 Also reading resources from different platforms (asynchronous like dvd reading on wii or nacl web files) will be more natural.
		// FIXME: ASYNC
		pFile = sdNew(File(sFilename));
		oggFile.dataPtr = pFile->GetData();
		oggFile.dataRead = 0;
		oggFile.dataSize = pFile->GetSize();

		vorbisCb.read_func = vorbis_read;
		vorbisCb.close_func = vorbis_close;
		vorbisCb.seek_func = vorbis_seek;
		vorbisCb.tell_func = vorbis_tell;

		if (ov_open_callbacks(&oggFile, &oggStream, NULL, 0, vorbisCb) != 0)
		{
			Log(TAG "Could not read ogg stream (%s).", filename.c_str());
			memset(&oggFile, '\0', sizeof(oggFile));

			AL_CHECK(alDeleteSources(1, &iSource));
			AL_CHECK(alDeleteBuffers(OPENAL_MUSIC_BUFFERS, iBuffers));
			memset(iBuffers, '\0', sizeof(iBuffers));
			bLoaded = false;

			return bLoaded;
		}

		vorbisInfo = ov_info(&oggStream, -1);
		vorbisComment = ov_comment(&oggStream, -1);

		if (vorbisInfo->channels > 1)
			eFormat = AL_FORMAT_STEREO16;

		AL_CHECK(alSource3f(iSource, AL_POSITION, 0.0f, 0.0f, 0.0f));
		AL_CHECK(alSource3f(iSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f));
		AL_CHECK(alSource3f(iSource, AL_DIRECTION, 0.0f, 0.0f, 0.0f));
		AL_CHECK(alSourcef(iSource, AL_ROLLOFF_FACTOR, 0.0f));
		AL_CHECK(alSourcei(iSource, AL_SOURCE_RELATIVE, AL_TRUE));

		this->Reset();
		bLoaded = true;
	}

	return bLoaded;
}

bool Music::Unload()
{
	if (!bLoaded)
		return true;

	pSoundSystem->StopMusic(0, this);
	nState = eMusicState::Stopped;
	eFormat = AL_FORMAT_MONO16;
	fVolume = 1.0f;

	alGetError();
	if (iSource)
	{
		alSourceStop(iSource);
		alGetError();

		int queued = 0;
		/*AL_CHECK*/(alGetSourcei(iSource, AL_BUFFERS_QUEUED, &queued));

		while (queued--)
		{
			ALuint buffer;
			/*AL_CHECK*/(alSourceUnqueueBuffers(iSource, 1, &buffer));
		}

		/*AL_CHECK*/(alDeleteSources(1, &iSource));
		iSource = 0;
	}

	/*AL_CHECK*/(alDeleteBuffers(OPENAL_MUSIC_BUFFERS, iBuffers));
	memset(iBuffers, '\0', sizeof(iBuffers));

	ov_clear(&oggStream);
	bLoaded = false;
	sdDelete(pFile);

	return true;
}

void Music::Reset()
{
	int queued = 0;
	AL_CHECK(alGetSourcei(iSource, AL_BUFFERS_QUEUED, &queued));

	while (queued--)
	{
		ALuint buffer;
		AL_CHECK(alSourceUnqueueBuffers(iSource, 1, &buffer));
	}

	this->SetVolume(fVolume);

	ov_raw_seek(&oggStream, 0);
	for (u32 i = 0; i < OPENAL_MUSIC_BUFFERS; i++)
		this->DoStream(iBuffers[i]);

	AL_CHECK(alSourceQueueBuffers(iSource, OPENAL_MUSIC_BUFFERS, iBuffers));
}

bool Music::Update(Seconds dt)
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
		nState = eMusicState::Stopped;

	return active;
}

bool Music::DoStream(ALuint buffer)
{
	return ogg_update_stream(&oggStream, vorbisInfo->rate, eFormat, buffer, bLoop);
}

void Music::SetVolume(f32 vol)
{
	IMusic::SetVolume(vol);
	AL_CHECK(alSourcef(iSource, AL_GAIN, fVolume * pSoundSystem->GetMusicVolume()));
}

void Music::UpdateVolume()
{
	AL_CHECK(alSourcef(iSource, AL_GAIN, fVolume * pSoundSystem->GetMusicVolume()));
}

const void *Music::GetData() const
{
	return iBuffers;
}

}} // namespace

#endif // USE_API_OAL
