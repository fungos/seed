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
