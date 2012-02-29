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

#if defined(BUILD_IOS)

#include "Log.h"
#include "SoundSystem.h"
#include "platform/ios/iosoneView.h"
#include <Foundation/NSURL.h>
#include <Foundation/NSBundle.h>
#include <Foundation/Foundation.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AVFoundation/AVFoundation.h>

#define TAG "[Music] "

namespace Seed { namespace iOS {

IResource *MusicResourceLoader(const String &filename, ResourceManager *res)
{
	UNUSED(res);

	Music *music = New(Music());
	music->Load(filename, res, pool);

	return music;
}

Music::Music()
	: bLoop(true)
	, stFile()
{
}

Music::~Music()
{
	this->Unload();
}

bool Music::Load(const String &filename, ResourceManager *res)
{
	ASSERT_NULL(filename);
	ASSERT_NULL(res);

	if (pSoundSystem->IsInitialized() && this->Unload())
	{
		sFilename = filename;
		pRes = res;

		/* Open file .music */
		/* FIXME: This should be from a file resource acquired using a Get() from resource manager cache. */
		SECURITY_CHECK(pFileSystem->Open(filename, &stFile), "Music object couldn't be opened");

		const u8 *ptr = static_cast<const u8 *>(stFile.GetData());
		ObjectHeader *block = NULL;
		READ_STRUCT(block, ObjectHeader, ptr);
		SECURITY_CHECK(seed_validate_block(&stFile, block, MUSIC_OBJECT_MAGIC, MUSIC_OBJECT_VERSION), "Invalid block header for music.");

		u32 volume = 0;
		READ_U32(volume, ptr);
		fVolume = (volume / 100.0f);

		const char *fname = NULL;
		READ_STR(fname, ptr);
		ASSERT_NULL(fname);

		this->Reset();

		{
			NSString *root = [NSString stringWithCString: iphGetRootPath() encoding: [NSString defaultCStringEncoding]];
			NSString *musicName = [NSString stringWithCString: fname encoding: [NSString defaultCStringEncoding]];
			//NSString *extensionName = [NSString stringWithCString: ".mp3" encoding: [NSString defaultCStringEncoding]];
			NSString *dataPath = [@"/data/" stringByAppendingString: musicName]; //[musicName stringByAppendingString: extensionName]];
			NSString *path = [root stringByAppendingString: dataPath];
			NSError *err = NULL;
			Log(TAG "Music: %s", [path cStringUsingEncoding: NSASCIIStringEncoding]);
			AVAudioPlayer *p =	[[[ AVAudioPlayer alloc ] initWithContentsOfURL: [ NSURL fileURLWithPath: path ] error: &err ] retain ];
			pAVPlayer = (void *)p;

			if (!err)
			{
				p.numberOfLoops = -1; // inf
				p.volume = fVolume * pSoundSystem->GetMusicVolume();
				[p prepareToPlay];
				//[p play];

				bLoaded = true;
			}
			else
			{
				Log(TAG "Error: %s", [[err localizedDescription] cStringUsingEncoding: NSASCIIStringEncoding]);
				Log(TAG "Error happened when trying to play music %s.", fname);
				bLoaded = false;
			}
		}
	}

	return bLoaded;
}

bool Music::Unload()
{
	if (!bLoaded)
		return true;

	eState = Seed::MusicStopped;
	fVolume = 1.0f;

	pSoundSystem->StopMusic(0, this);
	stFile.Close();

	bLoaded = false;

	AVAudioPlayer *p = (AVAudioPlayer *)pAVPlayer;
	if (p)
	{
		[p stop];
		[p release];
		pAVPlayer = NULL;
	}

	return true;
}

void Music::Reset()
{
	this->SetVolume(fVolume);
}

bool Music::Update(f32 dt)
{
	UNUSED(dt);

	if (!pAVPlayer)
		return false;

	AVAudioPlayer *p = (AVAudioPlayer *)pAVPlayer;
	if (eState == Seed::MusicPlay || eState == Seed::MusicFadeIn)
	{
		[p play];
	}
	else if (eState == Seed::MusicStop)
	{
		[p stop];
	}
	else if (eState == Seed::MusicPause)
	{
		[p pause];
	}

	return (eState != Seed::MusicStopped && eState != Seed::MusicStop);
}

void Music::SetVolume(f32 vol)
{
	IMusic::SetVolume(vol);

	if (pAVPlayer)
	{
		AVAudioPlayer *p = (AVAudioPlayer *)pAVPlayer;
		p.volume = fVolume * pSoundSystem->GetMusicVolume();
	}
}

void Music::UpdateVolume()
{
	if (pAVPlayer)
	{
		AVAudioPlayer *p = (AVAudioPlayer *)pAVPlayer;
		p.volume = fVolume * pSoundSystem->GetMusicVolume();
	}
}

void Music::FadeVolume(f32 vol)
{
	if (pAVPlayer)
	{
		AVAudioPlayer *p = (AVAudioPlayer *)pAVPlayer;
		p.volume = vol * pSoundSystem->GetMusicVolume();
	}
}

const void *Music::GetData() const
{
	return &stFile;
}

}} // namespace

#endif // BUILD_IOS
