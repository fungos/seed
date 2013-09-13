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

#include "Sound.h"

#if defined(USE_API_OAL)

#include "Log.h"
#include <vector>

#define TAG "[Sound] "
#define BUFFER_SIZE		8512

namespace Seed { namespace OAL {

IResource *SoundResourceLoader(const String &filename, ResourceManager *res)
{
	auto sound = New(Sound());
	sound->Load(filename, res);
	return sound;
}

Sound::Sound()
	: iBuffer(0)
	, iSize(0)
{
}

Sound::~Sound()
{
	this->Unload();
}

void Sound::Reset()
{
	this->Unload();
}

bool Sound::Load(const String &filename, ResourceManager *res)
{
	if (this->Unload())
	{
		sFilename = filename;
		pRes = res;

		OggVorbis_File	oggStream;
		ov_callbacks	vorbisCb;
		sOggFile		oggFile;
		ALenum			format = AL_FORMAT_MONO16;

		alGenBuffers(1, &iBuffer);

		WARNING(TODO - Move to async file loading)
		auto file = New(File(filename));
		oggFile.dataPtr = file->GetData();
		oggFile.dataRead = 0;
		oggFile.dataSize = iSize = file->GetSize();

		vorbisCb.read_func = vorbis_read;
		vorbisCb.close_func = vorbis_close;
		vorbisCb.seek_func = vorbis_seek;
		vorbisCb.tell_func = vorbis_tell;

		if (ov_open_callbacks(&oggFile, &oggStream, NULL, 0, vorbisCb) != 0)
		{
			Log(TAG "Could not read ogg file from memory");
			memset(&oggFile, '\0', sizeof(oggFile));
		}
		else
		{
			int bitstream = 0;
			long bytes;
			char arr[BUFFER_SIZE];

			ALsizei freq;
			Vector<char> buffer;

			vorbis_info *info = ov_info(&oggStream, -1);

			if (info->channels == 1)
				format = AL_FORMAT_MONO16;
			else if (info->channels == 2)
				format = AL_FORMAT_STEREO16;

			freq = (ALsizei)info->rate;
			do
			{
				bytes = ov_read(&oggStream, arr, BUFFER_SIZE, 0, 2, 1, &bitstream);

				if (bytes < 0)
				{
					ov_clear(&oggStream);
					Log(TAG "Error decoding ogg stream.");
				}

				buffer.insert(buffer.end(), arr, arr + bytes);
			} while (bytes > 0);

			ov_clear(&oggStream);
			alBufferData(iBuffer, format, &buffer[0], static_cast<ALsizei>(buffer.size()), freq);
		}

		bLoaded = true;
		Delete(file);
	}

	return bLoaded;
}

bool Sound::Unload()
{
	alDeleteBuffers(1, &iBuffer);
	bLoaded = false;

	return true;
}

const void *Sound::GetData() const
{
	return static_cast<const void *>(&iBuffer);
}

u32 Sound::GetUsedMemory() const
{
	return ISound::GetUsedMemory() + (u32)sizeof(this) + iSize;
}

}} // namespace

#endif // USE_API_OAL
