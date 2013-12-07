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

#if defined(BUILD_IOS)

#include "Log.h"
#include "platform/ios/iosView.h"
#include <Foundation/Foundation.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AVFoundation/AVFoundation.h>
#include <Foundation/NSURL.h>
#include <Foundation/NSBundle.h>

#define TAG "[Sound] "

#define AUDIO_FRAME_TIME 3
#define AUDIO_DATA_PATH		"/"
#define AUDIO_DATA_EXT		".caf"

typedef ALvoid AL_APIENTRY (*alBufferDataStaticProcPtr) (const ALint bid, ALenum format, ALvoid *data, ALsizei size, ALsizei freq);
ALvoid  alBufferDataStaticProc(const ALint bid, ALenum format, ALvoid *data, ALsizei size, ALsizei freq)
{
	static alBufferDataStaticProcPtr proc = nullptr;

	if (proc == nullptr)
	{
		proc = (alBufferDataStaticProcPtr)alcGetProcAddress(nullptr, (const ALCchar *)"alBufferDataStatic");
	}

	if (proc)
		proc(bid, format, data, size, freq);
}

namespace Seed { namespace iOS {

IResource *SoundResourceLoader(const String &filename, ResourceManager *res)
{
	auto sound = New(Sound());
	sound->Load(filename, res);
	return sound;
}

Sound::Sound()
	: iBuffer(0)
	, iSize(0)
	, iFreq(0)
	, eFormat(0)
	, pData(nullptr)
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

		this->ReadData(filename.c_str());

		bLoaded = true;
	}

	return bLoaded;
}

bool Sound::Unload()
{
	if (alIsBuffer(iBuffer))
		alDeleteBuffers(1, &iBuffer);

	Free(pData);
	eFormat = 0;
	iFreq = 0;
	iSize = 0;
	iBuffer = 0;

	bLoaded = false;

	return true;
}

const void *Sound::GetData() const
{
	return static_cast<const void *>(&iBuffer);
}

u32 Sound::GetUsedMemory() const
{
	return ISound::GetUsedMemory() + sizeof(this) + iSize;
}

void Sound::ReadData(const char *file)
{
	//Info("SOUND: %s", file);
	OSStatus					err = noErr;
	UInt64                      fileDataSize = 0;
	AudioStreamBasicDescription theFileFormat;
	UInt32                      thePropertySize = sizeof(theFileFormat);
	AudioFileID                 afid = 0;
	void						*theData = nullptr;
	UInt32						dataSize = 0;

	NSString *root = [NSString stringWithCString: iosGetRootPath() encoding: [NSString defaultCStringEncoding]];
	NSString *fname = [NSString stringWithCString: file encoding: [NSString defaultCStringEncoding]];
	//NSString *ext = [NSString stringWithCString: AUDIO_DATA_EXT encoding: [NSString defaultCStringEncoding]];

	NSString *path = fname; //[fname stringByAppendingString: ext];
	path = [@AUDIO_DATA_PATH stringByAppendingString: path];
	path = [root stringByAppendingString: path];

	CFURLRef fileURL = CFURLCreateWithFileSystemPath(nullptr, (CFStringRef)path, kCFURLPOSIXPathStyle, false);
	err = AudioFileOpenURL(fileURL, kAudioFileReadPermission, 0, &afid);
	if (err)
		{ Log(TAG "ReadAudioData: file %s%s%s not found, Error = %ld.", AUDIO_DATA_PATH, file, AUDIO_DATA_EXT, err); goto Exit; }

	err = AudioFileGetProperty(afid, kAudioFilePropertyDataFormat, &thePropertySize, &theFileFormat);
	if (err)
		{ Log(TAG "ReadAudioData: AudioFileGetProperty(kAudioFileProperty_DataFormat) FAILED, Error = %ld, File: %s%s%s.", err, AUDIO_DATA_PATH, file, AUDIO_DATA_EXT); goto Exit; }

	if (theFileFormat.mChannelsPerFrame > 2)
	{
		Log(TAG "ReadAudioData - Unsupported Format, channel count is greater than stereo, File: %s%s%s.", AUDIO_DATA_PATH, file, AUDIO_DATA_EXT);
		goto Exit;
	}

	if ((theFileFormat.mFormatID != kAudioFormatLinearPCM) || (!TestAudioFormatNativeEndian(theFileFormat)))
	{
		Log(TAG "ReadAudioData - Unsupported Format, must be little-endian PCM, File: %s%s%s.", AUDIO_DATA_PATH, file, AUDIO_DATA_EXT);
		goto Exit;
	}

	if ((theFileFormat.mBitsPerChannel != 8) && (theFileFormat.mBitsPerChannel != 16))
	{
		Log(TAG "ReadAudioData - Unsupported Format, must be 8 or 16 bit PCM, File: %s%s%s.", AUDIO_DATA_PATH, file, AUDIO_DATA_EXT);
		goto Exit;
	}

	thePropertySize = sizeof(fileDataSize);
	err = AudioFileGetProperty(afid, kAudioFilePropertyAudioDataByteCount, &thePropertySize, &fileDataSize);

	if (err)
		{ Log(TAG "ReadAudioData: AudioFileGetProperty(kAudioFilePropertyAudioDataByteCount) FAILED, Error = %ld, File: %s%s%s.", err, AUDIO_DATA_PATH, file, AUDIO_DATA_EXT); goto Exit; }

	dataSize = static_cast<UInt32>(fileDataSize);
	theData = Alloc(dataSize);
	if (theData)
	{
		//Info("AudioFileReadBytes: %x, false, 0, %d, theData", afid, dataSize);
		err = AudioFileReadBytes(afid, false, 0, &dataSize, theData);
		//Info("AudioFileReadBytes: %x, false, 0, %d, theData = %d", afid, dataSize, result);

		if (err == noErr)
		{
			iSize = (ALsizei)dataSize;

			if (theFileFormat.mBitsPerChannel == 8)
				eFormat = (theFileFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
			else
				eFormat = (theFileFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

			iFreq = (ALsizei)theFileFormat.mSampleRate;
			pData = theData;

			ALenum error = alGetError();

			alGenBuffers(1, &iBuffer);

			error = alGetError();

			alBufferDataStaticProc(iBuffer, eFormat, pData, iSize, iFreq);

			error = alGetError();

			if (error != AL_NO_ERROR)
			{
				Log(TAG "alBufferDataStaticProc buffer %d : error %d, File: %s%s%s.", iBuffer, error, AUDIO_DATA_PATH, file, AUDIO_DATA_EXT);

				Free(theData);
			}
		}
		else
		{
			Free(theData);

			Log(TAG "ReadAudioData: ExtAudioFileRead FAILED, Error = %ld, File: %s%s%s.", err, AUDIO_DATA_PATH, file, AUDIO_DATA_EXT);
		}

	}

Exit:
	if (afid)
		AudioFileClose(afid);
}


}} // namespace

#endif // BUILD_IOS
