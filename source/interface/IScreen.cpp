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

#include "interface/IScreen.h"
#include "Log.h"

namespace Seed {

IScreen::IScreen()
	: bFading(false)
	, nMode(Seed::Video_AutoDetect)
	, fAspectRatio(static_cast<f32>(PLATFORM_MAX_SCREEN_HEIGHT)/static_cast<f32>(PLATFORM_MAX_SCREEN_WIDTH))
	, iHeight(PLATFORM_MAX_SCREEN_HEIGHT)
	, iWidth(PLATFORM_MAX_SCREEN_WIDTH)
{
}

IScreen::~IScreen()
{
}

bool IScreen::IsFading() const
{
	return bFading;
}

void IScreen::SetMode(eVideoMode mode)
{
	nMode = mode;
}

eVideoMode IScreen::GetMode() const
{
	return nMode;
}

void IScreen::ToggleFullscreen()
{
	SEED_ABSTRACT_METHOD;
}

bool IScreen::HasWindowedMode() const
{
	SEED_ABSTRACT_METHOD;
	return false;
}

bool IScreen::IsFullscreen() const
{
	SEED_ABSTRACT_METHOD;
	return true;
}

void IScreen::Update()
{
	SEED_ABSTRACT_METHOD;
}

u32 IScreen::GetHeight() const
{
	return iHeight;
}

u32 IScreen::GetWidth() const
{
	return iWidth;
}

f32 IScreen::GetAspectRatio() const
{
	return fAspectRatio;
}

void IScreen::FadeIn()
{
}

void IScreen::FadeOut()
{
}

bool IScreen::IsRequired() const
{
	return true;
}

const char *IScreen::GetObjectName() const
{
	return "Screen";
}

} // namespace

