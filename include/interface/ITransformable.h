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

#ifndef __ITransformable_H__
#define __ITransformable_H__

#include "Defines.h"
#include "Vector3.h"

namespace Seed {

/// Transformable interface
class SEED_CORE_API ITransformable
{
	public:
		ITransformable();
		virtual ~ITransformable();

		virtual void SetWidth(f32 w);
		virtual void SetHeight(f32 h);

		virtual void SetX(f32 x);
		virtual void SetY(f32 y);
		virtual void AddX(f32 value);
		virtual void AddY(f32 value);

		virtual void SetPosition(f32 x, f32 y);
		virtual void AddPosition(f32 x, f32 y);
		virtual void SetPosition(const Vector3f &pos);
		virtual void AddPosition(const Vector3f &pos);

		virtual void SetPivotX(f32 x);
		virtual void SetPivotY(f32 y);
		virtual void SetPivotZ(f32 z);
		virtual void AddPivotX(f32 value);
		virtual void AddPivotY(f32 value);
		virtual void AddPivotZ(f32 value);

		virtual void SetPivot(f32 x, f32 y);
		virtual void AddPivot(f32 x, f32 y);
		virtual void SetPivot(const Vector3f &pos);
		virtual void AddPivot(const Vector3f &pos);

		virtual void SetRotation(f32 rot);
		virtual void AddRotation(f32 rot);

		virtual void SetScaleX(f32 scaleX);
		virtual void SetScaleY(f32 scaleY);
		virtual void SetScale(f32 scale);
		virtual void SetScale(f32 scaleX, f32 scaleY);
		virtual void SetScale(const Vector3f &scale);

		virtual void AddScaleX(f32 scaleX);
		virtual void AddScaleY(f32 scaleY);
		virtual void AddScale(f32 scale);
		virtual void AddScale(f32 scaleX, f32 scaleY);
		virtual void AddScale(const Vector3f &scale);

		virtual f32 GetWidth() const;
		virtual f32 GetHeight() const;

		virtual f32 GetX() const;
		virtual f32 GetY() const;
		virtual Vector3f GetPosition() const;

		virtual f32 GetPivotX() const;
		virtual f32 GetPivotY() const;
		virtual f32 GetPivotZ() const;
		virtual Vector3f GetPivot() const;

		virtual f32 GetRotation() const;

		virtual f32 GetScaleX() const;
		virtual f32 GetScaleY() const;
		virtual Vector3f GetScale() const;

		virtual bool ContainsPoint(f32 x, f32 y) const;
		virtual bool ContainsPoint(const Vector3f &pos) const;

		virtual void SetPriority(f32 prio);
		virtual f32 GetPriority() const;

		/// Set a parent for this transformable
		/**
		When a Transformable has a parent it will inherit all it's properies.
		If you use a instanced Transformable and delete it, you're responsable
		to set the parent of this object to NULL or we can crash badly.
		\param Set a Transformable as parent for this object
		 */
		virtual void SetParent(ITransformable *pParent);
		virtual ITransformable *GetParent() const;

		virtual void Reset();

		virtual bool IsChanged() const;

	protected:
		ITransformable *pParent;
		Vector3f vPos;
		Vector3f vPivot;
		Vector3f vScale;
		Vector3f vBoundingBox;
		f32 fRotation;
		bool bTransformationChanged;

	private:
		SEED_DISABLE_COPY(ITransformable);
};

/// Transformable ascending predicate
struct SEED_CORE_API ITransformableAscendingPrioritySort
{
	bool operator()(ITransformable * const &left, ITransformable * const &right)
	{
		return (left->GetPriority() < right->GetPriority());
	}
};

/// Transformable descending predicate
struct SEED_CORE_API ITransformableDescendingPrioritySort
{
	bool operator()(ITransformable * const &left, ITransformable * const &right)
	{
		return (left->GetPriority() > right->GetPriority());
	}
};

} // namespace

#endif // __ITransformable_H__
