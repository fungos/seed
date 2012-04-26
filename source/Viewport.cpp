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

#include "Viewport.h"
#include "Log.h"
#include "Renderer.h"
#include "RendererDevice.h"

namespace Seed {

Viewport::Viewport()
	: pRenderer(NULL)
	, fX(0.0f)
	, fY(0.0f)
	, fWidth(0.0f)
	, fHeight(0.0f)
{
}

Viewport::~Viewport()
{
}

void Viewport::SetRenderer(Renderer *renderer)
{
	SEED_ASSERT(renderer);
	pRenderer = renderer;
}

void Viewport::SetPosition(f32 x, f32 y)
{
	fX = x;
	fY = y;
}

void Viewport::SetWidth(f32 w)
{
	fWidth = w;
}

void Viewport::SetHeight(f32 h)
{
	fHeight = h;
}

Renderer *Viewport::GetRenderer() const
{
	return pRenderer;
}

f32 Viewport::GetX() const
{
	return fX;
}

f32 Viewport::GetY() const
{
	return fY;
}

f32 Viewport::GetWidth() const
{
	return fWidth;
}

f32 Viewport::GetHeight() const
{
	return fHeight;
}

void Viewport::Render()
{
	if (pRenderer)
	{
		pRendererDevice->SetViewport(fX, fY, fWidth, fHeight);
		pRenderer->Render();
	}
}

bool Viewport::Contains(f32 x, f32 y)
{
	return (x >= fX && x <= fX + fWidth && y >= fY && y <= fY + fHeight);
}

const String Viewport::GetObjectName() const
{
	return "Viewport";
}

} // namespace
