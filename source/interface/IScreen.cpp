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
#include "RendererDevice.h"

#define FADE_OUT_COLOR	0xff
#define FADE_OUT_SOLID	0xff
#define FADE_OUT_TRANS	0x00
#define FADE_INCREMENT	0x04

namespace Seed {

IScreen::IScreen()
	: fAspectRatio(1.0f)
	, iHeight(0)
	, iWidth(0)
	, iFadeStatus(0)
	, nFadeType(eFade::In)
	, bFading(false)
{
}

IScreen::~IScreen()
{
}

bool IScreen::IsFading() const
{
	return bFading;
}

void IScreen::EnableCursor(bool b)
{
	UNUSED(b)
	SEED_ABSTRACT_METHOD;
}

void IScreen::ToggleCursor()
{
	SEED_ABSTRACT_METHOD;
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

void IScreen::FadeOut()
{
	if (bFading)
		return;

	bFading = true;
	nFadeType = eFade::Out;
	iFadeStatus = FADE_OUT_TRANS;
}

void IScreen::FadeIn()
{
	if (bFading)
		return;

	bFading = true;
	nFadeType = eFade::In;
	iFadeStatus = FADE_OUT_SOLID;
}

void IScreen::CancelFade()
{
	bFading = false;
	iFadeStatus = FADE_OUT_TRANS;
}

void IScreen::ApplyFade()
{
	if (bFading == false)
		return;

	if (nFadeType == eFade::In)
	{
		iFadeStatus -= FADE_INCREMENT;
		if (iFadeStatus <= FADE_OUT_TRANS)
		{
			bFading = false;
			iFadeStatus = FADE_OUT_TRANS;
		}
	}
	else
	{
		iFadeStatus += FADE_INCREMENT;
		if (iFadeStatus >= FADE_OUT_SOLID)
		{
			bFading = false;
			iFadeStatus = FADE_OUT_SOLID;
		}
	}

	u8 c = static_cast<u8>(iFadeStatus & 0xff);
	pRendererDevice->BackbufferFill(Color(0u, 0u, 0u, c));
}

bool IScreen::IsRequired() const
{
	return true;
}

} // namespace

