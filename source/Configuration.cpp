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

#include "Configuration.h"


namespace Seed {


Configuration::Configuration()
	: bDebugSprite(FALSE)
	, nVideoMode(Video_AutoDetect)
	, iResolutionWidth(800)
	, iResolutionHeight(600)
	, pcWorkingDirectory(NULL)
	, bMultipleInstances(FALSE)
	, bWarningMultipleInstances(FALSE)
	, iFrameRate(Seed::FrameRateLockAt60)
	, pcTitle(NULL)
	, pcDescription(NULL)
	, pcPublisherName(NULL)
	, iRendererDeviceType(Seed::RendererDeviceOpenGL14)
	, iReaderType(Seed::ReaderDefault)
	, iInputRadius(0.0f)
{
}

Configuration::~Configuration()
{
}

void Configuration::SetVideoMode(eVideoMode videoMode)
{
	nVideoMode = videoMode;
}

eVideoMode Configuration::GetVideoMode() const
{
	return nVideoMode;
}
/*
void Configuration::SetResolution(u32 width, u32 height)
{
	iResolutionWidth = width;
	iResolutionHeight = height;
}
*/
u32 Configuration::GetResolutionWidth() const
{
	return iResolutionWidth;
}

u32 Configuration::GetResolutionHeight() const
{
	return iResolutionHeight;
}

void Configuration::SetWorkingDirectory(const char *path)
{
	pcWorkingDirectory = path;
}

const char *Configuration::GetWorkingDirectory() const
{
	return pcWorkingDirectory;
}

void Configuration::SetFrameRate(eSystemFrameRate frameRate)
{
	iFrameRate = frameRate;
}

eSystemFrameRate Configuration::GetFrameRate() const
{
	return iFrameRate;
}

void Configuration::SetApplicationTitle(const char *title)
{
	pcTitle = title;
}

const char *Configuration::GetApplicationTitle() const
{
	return pcTitle;
}

void Configuration::SetApplicationDescription(const char *desc)
{
	pcDescription = desc;
}

const char *Configuration::GetPublisherName() const
{
	return pcPublisherName;
}

void Configuration::SetPublisherName(const char *desc)
{
	pcPublisherName = desc;
}

const char *Configuration::GetApplicationDescription() const
{
	return pcDescription;
}

void Configuration::SetCanHaveMultipleInstances(bool enable)
{
	bMultipleInstances = enable;
}

bool Configuration::GetCanHaveMultipleInstances() const
{
	return bMultipleInstances;
}

void Configuration::SetWarningMultipleInstances(bool warnUser)
{
	bWarningMultipleInstances = warnUser;
}

bool Configuration::GetWarningMultipleInstances() const
{
	return bWarningMultipleInstances;
}

void Configuration::SetRendererDeviceType(eRendererDeviceType deviceType)
{
	iRendererDeviceType = deviceType;
}

eRendererDeviceType Configuration::GetRendererDeviceType() const
{
	return iRendererDeviceType;
}

void Configuration::SetReaderType(eReaderType readerType)
{
	iReaderType = readerType;
}

eReaderType Configuration::GetReaderType() const
{
	return iReaderType;
}

void Configuration::SetInputDistanceRadius(f32 dist)
{
	iInputRadius = dist;
}

f32 Configuration::GetInputDistanceRadius() const
{
	return iInputRadius;
}

const char *Configuration::GetObjectName() const
{
	return "Configuration";
}

int Configuration::GetObjectType() const
{
	return Seed::ObjectConfiguration;
}


} // namespace
