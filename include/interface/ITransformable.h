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

#ifndef __ITRANSFORMABLE_H__
#define __ITRANSFORMABLE_H__

#include "Defines.h"
#include "Reader.h"
#include "Writer.h"
#include "Rect.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Seed {

/// Transformable interface
class SEED_CORE_API ITransformable
{
	friend class ParticleEmitter; // argh. fix this please.
	friend class Camera;
	SEED_DISABLE_COPY(ITransformable)

	public:
		ITransformable();
		virtual ~ITransformable();

		virtual void SetWidth(f32 w);
		virtual void SetHeight(f32 h);
		virtual void SetDepth(f32 d);

		virtual void SetX(f32 x);
		virtual void SetY(f32 y);
		virtual void SetZ(f32 z);
		virtual void AddX(f32 value);
		virtual void AddY(f32 value);
		virtual void AddZ(f32 value);

		virtual void SetPosition(f32 x, f32 y);
		virtual void AddPosition(f32 x, f32 y);
		virtual void SetPosition(f32 x, f32 y, f32 z);
		virtual void AddPosition(f32 x, f32 y, f32 z);
		virtual void SetPosition(const vec3 &pos);
		virtual void AddPosition(const vec3 &pos);

		virtual void SetPivotX(f32 x);
		virtual void SetPivotY(f32 y);
		virtual void SetPivotZ(f32 z);
		virtual void AddPivotX(f32 value);
		virtual void AddPivotY(f32 value);
		virtual void AddPivotZ(f32 value);

		virtual void SetPivot(f32 x, f32 y);
		virtual void AddPivot(f32 x, f32 y);
		virtual void SetPivot(f32 x, f32 y, f32 z);
		virtual void AddPivot(f32 x, f32 y, f32 z);
		virtual void SetPivot(const vec3 &pos);
		virtual void AddPivot(const vec3 &pos);

		virtual void SetRotation(f32 rot);
		virtual void AddRotation(f32 rot);

		virtual void SetScaleX(f32 scaleX);
		virtual void SetScaleY(f32 scaleY);
		virtual void SetScaleZ(f32 scaleZ);
		virtual void SetScale(f32 scaleXY);
		virtual void SetScale(f32 scaleX, f32 scaleY);
		virtual void SetScale(f32 scaleX, f32 scaleY, f32 scaleZ);
		virtual void SetScale(const vec3 &scale);

		virtual void AddScaleX(f32 scaleX);
		virtual void AddScaleY(f32 scaleY);
		virtual void AddScaleZ(f32 scaleZ);
		virtual void AddScale(f32 scale);
		virtual void AddScale(f32 scaleX, f32 scaleY);
		virtual void AddScale(f32 scaleX, f32 scaleY, f32 scaleZ);
		virtual void AddScale(const vec3 &scale);

		virtual f32 GetWidth() const;
		virtual f32 GetHeight() const;
		virtual f32 GetDepth() const;

		virtual f32 GetX() const;
		virtual f32 GetY() const;
		virtual f32 GetZ() const;
		virtual vec3 GetPosition() const;

		virtual f32 GetPivotX() const;
		virtual f32 GetPivotY() const;
		virtual f32 GetPivotZ() const;
		virtual vec3 GetPivot() const;

		virtual f32 GetRotation() const;

		virtual f32 GetScaleX() const;
		virtual f32 GetScaleY() const;
		virtual f32 GetScaleZ() const;
		virtual vec3 GetScale() const;

		virtual bool ContainsPoint(f32 x, f32 y) const;
		virtual bool ContainsPoint(f32 x, f32 y, f32 z) const;
		virtual bool ContainsPoint(const vec3 &pos) const;

		virtual void UpdateTransform();
		virtual void UpdateBoundingCircle();
		virtual Rect4f GetBoundingBox() const;

		/// Set a parent for this transformable
		/**
		When a Transformable has a parent it will inherit all it's properies.
		If you use a instanced Transformable and delete it, you're responsable
		to set the parent of this object to null or we can crash badly.
		\param Set a Transformable as parent for this object
		 */
		virtual void SetParent(ITransformable *pParent);
		virtual ITransformable *GetParent() const;

		virtual void Reset();
		virtual void Unserialize(Reader &reader);
		virtual void Serialize(Writer &writer);

		virtual bool IsChanged() const;

	protected:
		ITransformable *pParent;
		mat4 mTransform;
		vec3 vPos;
		vec3 vPivot;
		vec3 vTransformedPivot;
		vec3 vScale;
		vec3 vBoundingBox;
		f32 fBoundingCircleRadius;
		f32 fRotation;
		bool bTransformationChanged : 1;
};

/// Transformable ascending predicate
struct SEED_CORE_API ITransformableAscendingPrioritySort
{
	bool operator()(ITransformable * const &left, ITransformable * const &right)
	{
		return (left->GetZ() < right->GetZ());
	}
};

/// Transformable descending predicate
struct SEED_CORE_API ITransformableDescendingPrioritySort
{
	bool operator()(ITransformable * const &left, ITransformable * const &right)
	{
		return (left->GetZ() > right->GetZ());
	}
};

} // namespace

#endif // __ITRANSFORMABLE_H__
