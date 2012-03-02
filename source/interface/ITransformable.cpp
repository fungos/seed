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
	, vPos(0.0f, 0.0f, 0.0f)
	, vPivot(0.0f, 0.0f, 0.0f)
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
	vPos.x			= 0.0f;
	vPos.y			= 0.0f;
	vPos.z			= 0.0f;
	vPivot.x		= 0.0f;
	vPivot.y		= 0.0f;
	vPivot.z		= 0.0f;
	vScale.x		= 1.0f;
	vScale.y		= 1.0f;
	vScale.z		= 1.0f;
	vBoundingBox.x  = 0.0f;
	vBoundingBox.y  = 0.0f;
	vBoundingBox.z  = 0.0f;
	fRotation		= 0.0f;

	pParent			= NULL;

	bTransformationChanged = true;

	//IRenderable::Reset();
}

void ITransformable::SetWidth(f32 w)
{
	if (w == vBoundingBox.x)
		return;

	vBoundingBox.x = w;
	bTransformationChanged = true;
}

void ITransformable::SetHeight(f32 h)
{
	if (h == vBoundingBox.y)
		return;

	vBoundingBox.y = h;
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

void ITransformable::AddX(f32 value)
{
	if (value == 0)
		return;

	vPos.x += value;
	bTransformationChanged = true;
}

void ITransformable::AddY(f32 value)
{
	if (value == 0)
		return;

	vPos.y += value;
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

void ITransformable::SetPosition(const Vector3f &pos)
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

	vPos.x += x;
	vPos.y += y;
	bTransformationChanged = true;
}

void ITransformable::AddPosition(const Vector3f &pos)
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
}

void ITransformable::SetPivotY(f32 y)
{
	if (y == vPivot.y)
		return;

	vPivot.y = y;
	bTransformationChanged = true;
}

void ITransformable::SetPivotZ(f32 z)
{
	if (z == vPivot.z)
		return;

	vPivot.z = z;
	bTransformationChanged = true;
}

void ITransformable::AddPivotX(f32 value)
{
	if (value == 0)
		return;

	vPivot.x += value;
	bTransformationChanged = true;
}

void ITransformable::AddPivotY(f32 value)
{
	if (value == 0)
		return;

	vPivot.y += value;
	bTransformationChanged = true;
}

void ITransformable::AddPivotZ(f32 value)
{
	if (value == 0)
		return;

	vPivot.z += value;
	bTransformationChanged = true;
}

void ITransformable::SetPivot(f32 x, f32 y)
{
	if (vPivot.x == x && vPivot.y == y)
		return;

	vPivot.x = x;
	vPivot.y = y;
	bTransformationChanged = true;
}

void ITransformable::SetPivot(const Vector3f &pos)
{
	if (vPivot == pos)
		return;

	vPivot = pos;
	bTransformationChanged = true;
}

void ITransformable::AddPivot(f32 x, f32 y)
{
	if (0.0f == x && 0.0f == y)
		return;

	vPivot.x += x;
	vPivot.y += y;
	bTransformationChanged = true;
}

void ITransformable::AddPivot(const Vector3f &pos)
{
	if (0.0f == pos.x && 0.0f == pos.y && 0.0f == pos.z)
		return;

	vPivot += pos;
	bTransformationChanged = true;
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

void ITransformable::SetScale(f32 scale)
{
	if (vScale.x == scale && vScale.y == scale)
		return;

	vScale.x = scale;
	vScale.y = scale;
	bTransformationChanged = true;
}

void ITransformable::SetScale(const Vector3f &scale)
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

void ITransformable::AddScaleX(f32 scaleX)
{
	if (0.0f == scaleX)
		return;

	vScale.x += scaleX;
	bTransformationChanged = true;
}

void ITransformable::AddScaleY(f32 scaleY)
{
	if (0.0f == scaleY)
		return;

	vScale.y += scaleY;
	bTransformationChanged = true;
}

void ITransformable::AddScale(f32 scaleX, f32 scaleY)
{
	if (0.0f == scaleX && 0.0f == scaleY)
		return;

	vScale.x += scaleX;
	vScale.y += scaleY;
	bTransformationChanged = true;
}

void ITransformable::AddScale(f32 scale)
{
	if (0.0f == scale)
		return;

	vScale.x += scale;
	vScale.y += scale;
	bTransformationChanged = true;
}

void ITransformable::AddScale(const Vector3f &scale)
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
	f32 s = 1.0f;
	if (pParent)
		s = pParent->GetScaleX();

	return s * vScale.x;
}

f32 ITransformable::GetScaleY() const
{
	f32 s = 1.0f;
	if (pParent)
		s = pParent->GetScaleY();

	return s * vScale.y;
}

f32 ITransformable::GetWidth() const
{
	return vBoundingBox.x * Number::Abs(this->GetScaleX());
}

f32 ITransformable::GetHeight() const
{
	return vBoundingBox.y * Number::Abs(this->GetScaleY());
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

Vector3f ITransformable::GetPosition() const
{
	Vector3f p = vPos;
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

Vector3f ITransformable::GetPivot() const
{
	Vector3f p = vPivot;
	if (pParent)
		p = pParent->GetPivot();

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
	return this->ContainsPoint(pos.x, pos.y);
}

void ITransformable::SetPriority(f32 prio)
{
	vPos.z = prio;
}

f32 ITransformable::GetPriority() const
{
	u32 prio = vPos.z;

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

} // namespace
