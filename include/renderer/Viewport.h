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

#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include "Defines.h"
#include "Rect.h"
#include "interface/IObject.h"

namespace Seed {

class Renderer;
class Camera;
class Presentation;
class SceneNode;

/// Viewport Interface
/**
Interface for working with viewports.
*/
class SEED_CORE_API Viewport : public IObject
{
	SEED_DISABLE_COPY(Viewport)
	SEED_DECLARE_RTTI(Viewport, IObject)

	friend class Presentation;
	public:
		Viewport();
		virtual ~Viewport();

		void Render();

		void SetRenderer(Renderer *renderer);
		Renderer *GetRenderer() const;

		void SetCamera(Camera *camera);
		Camera *GetCamera() const;

		void SetScene(SceneNode *node);
		SceneNode *GetScene() const;

		void SetArea(const Rect4u &rect);
		void SetPosition(u32 x, u32 y);
		void SetWidth(u32 w);
		void SetHeight(u32 h);

		u32 GetX() const;
		u32 GetY() const;
		u32 GetWidth() const;
		u32 GetHeight() const;

		bool Contains(u32 x, u32 y);

	protected:
		Renderer	*pRenderer;
		Camera		*pCamera;
		SceneNode	*pScene;

		u32		iX;
		u32		iY;
		u32		iWidth;
		u32		iHeight;

		String	sCameraNameToAttach;
		String	sSceneToAttach;
};

} // namespace


#endif // __VIEWPORT_H__
