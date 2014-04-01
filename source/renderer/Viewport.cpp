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

#include "Log.h"
#include "RendererDevice.h"
#include "renderer/Viewport.h"
#include "renderer/Renderer.h"
#include "renderer/Camera.h"

namespace Seed {

Viewport::Viewport()
	: pRenderer(nullptr)
	, pCamera(nullptr)
	, pScene(nullptr)
	, iX(0)
	, iY(0)
	, iWidth(0)
	, iHeight(0)
	, sCameraNameToAttach()
{
}

Viewport::~Viewport()
{
}

void Viewport::SetRenderer(Renderer *renderer)
{
	pRenderer = renderer;
	if (pScene)
		pRenderer->pScene = pScene;
}

Renderer *Viewport::GetRenderer() const
{
	return pRenderer;
}

void Viewport::SetCamera(Camera *camera)
{
	pCamera = camera;
}

Camera *Viewport::GetCamera() const
{
	return pCamera;
}

void Viewport::SetScene(SceneNode *node)
{
	pScene = node;
	if (pRenderer)
		pRenderer->pScene = pScene;
}

SceneNode *Viewport::GetScene() const
{
	return pScene;
}

void Viewport::SetArea(const Rect4u &rect)
{
	iX = rect.x1;
	iY = rect.y1;
	iWidth = rect.x2 - rect.x1;
	iHeight = rect.y2 - rect.y1;
}

void Viewport::SetPosition(u32 x, u32 y)
{
	iX = x;
	iY = y;
}

void Viewport::SetWidth(u32 w)
{
	iWidth = w;
}

void Viewport::SetHeight(u32 h)
{
	iHeight = h;
}

u32 Viewport::GetX() const
{
	return iX;
}

u32 Viewport::GetY() const
{
	return iY;
}

u32 Viewport::GetWidth() const
{
	return iWidth;
}

u32 Viewport::GetHeight() const
{
	return iHeight;
}

void Viewport::Render()
{
	if (pRenderer && pCamera)
	{
<<<<<<< HEAD:source/Viewport.cpp
		pCamera->SetView(Rect4f(static_cast<f32>(iX), static_cast<f32>(iY), static_cast<f32>(iWidth), static_cast<f32>(iHeight)));
		pRendererDevice->SetViewport(iX, iY, iWidth, iHeight);
=======
		pCamera->SetView(Rect4f(f32(iX), f32(iY), f32(iWidth), f32(iHeight)));
		pRendererDevice->SetViewport(f32(iX), f32(iY), f32(iWidth), f32(iHeight));
>>>>>>> develop:source/renderer/Viewport.cpp
		pRenderer->Render(pCamera);
	}
}

bool Viewport::Contains(u32 x, u32 y)
{
	return (x >= iX && x <= iX + iWidth && y >= iY && y <= iY + iHeight);
}

} // namespace
