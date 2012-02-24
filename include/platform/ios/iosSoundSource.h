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

/*! \file iosSoundSource.h
	\author	Danny Angelo Carminati Grein
	\brief Sound source implementation using OpenAL API
*/

#ifndef __IOSONE_SOUND_SOURCE_H__
#define __IOSONE_SOUND_SOURCE_H__

#include "Defines.h"

#if defined(BUILD_IOS)

#include "Sound.h"
#include "interface/ISoundSource.h"
#include "interface/ISound.h"
#include "File.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

namespace Seed { namespace iOS {

class SEED_CORE_API SoundSource : public ISoundSource
{
	friend class SoundSystem;

	public:
		SoundSource();
		virtual ~SoundSource();

		// ISoundSource
		virtual void Load(const char *filename, ResourceManager *res = pResourceManager);
		virtual void Unload();

		virtual void SetLoop(bool b);
		virtual void Play();
		virtual void Stop(f32 ms = 0.0f);
		virtual void Resume();

		virtual void SetVolume(f32 vol);
		virtual void UpdateVolume();

	private:
		SEED_DISABLE_COPY(SoundSource);

	private:
		ALuint			iSource;
		Sound			*pSound;
		File			stFile;
};

}} // namespace

#else // BUILD_IOS
	#error "Include 'SoundSource.h' instead 'platform/api/IphSoundSource.h' directly."
#endif // BUILD_IOS
#endif // __IOSONE_SOUND_SOURCE_H__
