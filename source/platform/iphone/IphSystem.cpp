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

/*! \file IphSystem.cpp
	\author	Danny Angelo Carminati Grein
	\brief System Iphone Implementation
*/

#if defined(_IPHONE_)

#include "Defines.h"
#include "System.h"
#include "Log.h"
#include "FileSystem.h"
#include "platform/iphone/IphoneView.h"

#define TAG "[System] "

namespace Seed { namespace iPhone {

SEED_SINGLETON_DEFINE(System);

System::System()
	: iRetraceCount(0)
	, iFrameRate(Seed::FrameRateLockAt60)
{
}

System::~System()
{
}

bool System::Reset()
{
	return TRUE;
}

bool System::Initialize()
{
	Log(TAG "Initializing...");
	Log(TAG "Initialization completed.");

	pFileSystem->SetWriteableDirectory(iphGetHomePath());

	return TRUE;
}

bool System::Shutdown()
{
	Log(TAG "Terminated.");
	Log(TAG "Terminated.");

	return TRUE;
}

bool System::Update(f32 delta)
{
	UNUSED(delta);

	this->WaitForRetrace(this->iFrameRate);

	return TRUE;
}

void System::Sleep()
{
	Log(TAG "WARNING: Platform doesnt support sleep mode.");
}

bool System::IsSleeping() const
{
	return FALSE;
}

bool System::IsShuttingDown() const
{
	return FALSE;
}

bool System::IsResetting() const
{
	return FALSE;
}

void System::WaitForRetrace(eSystemFrameRate rate)
{
	UNUSED(rate);
	// This platform is synced by NSTimer at AppView
	this->iRetraceCount = 0;
}

void System::GoToMenu()
{
}

void System::OnHomeCalled()
{
}

void System::GoToDataManager()
{
}

void System::HangUp()
{
}

void System::DisableHome()
{
}

void System::EnableHome()
{
}

bool System::IsHomeEnabled() const
{
	return FALSE;
}

bool System::IsHomeRunning() const
{
	return FALSE;
}

bool System::InitializeHome()
{
	return FALSE;
}

}} // namespace

#endif // _IPHONE_
