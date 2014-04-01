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
#include <glm/glm.hpp>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#if SEED_USE_CENTERED_PIVOT == 1
	#define SEED_PIVOT_VALUE 0.5f
#else
	#define SEED_PIVOT_VALUE 0.0f
#endif

namespace Seed {

ITransformable::ITransformable()
	: pParent(nullptr)
	, mTransform()
	, vPos(0.0f, 0.0f, 0.0f)
	, vPivot(SEED_PIVOT_VALUE, SEED_PIVOT_VALUE, SEED_PIVOT_VALUE)
	, vTransformedPivot(0.0f, 0.0f, 0.0f)
	, vScale(1.0f, 1.0f, 1.0f)
	, vBoundingBox(0.0f, 0.0f, 1.0f)
	, fBoundingCircleRadius(0.0f)
	, fRotation(0.0f)
	, bTransformationChanged(true)
{
	memset(&mTransform, 0, sizeof(mTransform));
	memset(&vPos, 0, sizeof(vPos));
	memset(&vPivot, 0, sizeof(vPivot));
	memset(&vTransformedPivot, 0, sizeof(vTransformedPivot));
	memset(&vScale, 0, sizeof(vScale));
	memset(&vBoundingBox, 0, sizeof(vBoundingBox));

	vPivot.x = vPivot.y = vPivot.z = (SEED_PIVOT_VALUE);
	vScale.x = vScale.y = vScale.z = 1.0f;
	vBoundingBox.z = 1.0f;
}

ITransformable::~ITransformable()
{
	this->Reset();
}

void ITransformable::Reset()
{
	vPos = vec3();
	vPivot = vec3();
	vScale = vec3();
	vBoundingBox = vec3();
	fRotation = 0.0f;
	pParent = nullptr;
	bTransformationChanged = true;
}

void ITransformable::SetWidth(f32 w)
{
	if (w == vBoundingBox.x)
		return;

	vBoundingBox.x = w;
	this->UpdateBoundingCircle();

	bTransformationChanged = true;
}

void ITransformable::SetHeight(f32 h)
{
	if (h == vBoundingBox.y)
		return;

	vBoundingBox.y = h;
	this->UpdateBoundingCircle();

	bTransformationChanged = true;
}

void ITransformable::SetDepth(f32 d)
{
	if (d == vBoundingBox.z)
		return;

	vBoundingBox.z = d;

	bTransformationChanged = true;
}

void ITransformable::SetX(f32 x)
{
	if (x == vPos.x)
		return;

	vPos.x = x;
	bTransformationChanged = true;
}

void ITransformable::SetY(f32 y)
{
	if (y == vPos.y)
		return;

	vPos.y = y;
	bTransformationChanged = true;
}

void ITransformable::SetZ(f32 z)
{
	if (z == vPos.z)
		return;

	vPos.z = z;
	bTransformationChanged = true;
}

void ITransformable::AddX(f32 value)
{
	if (value == 0)
		return;

	vPos.x = vPos.x + value;
	bTransformationChanged = true;
}

void ITransformable::AddY(f32 value)
{
	if (value == 0)
		return;

	vPos.y = vPos.y + value;
	bTransformationChanged = true;
}

void ITransformable::AddZ(f32 value)
{
	if (value == 0)
		return;

	vPos.z = vPos.z + value;
	bTransformationChanged = true;
}

void ITransformable::SetPosition(f32 x, f32 y)
{
	if (vPos.x == x && vPos.y == y)
		return;

	vPos.x = x;
	vPos.y = y;
	bTransformationChanged = true;
}

void ITransformable::SetPosition(f32 x, f32 y, f32 z)
{
	if (vPos.x == x && vPos.y == y && vPos.z == z)
		return;

	vPos.x = x;
	vPos.y = y;
	vPos.z = z;
	bTransformationChanged = true;
}

void ITransformable::SetPosition(const vec3 &pos)
{
	if (vPos == pos)
		return;

	vPos = pos;
	bTransformationChanged = true;
}

void ITransformable::AddPosition(f32 x, f32 y)
{
	if (0.0f == x && 0.0f == y)
		return;

	vPos.x = vPos.x + x;
	vPos.y = vPos.y + y;
	bTransformationChanged = true;
}

void ITransformable::AddPosition(f32 x, f32 y, f32 z)
{
	if (0.0f == x && 0.0f == y && 0.0f == z)
		return;

	vPos.x = vPos.x + x;
	vPos.y = vPos.y + y;
	vPos.z = vPos.z + z;
	bTransformationChanged = true;
}

void ITransformable::AddPosition(const vec3 &pos)
{
	if (0.0f == pos.x && 0.0f == pos.y && 0.0f == pos.z)
		return;

	vPos += pos;
	bTransformationChanged = true;
}

void ITransformable::SetPivotX(f32 x)
{
	if (x == vPivot.x)
		return;

	vPivot.x = x;
	bTransformationChanged = true;

	vTransformedPivot = vPivot - vec3{0.5f, 0.5f, 0.5f};
	vTransformedPivot *= vBoundingBox;
}

void ITransformable::SetPivotY(f32 y)
{
	if (y == vPivot.y)
		return;

	vPivot.y = y;
	bTransformationChanged = true;

	vTransformedPivot = vPivot - vec3{0.5f, 0.5f, 0.5f};
	vTransformedPivot *= vBoundingBox;
}

void ITransformable::SetPivotZ(f32 z)
{
	if (z == vPivot.z)
		return;

	vPivot.z = z;
	bTransformationChanged = true;

	vTransformedPivot = vPivot - vec3{0.5f, 0.5f, 0.5f};
	vTransformedPivot *= vBoundingBox;
}

void ITransformable::AddPivotX(f32 value)
{
	if (value == 0)
		return;

	vPivot.x = vPivot.x + value;
	bTransformationChanged = true;

	vTransformedPivot = vPivot - vec3{0.5f, 0.5f, 0.5f};
	vTransformedPivot *= vBoundingBox;
}

void ITransformable::AddPivotY(f32 value)
{
	if (value == 0)
		return;

	vPivot.y = vPivot.y + value;
	bTransformationChanged = true;

	vTransformedPivot = vPivot - vec3{0.5f, 0.5f, 0.5f};
	vTransformedPivot *= vBoundingBox;
}

void ITransformable::AddPivotZ(f32 value)
{
	if (value == 0)
		return;

	vPivot.z = vPivot.z + value;
	bTransformationChanged = true;

	vTransformedPivot = vPivot - vec3{0.5f, 0.5f, 0.5f};
	vTransformedPivot *= vBoundingBox;
}

void ITransformable::SetPivot(f32 x, f32 y)
{
	if (vPivot.x == x && vPivot.y == y)
		return;

	vPivot.x = x;
	vPivot.y = y;
	bTransformationChanged = true;

	vTransformedPivot = vPivot - vec3{0.5f, 0.5f, 0.5f};
	vTransformedPivot *= vBoundingBox;
}

void ITransformable::SetPivot(f32 x, f32 y, f32 z)
{
	if (vPivot.x == x && vPivot.y == y && vPivot.z == z)
		return;

	vPivot.x = x;
	vPivot.y = y;
	vPivot.z = z;
	bTransformationChanged = true;

	vTransformedPivot = vPivot - vec3{0.5f, 0.5f, 0.5f};
	vTransformedPivot *= vBoundingBox;
}

void ITransformable::SetPivot(const vec3 &pos)
{
	if (vPivot == pos)
		return;

	vPivot = pos;
	bTransformationChanged = true;

	vTransformedPivot = vPivot - vec3{0.5f, 0.5f, 0.5f};
	vTransformedPivot *= vBoundingBox;
}

void ITransformable::AddPivot(f32 x, f32 y)
{
	if (0.0f == x && 0.0f == y)
		return;

	vPivot.x = vPivot.x + x;
	vPivot.y = vPivot.y + y;
	bTransformationChanged = true;

	vTransformedPivot = vPivot - vec3{0.5f, 0.5f, 0.5f};
	vTransformedPivot *= vBoundingBox;
}

void ITransformable::AddPivot(f32 x, f32 y, f32 z)
{
	if (0.0f == x && 0.0f == y && 0.0f == z)
		return;

	vPivot.x = vPivot.x + x;
	vPivot.y = vPivot.y + y;
	vPivot.y = vPivot.z + z;
	bTransformationChanged = true;

	vTransformedPivot = vPivot - vec3{0.5f, 0.5f, 0.5f};
	vTransformedPivot *= vBoundingBox;
}

void ITransformable::AddPivot(const vec3 &pos)
{
	if (0.0f == pos.x && 0.0f == pos.y && 0.0f == pos.z)
		return;

	vPivot += pos;
	bTransformationChanged = true;

	vTransformedPivot = vPivot - vec3{0.5f, 0.5f, 0.5f};
	vTransformedPivot *= vBoundingBox;
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
	if (vScale.x == scaleX)
		return;

	vScale.x = scaleX;
	bTransformationChanged = true;
}

void ITransformable::SetScaleY(f32 scaleY)
{
	if (vScale.y == scaleY)
		return;

	vScale.y = scaleY;
	bTransformationChanged = true;
}

void ITransformable::SetScaleZ(f32 scaleZ)
{
	if (vScale.z == scaleZ)
		return;

	vScale.z = scaleZ;
	bTransformationChanged = true;
}

void ITransformable::SetScale(f32 scaleXY)
{
	if (vScale.x == scaleXY && vScale.y == scaleXY)
		return;

	vScale.x = scaleXY;
	vScale.y = scaleXY;

	bTransformationChanged = true;
}

void ITransformable::SetScale(const vec3 &scale)
{
	if (vScale == scale)
		return;

	vScale = scale;
	bTransformationChanged = true;
}

void ITransformable::SetScale(f32 scaleX, f32 scaleY)
{
	if (vScale.x == scaleX && vScale.y == scaleY)
		return;

	vScale.x = scaleX;
	vScale.y = scaleY;
	bTransformationChanged = true;
}

void ITransformable::SetScale(f32 scaleX, f32 scaleY, f32 scaleZ)
{
	if (vScale.x == scaleX && vScale.y == scaleY && vScale.z == scaleZ)
		return;

	vScale.x = scaleX;
	vScale.y = scaleY;
	vScale.z = scaleZ;
	bTransformationChanged = true;
}

void ITransformable::AddScaleX(f32 scaleX)
{
	if (0.0f == scaleX)
		return;

	vScale.x = vScale.x + scaleX;
	bTransformationChanged = true;
}

void ITransformable::AddScaleY(f32 scaleY)
{
	if (0.0f == scaleY)
		return;

	vScale.y = vScale.y + scaleY;
	bTransformationChanged = true;
}

void ITransformable::AddScaleZ(f32 scaleZ)
{
	if (0.0f == scaleZ)
		return;

	vScale.z = vScale.z + scaleZ;
	bTransformationChanged = true;
}

void ITransformable::AddScale(f32 scaleX, f32 scaleY)
{
	if (0.0f == scaleX && 0.0f == scaleY)
		return;

	vScale.x = vScale.x + scaleX;
	vScale.y = vScale.y + scaleY;
	bTransformationChanged = true;
}

void ITransformable::AddScale(f32 scaleX, f32 scaleY, f32 scaleZ)
{
	if (0.0f == scaleX && 0.0f == scaleY && 0.0f == scaleZ)
		return;

	vScale.x = vScale.x + scaleX;
	vScale.y = vScale.y + scaleY;
	vScale.z = vScale.z + scaleZ;
	bTransformationChanged = true;
}

void ITransformable::AddScale(f32 scale)
{
	if (0.0f == scale)
		return;

	vScale.x = vScale.x + scale;
	vScale.y = vScale.y + scale;
	bTransformationChanged = true;
}

void ITransformable::AddScale(const vec3 &scale)
{
	if (0.0f == scale.x && 0.0f == scale.y && 0.0f == scale.z)
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
	f32 s = vScale.x;
	if (pParent)
		s *= pParent->GetScaleX();

	return s;
}

f32 ITransformable::GetScaleY() const
{
	f32 s = vScale.y;
	if (pParent)
		s *= pParent->GetScaleY();

	return s;
}

f32 ITransformable::GetScaleZ() const
{
	f32 s = vScale.z;
	if (pParent)
		s *= pParent->GetScaleZ();

	return s;
}

vec3 ITransformable::GetScale() const
{
	vec3 s = vScale;
	if (pParent)
	{
		vec3 p = pParent->GetScale();
		s.x = s.x * p.x;
		s.y = s.y * p.y;
		s.z = s.z * p.z;
	}

	return s;
}

f32 ITransformable::GetWidth() const
{
	return vBoundingBox.x * Number::Abs(this->GetScaleX());
}

f32 ITransformable::GetHeight() const
{
	return vBoundingBox.y * Number::Abs(this->GetScaleY());
}

f32 ITransformable::GetDepth() const
{
	return vBoundingBox.z * Number::Abs(this->GetScaleZ());
}

f32 ITransformable::GetX() const
{
	f32 x = vPos.x;
	if (pParent)
		x += pParent->GetX();

	return x;
}

f32 ITransformable::GetY() const
{
	f32 y = vPos.y;
	if (pParent)
		y += pParent->GetY();

	return y;
}

f32 ITransformable::GetZ() const
{
	f32 prio = vPos.z;

	if (pParent)
		prio += pParent->GetZ();

	return prio;
}

vec3 ITransformable::GetPosition() const
{
	vec3 p = vPos;
	if (pParent)
		p += pParent->GetPosition();

	return p;
}

f32 ITransformable::GetPivotX() const
{
	f32 x = vPivot.x;
	if (pParent)
		x += pParent->GetPivotX();

	return x;
}

f32 ITransformable::GetPivotY() const
{
	f32 y = vPivot.y;
	if (pParent)
		y += pParent->GetPivotY();

	return y;
}

f32 ITransformable::GetPivotZ() const
{
	f32 z = vPivot.z;
	if (pParent)
		z += pParent->GetPivotZ();

	return z;
}

vec3 ITransformable::GetPivot() const
{
	vec3 p = vPivot;
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

bool ITransformable::ContainsPoint(f32 x, f32 y, f32 z) const
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
	if (z > (this->GetZ() + this->GetDepth()))
	{
		return false;
	}
	else if	(z < this->GetZ())
	{
		return false;
	}

	return true;
}

bool ITransformable::ContainsPoint(const vec3 &pos) const
{
	return this->ContainsPoint(pos.x, pos.y); // Z?
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
	if (bTransformationChanged)
	{
		vec3 pos = this->GetPosition();

#if SEED_USE_ROTATION_PIVOT == 0
		mat4 p = glm::translate(mat4(1.0f), -vTransformedPivot);
		mat4 s = glm::translate(mat4(1.0f), pos);
		mat4 r = glm::scale(glm::rotate(mat4(1.0f), glm::radians(this->GetRotation()), vec3(0.0f, 0.0f, 1.0f)), this->GetScale());

		mTransform = s * (r * p);
#else
		Matrix4f r = Matrix4f(Quaternion::rotationZ(DegToRad(this->GetRotation())), -vTransformedPivot);
		r = appendScale(r, this->GetScale());
		Matrix4f self = Matrix4f::identity();
		self.setTranslation(pos);
		mTransform = self * r;
#endif
/*

#if SEED_USE_ROTATION_PIVOT == 0
		Matrix4f r = Matrix4f::rotationZ(glm::radians(this->GetRotation()));
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
*/
		bTransformationChanged = false;
	}
}

void ITransformable::UpdateBoundingCircle()
{
	fBoundingCircleRadius = static_cast<f32>(sqrt(vBoundingBox.x * vBoundingBox.x + vBoundingBox.y * vBoundingBox.y) / 2.0f);
}

Rect4f ITransformable::GetBoundingBox() const
{
	return Rect4f(this->GetX(), this->GetY(), vBoundingBox.x, vBoundingBox.y);
}

void ITransformable::Unserialize(Reader &reader)
{
	this->SetRotation(reader.ReadF32("fRotation", 0.0f));

	if (reader.SelectNode("cPosition"))
	{
		vPos.x = reader.ReadF32("x", vPos.x);
		vPos.y = reader.ReadF32("y", vPos.y);
		vPos.z = reader.ReadF32("z", vPos.z);
		reader.UnselectNode();
	}

	if (reader.SelectNode("cPivot"))
	{
		vPivot.x = reader.ReadF32("x", vPivot.x);
		vPivot.y = reader.ReadF32("y", vPivot.y);
		vPivot.z = reader.ReadF32("z", vPivot.z);
		reader.UnselectNode();
	}

	if (reader.SelectNode("cScale"))
	{
		vScale.x = reader.ReadF32("x", vScale.x);
		vScale.y = reader.ReadF32("y", vScale.y);
		vScale.z = reader.ReadF32("z", vScale.z);
		reader.UnselectNode();
	}
}

void ITransformable::Serialize(Writer &writer)
{
	writer.WriteF32("fRotation", fRotation);

	writer.OpenNode("cPosition");
		writer.WriteF32("x", vPos.x);
		writer.WriteF32("y", vPos.y);
		writer.WriteF32("z", vPos.z);
	writer.CloseNode();

	writer.OpenNode("cPivot");
		writer.WriteF32("x", vPivot.x);
		writer.WriteF32("y", vPivot.y);
		writer.WriteF32("z", vPivot.z);
	writer.CloseNode();

	writer.OpenNode("cScale");
		writer.WriteF32("x", vScale.x);
		writer.WriteF32("y", vScale.y);
		writer.WriteF32("z", vScale.z);
	writer.CloseNode();
}

} // namespace
