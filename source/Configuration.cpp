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

#include "Configuration.h"

namespace Seed {

Configuration::Configuration()
	: bDebugSprite(false)
	, pcWorkingDirectory(NULL)
	, pcTitle(NULL)
	, pcDescription(NULL)
	, pcPublisherName(NULL)
	, fInputRadius(0.0f)
	, iRendererDeviceType(Seed::RendererDeviceOpenGL14)
	, iReaderType(Seed::ReaderDefault)
	, nVideoMode(Video_AutoDetect)
	, iFrameRate(Seed::FrameRateLockAt60)
	, iResolutionWidth(800)
	, iResolutionHeight(600)
	, bMultipleInstances(false)
	, bWarningMultipleInstances(false)
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
	fInputRadius = dist;
}

f32 Configuration::GetInputDistanceRadius() const
{
	return fInputRadius;
}

const String Configuration::GetObjectName() const
{
	return "Configuration";
}

int Configuration::GetObjectType() const
{
	return Seed::ObjectConfiguration;
}

} // namespace
