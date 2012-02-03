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

/*! \file IphTimer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Timer Iphone Implementation
*/

#if defined(_IPHONE_)

#include "Defines.h"
#include "Timer.h"
#include "Log.h"

#include <time.h>
#include <sys/time.h>

#define TAG "[Timer] "

namespace Seed { namespace iPhone {

SEED_SINGLETON_DEFINE(Timer);

Timer::Timer()
	: fStart(0)
{
}

Timer::~Timer()
{
}

bool Timer::Initialize()
{
	Log(TAG "Initializing...");
	this->Reset();
	Log(TAG "Initialization completed.");

	return TRUE;
}

bool Timer::Reset()
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	fStart = (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL);

	//fStart = CFAbsoluteTimeGetCurrent();

	return TRUE;
}

bool Timer::Shutdown()
{
	return this->Reset();
}

u64 Timer::GetMilliseconds() const
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	u64 ms = (tv.tv_usec / 1000ULL);
	u64 x =  (tv.tv_sec * 1000ULL);
	u64 ret = x + ms - fStart;
	return ret;

	//return CFAbsoluteTimeGetCurrent() - fStart;
}

void Timer::Sleep(u32 ms) const
{
	UNUSED(ms);
}

}} // namespace

#endif // _IPHONE_
