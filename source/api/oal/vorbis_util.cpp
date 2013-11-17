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

#include "Defines.h"

#if defined(USE_API_OAL)

#define OV_EXCLUDE_STATIC_CALLBACKS
#include "api/oal/vorbis_util.h"

size_t vorbis_read(void *ptr, size_t byteSize, size_t sizeToRead, void *datasource)
{
	size_t spaceToEOF;
	size_t actualSizeToRead;
	sOggFile *vorbisData;

	vorbisData = (sOggFile *)datasource;

	spaceToEOF = static_cast<size_t>(vorbisData->dataSize - vorbisData->dataRead);
	if ((sizeToRead*byteSize) < spaceToEOF)
		actualSizeToRead = (sizeToRead*byteSize);
	else
		actualSizeToRead = spaceToEOF;

	if (actualSizeToRead)
	{
		memcpy(ptr, (char*)vorbisData->dataPtr + vorbisData->dataRead, actualSizeToRead);
		vorbisData->dataRead += (actualSizeToRead);
	}

	return actualSizeToRead;
}


int vorbis_seek(void *datasource, ogg_int64_t offset, int whence)
{
	ogg_int64_t actualOffset;
	sOggFile *vorbisData;

	vorbisData = (sOggFile *)datasource;

	switch (whence)
	{
		case SEEK_SET:
		{
			if (vorbisData->dataSize >= offset)
				actualOffset = offset;
			else
				actualOffset = vorbisData->dataSize;
			vorbisData->dataRead = (int)actualOffset;
		}
		break;

		case SEEK_CUR:
		{
			size_t spaceToEOF = static_cast<size_t>(vorbisData->dataSize - vorbisData->dataRead);
			if (offset < (ogg_int64_t)spaceToEOF)
				actualOffset = (offset);
			else
				actualOffset = spaceToEOF;
			vorbisData->dataRead += actualOffset;
		}
		break;

		case SEEK_END:
		{
			vorbisData->dataRead = vorbisData->dataSize + 1;
		}
		break;

		default:
		break;
	};

	return 0;
}


int vorbis_close(void *datasource)
{
	UNUSED(datasource);
	return 1;
}


long vorbis_tell(void *datasource)
{
	sOggFile *vorbisData = (sOggFile *)datasource;
	return static_cast<long>(vorbisData->dataRead);
}


bool ogg_update_stream(OggVorbis_File *oggStream, ogg_int64_t rate, ALenum format, ALuint buffer, bool loop)
{
	char pcm[VORBIS_BUFFER_SIZE];
	int size = 0;
	int section;

	while (size < VORBIS_BUFFER_SIZE)
	{
		int result = (int)ov_read(oggStream, pcm + size, VORBIS_BUFFER_SIZE - size, 0, 2, 1, &section);

		if (result > 0)
			size += result;
		else
			if (result < 0)
				return false;
			else // end of file
			{
				if (loop)
					ov_raw_seek(oggStream, 0);
				else
					break;
			}
	}

	if (size == 0)
	{
		ov_raw_seek(oggStream, 0);
		return false;
	}

	alBufferData(buffer, format, pcm, size, static_cast<ALsizei>(rate));

	return true;
}

#endif // USE_API_OAL
