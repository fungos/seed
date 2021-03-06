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

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "interface/IUpdatable.h"
#include "SceneNode.h"

namespace Seed {

class ISceneObject;
class ISceneNode;
class ITexture;
class Camera;
class Presentation;

struct VisibleObject
{
	mat4 mWorldTransform;
	ISceneObject *pObj;

	VisibleObject()
		: mWorldTransform()
		, pObj(nullptr)
	{}
};

/// Renderer
class SEED_CORE_API Renderer : public IUpdatable, public IObject
{
	SEED_DECLARE_CONTAINER(Vector, SceneNode)
	SEED_DISABLE_COPY(Renderer)
	SEED_DECLARE_RTTI(Renderer, IObject)

	typedef Vector<VisibleObject> VisibleVector;
	typedef VisibleVector::iterator VisibleVectorIterator;
	typedef VisibleVector::const_iterator ConstVisibleVectorIterator;

	typedef Vector<ISceneObject *> RenderableVector;
	typedef RenderableVector::iterator RenderableVectorIterator;
	typedef RenderableVector::const_iterator ConstRenderableVectorIterator;

	friend class Presentation;
	friend class Viewport;
	public:
		Renderer();
		virtual ~Renderer();
		virtual void Render(Camera *camera);

		virtual void Begin() const;
		virtual void End() const;

		Renderer *Clone() const;

		void Setup();
		void Teardown();
		void Disable();
		void Enable();
		bool IsEnabled() const;

		// IUpdatable
		virtual bool Update(Seconds delta) override;

	private:
		void RenderObjects(const VisibleVector &vec) const;
		void PushChildNodes(SceneNode *, SceneNodeVector &vec);

		void Sort(VisibleVector &vec);
		void Culler(Camera *camera);

	protected:
		SceneNode *pScene;
		String sPrefabToLoad;
		RenderableVector vRenderables;
		VisibleVector vVisibleRenderables;
		bool bEnabled : 1;
};

inline void Renderer::Setup()
{}

inline void Renderer::Teardown()
{}

inline void Renderer::Disable()
{
	bEnabled = false;
}

inline void Renderer::Enable()
{
	bEnabled = true;
}

inline bool Renderer::IsEnabled() const
{
	return bEnabled;
}


} // namespace

#endif // __RENDERER_H__
