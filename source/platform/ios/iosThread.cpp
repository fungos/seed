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

/*! \file iosThread.cpp
	\author	Danny Angelo Carminati Grein
	\brief Thread implementation
*/

#if defined(BUILD_IOS)

#include "platform/ios/iosThread.h"
#include "Log.h"

#define TAG 	"[Thread] "

namespace Seed { namespace iOS {

static void *__seed_thread_loop_callback(void *param)
{
	Thread *pt = static_cast<Thread *>(param);
	while (pt->Run());
	pt->Destroy();

	return NULL;
}

Thread::Thread()
	: bCreated(FALSE)
	, bRunning(FALSE)
{
}

Thread::~Thread()
{
	this->Destroy();
}

void Thread::Destroy()
{
	bRunning = FALSE;
	if (bCreated)
	{
		pthread_exit(NULL);
		bCreated = FALSE;
	}
	thread = 0;
}

void Thread::Create()
{
	bRunning = TRUE;
	if (!bCreated)
	{
		int err = pthread_create(&thread, 0, __seed_thread_loop_callback, (void *)this);
		UNUSED(err);
		ASSERT_MSG(err == 0, TAG "Failed to create thread.");
		bCreated = TRUE;
	}
}

bool Thread::Run()
{
	return TRUE;
}

}} // namespace

#endif // BUILD_IOS
