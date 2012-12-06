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

#ifndef __SCENE_NODE_H__
#define __SCENE_NODE_H__

#include "interface/ISceneObject.h"
#include "Container.h"

namespace Seed {

ISceneObject *FactorySceneNode();

DECLARE_CONTAINER_TYPE(Vector, ISceneObject)

/// Scene Node
class SEED_CORE_API SceneNode : public ISceneObject
{
	friend class Renderer;
	public:
		SceneNode();
		virtual ~SceneNode();

		virtual bool IsNode() const;

		// IRenderable
		virtual void Update(f32 dt);
		virtual void Render(const Matrix4f &worldTransform);

		virtual void Add(ISceneObject *obj);
		virtual void Remove(ISceneObject *obj);
		virtual u32 Size() const;
		virtual ISceneObject *GetChildAt(u32 i);
		virtual ISceneObject *GetChildByName(String name);

		// IDataObject
		virtual bool Load(Reader &reader, ResourceManager *res = pResourceManager);
		virtual bool Write(Writer &writer);

		/*! Will unload all children objects, so if you want to keep some object loaded,
		 * remove it from the scene before calling a parent's Unload.
		 */
		virtual bool Unload();

		/*! Reset will not unload any children, it is just to clear the current node children
		 * as if you're removing one by one.
		 */
		virtual void Reset();

		// IObject
		virtual const String GetClassName() const;
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(SceneNode);

	protected:
		ISceneObjectVector vChild;
};

} // namespace

#endif // __SCENE_NODE_H__
