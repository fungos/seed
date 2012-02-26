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

#ifndef __IPLAYABLE_H__
#define __IPLAYABLE_H__

#include "Defines.h"
#include "Enum.h"

namespace Seed {
/*
class IPlayable
{
	public:
		IPlayable();
		virtual ~IPlayable();

		virtual void GotoAndPlay(u32 frame);
		virtual void GotoAndStop(u32 frame);
		virtual void Play();
		virtual bool IsPlaying() const;

		virtual void Stop();
		virtual bool IsStopped() const;

		virtual void Pause();
		virtual bool IsPaused() const;

		virtual void SetLoop(bool loop);
		virtual bool IsLoop() const;

		virtual void Seek(f32 d);
		virtual bool IsSeekable() const;

		virtual void Rewind();
		virtual bool IsFinished() const;

		virtual u32 GetNumFrames() const;
		virtual f32 GetFrameRate() const;

	private:
		SEED_DISABLE_COPY(IPlayable);

	protected:
		f32 fFrameRate;
		u32 iTotalFrames;
		ePlayableState	nState;
		bool bFinished;
		bool bLoop;
};
*/
} // namespace

#endif // __IPLAYABLE_H__
