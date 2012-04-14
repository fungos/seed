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

#include "interface/ITransformable.h"
#include "Number.h"

namespace Seed {

ITransformable::ITransformable()
	: pParent(NULL)
    , mTransform()
	, vPos(0.0f, 0.0f, 0.0f)
#if SEED_USE_CENTERED_PIVOT == 1
	, vPivot(0.5f, 0.5f, 0.5f)
#else
	, vPivot(0.0f, 0.0f, 0.0f)
#endif
    , vTransformedPivot(0.0f, 0.0f, 0.0f)
	, vScale(1.0f, 1.0f, 1.0f)
	, vBoundingBox(0.0f, 0.0f, 0.0f)
	, fRotation(0.0f)
	, bTransformationChanged(true)
{
}

ITransformable::~ITransformable()
{
	this->Reset();
}

void ITransformable::Reset()
{
	vPos = Vector3f();
	vPivot = Vector3f();
	vScale = Vector3f();
	vBoundingBox = Vector3f();
	fRotation		= 0.0f;

	pParent			= NULL;

	bTransformationChanged = true;

	//IRenderable::Reset();
}

void ITransformable::SetWidth(f32 w)
{
	if (w == vBoundingBox.getX())
		return;

	vBoundingBox.setX(w);
	bTransformationChanged = true;
}

void ITransformable::SetHeight(f32 h)
{
	if (h == vBoundingBox.getY())
		return;

	vBoundingBox.setY(h);
	bTransformationChanged = true;
}

void ITransformable::SetX(f32 x)
{
	if (x == vPos.getX())
		return;

	vPos.setX(x);
	bTransformationChanged = true;
}

void ITransformable::SetY(f32 y)
{
	if (y == vPos.getY())
		return;

	vPos.setY(y);
	bTransformationChanged = true;
}

void ITransformable::AddX(f32 value)
{
	if (value == 0)
		return;

	vPos.setX(vPos.getX() + value);
	bTransformationChanged = true;
}

void ITransformable::AddY(f32 value)
{
	if (value == 0)
		return;

	vPos.setY(vPos.getY() + value);
	bTransformationChanged = true;
}

void ITransformable::SetPosition(f32 x, f32 y)
{
	if (vPos.getX() == x && vPos.getY() == y)
		return;

	vPos.setX(x);
	vPos.setY(y);
	bTransformationChanged = true;
}

void ITransformable::SetPosition(const Vector3f &pos)
{
	if (VectorEquals(vPos, pos))
		return;

	vPos = pos;
	bTransformationChanged = true;
}

void ITransformable::AddPosition(f32 x, f32 y)
{
	if (0.0f == x && 0.0f == y)
		return;

	vPos.setX(vPos.getX() + x);
	vPos.setY(vPos.getY() + y);
	bTransformationChanged = true;
}

void ITransformable::AddPosition(const Vector3f &pos)
{
	if (0.0f == pos.getX() && 0.0f == pos.getY() && 0.0f == pos.getZ())
		return;

	vPos += pos;
	bTransformationChanged = true;
}

void ITransformable::SetPivotX(f32 x)
{
	if (x == vPivot.getX())
		return;

	vPivot.setX(x);
	bTransformationChanged = true;

	vTransformedPivot = this->GetPivot() - Vector3f(0.5f, 0.5f, 0.5f);
	VectorAgg(vTransformedPivot, vBoundingBox);
}

void ITransformable::SetPivotY(f32 y)
{
	if (y == vPivot.getY())
		return;

	vPivot.setY(y);
	bTransformationChanged = true;

	vTransformedPivot = this->GetPivot() - Vector3f(0.5f, 0.5f, 0.5f);
	VectorAgg(vTransformedPivot, vBoundingBox);
}

void ITransformable::SetPivotZ(f32 z)
{
	if (z == vPivot.getZ())
		return;

	vPivot.setZ(z);
	bTransformationChanged = true;

	vTransformedPivot = this->GetPivot() - Vector3f(0.5f, 0.5f, 0.5f);
	VectorAgg(vTransformedPivot, vBoundingBox);
}

void ITransformable::AddPivotX(f32 value)
{
	if (value == 0)
		return;

	vPivot.setX(vPivot.getX() + value);
	bTransformationChanged = true;

	vTransformedPivot = this->GetPivot() - Vector3f(0.5f, 0.5f, 0.5f);
	VectorAgg(vTransformedPivot, vBoundingBox);
}

void ITransformable::AddPivotY(f32 value)
{
	if (value == 0)
		return;

	vPivot.setY(vPivot.getY() + value);
	bTransformationChanged = true;

	vTransformedPivot = this->GetPivot() - Vector3f(0.5f, 0.5f, 0.5f);
	VectorAgg(vTransformedPivot, vBoundingBox);
}

void ITransformable::AddPivotZ(f32 value)
{
	if (value == 0)
		return;

	vPivot.setZ(vPivot.getZ() + value);
	bTransformationChanged = true;

	vTransformedPivot = this->GetPivot() - Vector3f(0.5f, 0.5f, 0.5f);
	VectorAgg(vTransformedPivot, vBoundingBox);
}

void ITransformable::SetPivot(f32 x, f32 y)
{
	if (vPivot.getX() == x && vPivot.getY() == y)
		return;

	vPivot.setX(x);
	vPivot.setY(y);
	bTransformationChanged = true;

	vTransformedPivot = this->GetPivot() - Vector3f(0.5f, 0.5f, 0.5f);
	VectorAgg(vTransformedPivot, vBoundingBox);
}

void ITransformable::SetPivot(const Vector3f &pos)
{
	if (VectorEquals(vPivot, pos))
		return;

	vPivot = pos;
	bTransformationChanged = true;

	vTransformedPivot = this->GetPivot() - Vector3f(0.5f, 0.5f, 0.5f);
	VectorAgg(vTransformedPivot, vBoundingBox);
}

void ITransformable::AddPivot(f32 x, f32 y)
{
	if (0.0f == x && 0.0f == y)
		return;

	vPivot.setX(vPivot.getX() + x);
	vPivot.setY(vPivot.getY() + y);
	bTransformationChanged = true;

	vTransformedPivot = this->GetPivot() - Vector3f(0.5f, 0.5f, 0.5f);
	VectorAgg(vTransformedPivot, vBoundingBox);
}

void ITransformable::AddPivot(const Vector3f &pos)
{
	if (0.0f == pos.getX() && 0.0f == pos.getY() && 0.0f == pos.getZ())
		return;

	vPivot += pos;
	bTransformationChanged = true;

	vTransformedPivot = this->GetPivot() - Vector3f(0.5f, 0.5f, 0.5f);
	VectorAgg(vTransformedPivot, vBoundingBox);
}

void ITransformable::SetRotation(f32 rot)
{
	if (fRotation == rot)
		return;

	fRotation = rot;

	if (fRotation >= 360)
		fRotation -= 360;

	if (fRotation < 0)
		fRotation += 360;

	bTransformationChanged = true;
}

void ITransformable::AddRotation(f32 rot)
{
	if (rot == 0)
		return;

	fRotation += rot;

	if (fRotation >= 360)
		fRotation -= 360;

	if (fRotation < 0)
		fRotation += 360;

	bTransformationChanged = true;
}

void ITransformable::SetScaleX(f32 scaleX)
{
	if (vScale.getX() == scaleX)
		return;

	vScale.setX(scaleX);
	bTransformationChanged = true;
}

void ITransformable::SetScaleY(f32 scaleY)
{
	if (vScale.getY() == scaleY)
		return;

	vScale.setY(scaleY);
	bTransformationChanged = true;
}

void ITransformable::SetScale(f32 scale)
{
	if (vScale.getX() == scale && vScale.getY() == scale)
		return;

	vScale.setX(scale);
	vScale.setY(scale);
	bTransformationChanged = true;
}

void ITransformable::SetScale(const Vector3f &scale)
{
	if (VectorEquals(vScale, scale))
		return;

	vScale = scale;
	bTransformationChanged = true;
}

void ITransformable::SetScale(f32 scaleX, f32 scaleY)
{
	if (vScale.getX() == scaleX && vScale.getY() == scaleY)
		return;

	vScale.setX(scaleX);
	vScale.setY(scaleY);
	bTransformationChanged = true;
}

void ITransformable::AddScaleX(f32 scaleX)
{
	if (0.0f == scaleX)
		return;

	vScale.setX(vScale.getX() + scaleX);
	bTransformationChanged = true;
}

void ITransformable::AddScaleY(f32 scaleY)
{
	if (0.0f == scaleY)
		return;

	vScale.setY(vScale.getY() + scaleY);
	bTransformationChanged = true;
}

void ITransformable::AddScale(f32 scaleX, f32 scaleY)
{
	if (0.0f == scaleX && 0.0f == scaleY)
		return;

	vScale.setX(vScale.getX() + scaleX);
	vScale.setY(vScale.getY() + scaleY);
	bTransformationChanged = true;
}

void ITransformable::AddScale(f32 scale)
{
	if (0.0f == scale)
		return;

	vScale.setX(vScale.getX() + scale);
	vScale.setY(vScale.getY() + scale);
	bTransformationChanged = true;
}

void ITransformable::AddScale(const Vector3f &scale)
{
	if (0.0f == scale.getX() && 0.0f == scale.getY() && 0.0f == scale.getZ())
		return;

	vScale += scale;
	bTransformationChanged = true;
}

f32 ITransformable::GetRotation() const
{
	f32 r = fRotation;
	if (pParent)
		r += pParent->GetRotation();

	return r;
}

f32 ITransformable::GetScaleX() const
{
	f32 s = vScale.getX();
	if (pParent)
		s *= pParent->GetScaleX();

	return s;
}

f32 ITransformable::GetScaleY() const
{
	f32 s = vScale.getY();
	if (pParent)
		s *= pParent->GetScaleY();

	return s;
}

Vector3f ITransformable::GetScale() const
{
	Vector3f s = vScale;
	if (pParent)
	{
		Vector3f p = pParent->GetScale();
		s.setX(s.getX() * p.getX());
		s.setY(s.getY() * p.getY());
		s.setZ(s.getZ() * p.getZ());
	}

	return s;
}

f32 ITransformable::GetWidth() const
{
	return vBoundingBox.getX() * Number::Abs(this->GetScaleX());
}

f32 ITransformable::GetHeight() const
{
	return vBoundingBox.getY() * Number::Abs(this->GetScaleY());
}

f32 ITransformable::GetX() const
{
	f32 x = vPos.getX();
	if (pParent)
		x += pParent->GetX();

	return x;
}

f32 ITransformable::GetY() const
{
	f32 y = vPos.getY();
	if (pParent)
		y += pParent->GetY();

	return y;
}

Vector3f ITransformable::GetPosition() const
{
	Vector3f p = vPos;
	if (pParent)
		p += pParent->GetPosition();

	return p;
}

f32 ITransformable::GetPivotX() const
{
	f32 x = vPivot.getX();
	if (pParent)
		x += pParent->GetPivotX();

	return x;
}

f32 ITransformable::GetPivotY() const
{
	f32 y = vPivot.getY();
	if (pParent)
		y += pParent->GetPivotY();

	return y;
}

f32 ITransformable::GetPivotZ() const
{
	f32 z = vPivot.getZ();
	if (pParent)
		z += pParent->GetPivotZ();

	return z;
}

Vector3f ITransformable::GetPivot() const
{
	Vector3f p = vPivot;
	if (pParent)
		p += pParent->GetPivot();

	return p;
}

bool ITransformable::ContainsPoint(f32 x, f32 y) const
{
	if (x > (this->GetX() + this->GetWidth()))
	{
		return false;
	}
	else if	(x < this->GetX())
	{
		return false;
	}
	if (y > (this->GetY() + this->GetHeight()))
	{
		return false;
	}
	else if	(y < this->GetY())
	{
		return false;
	}

	return true;
}

bool ITransformable::ContainsPoint(const Vector3f &pos) const
{
	return this->ContainsPoint(pos.getX(), pos.getY());
}

void ITransformable::SetPriority(f32 prio)
{
	vPos.setZ(prio);
}

f32 ITransformable::GetPriority() const
{
	u32 prio = vPos.getZ();

	if (pParent)
		prio += pParent->GetPriority();

	return prio;
}

void ITransformable::SetParent(ITransformable *parent)
{
	pParent = parent;
}

ITransformable *ITransformable::GetParent() const
{
	return pParent;
}

bool ITransformable::IsChanged() const
{
	bool ret = bTransformationChanged;
	if (!ret && pParent)
		ret = pParent->IsChanged();

	return ret;
}

void ITransformable::UpdateTransform()
{
	Vector3f pos = this->GetPosition();

#if SEED_USE_ROTATION_PIVOT == 0
	Matrix4f r = Matrix4f::rotationZ(DegToRad(this->GetRotation()));
	r = appendScale(r, this->GetScale());
	Matrix4f p = Matrix4f::identity();
	p.setTranslation(-vTransformedPivot);
	Matrix4f self = Matrix4f::identity();
	self.setTranslation(pos);
	mTransform = self * (r * p);
#else
	Matrix4f r = Matrix4f(Quaternion::rotationZ(DegToRad(this->GetRotation())), -vTransformedPivot);
	r = appendScale(r, this->GetScale());
	Matrix4f self = Matrix4f::identity();
	self.setTranslation(pos);
	mTransform = self * r;
#endif

	bTransformationChanged = false;
}

void ITransformable::Unserialize(Reader &reader)
{
	vPos.setZ(reader.ReadF32("priority", 0.0f));
	this->SetRotation(reader.ReadF32("rotation", 0.0f));

	if (reader.SelectNode("position"))
	{
		vPos.setX(reader.ReadF32("x", 0.0f));
		vPos.setY(reader.ReadF32("y", 0.0f));
		reader.UnselectNode();
	}

	if (reader.SelectNode("pivot"))
	{
		vPivot.setX(reader.ReadF32("x", 0.0f));
		vPivot.setY(reader.ReadF32("y", 0.0f));
		reader.UnselectNode();
	}

	if (reader.SelectNode("scale"))
	{
		vScale.setX(reader.ReadF32("x", 1.0f));
		vScale.setY(reader.ReadF32("y", 1.0f));
		vScale.setZ(reader.ReadF32("z", 1.0f));
		reader.UnselectNode();
	}
}

void ITransformable::Serialize(Writer &writer)
{
	writer.WriteF32("priority", (s32)vPos.getZ());
	writer.WriteF32("rotation", fRotation);

	writer.OpenNode("position");
		writer.WriteF32("x", vPos.getX());
		writer.WriteF32("y", vPos.getY());
	writer.CloseNode();

	writer.OpenNode("pivot");
		writer.WriteF32("x", vPivot.getX());
		writer.WriteF32("y", vPivot.getY());
	writer.CloseNode();

	writer.OpenNode("scale");
		writer.WriteF32("x", vScale.getX());
		writer.WriteF32("y", vScale.getY());
	writer.CloseNode();
}

} // namespace
