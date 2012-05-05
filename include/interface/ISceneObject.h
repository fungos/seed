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

#ifndef __ISCENE_OBJECT_H__
#define __ISCENE_OBJECT_H__

#include "interface/ITransformable.h"
#include "interface/IRenderable.h"
#include "interface/IDataObject.h"

namespace Seed {

/// Scene object interface
class SEED_CORE_API ISceneObject : public ITransformable, public IRenderable, public IDataObject
{
	public:
		String sName;

		ISceneObject();
		virtual ~ISceneObject();

		virtual bool IsNode() const;

		// IRenderable
		virtual void Update(f32 delta) = 0;
		virtual void Render() = 0;

		// IObject
//		virtual const String GetObjectName() const;
//		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(ISceneObject);
};

/// Scene object ascending predicate
struct SEED_CORE_API ISceneObjectAscendingPrioritySort
{
	bool operator()(ISceneObject * const &left, ISceneObject * const &right)
	{
		return (left->GetPriority() < right->GetPriority());
	}
};

/// Scene object descending predicate
struct SEED_CORE_API ISceneObjectDescendingPrioritySort
{
	bool operator()(ISceneObject * const &left, ISceneObject * const &right)
	{
		return (left->GetPriority() > right->GetPriority());
	}
};


} // namespace

#endif // __ISCENE_OBJECT_H__
