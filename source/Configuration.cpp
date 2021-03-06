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
#include "File.h"
#include "Reader.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Memory.h"
#include "SeedInit.h"
#include <algorithm>

namespace Seed {

SEED_SINGLETON_DEFINE(Configuration)

Configuration::Configuration()
	: sWorkingDirectory("./")
	, sTitle("")
	, sDescription("")
	, sPublisherName("")
	, nRendererDeviceType(eRendererDeviceType::Auto)
	, nReaderType(eReaderType::Default)
	, iFrameRate(60)
	, iResolutionWidth(800)
	, iResolutionHeight(600)
	, bMultipleInstances(false)
	, bWarningMultipleInstances(false)
	, bFullScreen(false)
	, bShowCursor(false)
	, bDebugSprite(false)
{
}

Configuration::~Configuration()
{
}

void Configuration::Load(const String &file)
{
	// This must not be async, it is a starting configuration file
	File *f = sdNew(File(file));
	if (f && f->GetData())
	{
		Reader r(f);
		bDebugSprite = r.ReadBool("bDebugSprite", false);
		bMultipleInstances = r.ReadBool("bMultipleInstances", false);
		bWarningMultipleInstances = r.ReadBool("bWarningMultipleInstances", false);
		bFullScreen = r.ReadBool("bFullScreen", false);
		bShowCursor = r.ReadBool("bShowCursor", false);

		// Overwritten by command line
		if (!Private::bDisableResourceLoader)
			Private::bDisableResourceLoader = r.ReadBool("bDisableResourceLoader", false);
		if (!Private::bDisableThread)
			Private::bDisableThread = r.ReadBool("bDisableThread", false);
		if (!Private::bDisableSound)
			Private::bDisableSound = r.ReadBool("bDisableSound", false);

		String renderer = r.ReadString("sRendererDevice", "auto");

		// FIXME: A better way to select the renderer (via register/unregister handlers?)
		// also, a way to detect the default system renderer.
		// FIXME: Clean up this, only what is really needed.
		if (StringUtil::Equals(renderer, "auto"))
			nRendererDeviceType = eRendererDeviceType::Auto;
		else if (StringUtil::Equals(renderer, "ogl") || StringUtil::Equals(renderer, "opengl"))
			nRendererDeviceType = eRendererDeviceType::OpenGLAny;
		else if (StringUtil::Equals(renderer, "ogles1") || StringUtil::Equals(renderer, "opengl es1"))
			nRendererDeviceType = eRendererDeviceType::OpenGLES1;
		else if (StringUtil::Equals(renderer, "ogles2") || StringUtil::Equals(renderer, "opengl es2"))
			nRendererDeviceType = eRendererDeviceType::OpenGLES2;
		else if (StringUtil::Equals(renderer, "ogl2") || StringUtil::Equals(renderer, "opengl 2.x"))
			nRendererDeviceType = eRendererDeviceType::OpenGL2x;
		else if (StringUtil::Equals(renderer, "ogl3") || StringUtil::Equals(renderer, "opengl 3.x"))
			nRendererDeviceType = eRendererDeviceType::OpenGL3x;
		else if (StringUtil::Equals(renderer, "ogl4") || StringUtil::Equals(renderer, "opengl 4.x"))
			nRendererDeviceType = eRendererDeviceType::OpenGL4x;
		else if (StringUtil::Equals(renderer, "dx8") || StringUtil::Equals(renderer, "directx 8"))
			nRendererDeviceType = eRendererDeviceType::DirectX8;
		else if (StringUtil::Equals(renderer, "dx9") || StringUtil::Equals(renderer, "directx 9"))
			nRendererDeviceType = eRendererDeviceType::DirectX9;
		else if (StringUtil::Equals(renderer, "dx10") || StringUtil::Equals(renderer, "directx 10"))
			nRendererDeviceType = eRendererDeviceType::DirectX10;
		else if (StringUtil::Equals(renderer, "dx11") || StringUtil::Equals(renderer, "directx 11"))
			nRendererDeviceType = eRendererDeviceType::DirectX11;
		else
			Log("[Configuration] Unknown renderer %s - fallbacking to OpenGL 1.x.", renderer.c_str());

		sWorkingDirectory = r.ReadString("sWorkingDirectory", "./");
		sTitle = r.ReadString("sTitle", "");
		sDescription = r.ReadString("sDescription", "");
		sPublisherName = r.ReadString("sPublisherName", "");

		iResolutionWidth = r.ReadU32("iResolutionWidth", 800);
		iResolutionHeight = r.ReadU32("iResolutionHeight", 600);
		iFrameRate = r.ReadU32("iFrameRate", 60);
	}

	sdDelete(f);
}

u32 Configuration::GetResolutionWidth() const
{
	return iResolutionWidth;
}

u32 Configuration::GetResolutionHeight() const
{
	return iResolutionHeight;
}

void Configuration::SetWorkingDirectory(const String path)
{
	sWorkingDirectory = path;
}

const String &Configuration::GetWorkingDirectory() const
{
	return sWorkingDirectory;
}

void Configuration::SetFrameRate(u32 frameRate)
{
	iFrameRate = frameRate;
}

u32 Configuration::GetFrameRate() const
{
	return iFrameRate;
}

void Configuration::SetApplicationTitle(const String &title)
{
	sTitle = title;
}

const String &Configuration::GetApplicationTitle() const
{
	return sTitle;
}

void Configuration::SetApplicationDescription(const String &desc)
{
	sDescription = desc;
}

bool Configuration::IsCursorEnabled() const
{
	return bShowCursor;
}

void Configuration::EnableCursor(bool b)
{
	bShowCursor = b;
}

const String &Configuration::GetPublisherName() const
{
	return sPublisherName;
}

void Configuration::SetPublisherName(const String desc)
{
	sPublisherName = desc;
}

const String &Configuration::GetApplicationDescription() const
{
	return sDescription;
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

void Configuration::SetFullScreen(bool enable)
{
	bFullScreen = enable;
}

bool Configuration::GetFullScreen() const
{
	return bFullScreen;
}

void Configuration::SetRendererDeviceType(eRendererDeviceType deviceType)
{
	nRendererDeviceType = deviceType;
}

eRendererDeviceType Configuration::GetRendererDeviceType() const
{
	return nRendererDeviceType;
}

void Configuration::SetReaderType(eReaderType readerType)
{
	nReaderType = readerType;
}

eReaderType Configuration::GetReaderType() const
{
	return nReaderType;
}

} // namespace
