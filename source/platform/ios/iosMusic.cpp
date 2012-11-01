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

#if defined(BUILD_IOS)

#include "Music.h"
#include "Log.h"
#include "SoundSystem.h"
#include "platform/ios/iosView.h"
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
	music->Load(filename, res);

	return music;
}

Music::Music()
	: pAVPlayer(NULL)
	, bLoop(true)
{
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

		this->Reset();

		{
			NSString *root = [NSString stringWithCString: iosGetRootPath() encoding: [NSString defaultCStringEncoding]];
			NSString *musicName = [NSString stringWithCString: filename.c_str() encoding: [NSString defaultCStringEncoding]];
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
				Log(TAG "Error happened when trying to play music %s.", filename.c_str());
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
	return (void *)pAVPlayer;
}

}} // namespace

#endif // BUILD_IOS
