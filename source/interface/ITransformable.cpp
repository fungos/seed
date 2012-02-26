/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

#include "interface/ITransformable.h"
#include "Number.h"

namespace Seed {

ITransformable::ITransformable()
	: pParent(NULL)
	, vPos(0.0f, 0.0f, 0.0f)
	, vLocalPos(0.0f, 0.0f, 0.0f)
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
	vLocalPos.x		= 0.0f;
	vLocalPos.y		= 0.0f;
	vLocalPos.z		= 0.0f;
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

void ITransformable::SetLocalX(f32 x)
{
	if (x == vLocalPos.x)
		return;

	vLocalPos.x = x;
	bTransformationChanged = true;
}

void ITransformable::SetLocalY(f32 y)
{
	if (y == vLocalPos.y)
		return;

	vLocalPos.y = y;
	bTransformationChanged = true;
}

void ITransformable::AddLocalX(f32 value)
{
	if (value == 0)
		return;

	vLocalPos.x += value;
	bTransformationChanged = true;
}

void ITransformable::AddLocalY(f32 value)
{
	if (value == 0)
		return;

	vLocalPos.y += value;
	bTransformationChanged = true;
}

void ITransformable::SetLocalPosition(f32 x, f32 y)
{
	if (vLocalPos.x == x && vLocalPos.y == y)
		return;

	vLocalPos.x = x;
	vLocalPos.y = y;
	bTransformationChanged = true;
}

void ITransformable::SetLocalPosition(const Vector3f &pos)
{
	if (vLocalPos == pos)
		return;

	vLocalPos = pos;
	bTransformationChanged = true;
}

void ITransformable::AddLocalPosition(f32 x, f32 y)
{
	if (0.0f == x && 0.0f == y)
		return;

	vLocalPos.x += x;
	vLocalPos.y += y;
	bTransformationChanged = true;
}

void ITransformable::AddLocalPosition(const Vector3f &pos)
{
	if (0.0f == pos.x && 0.0f == pos.y && 0.0f == pos.z)
		return;

	vLocalPos += pos;
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
	f32 r = 0;
	if (pParent)
		r = pParent->GetRotation();

	return r + fRotation;
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
	f32 x = 0;
	if (pParent)
		x = pParent->GetX();

	return x + vPos.x;
}

f32 ITransformable::GetY() const
{
	f32 y = 0;
	if (pParent)
		y = pParent->GetY();

	return y + vPos.y;
}

Vector3f ITransformable::GetPosition() const
{
	Vector3f p = vPos;
	if (pParent)
		p += pParent->GetPosition();

	return p;
}

f32 ITransformable::GetLocalX() const
{
	f32 x = 0;
	if (pParent)
		x = pParent->GetLocalX();

	return x + vLocalPos.x;
}

f32 ITransformable::GetLocalY() const
{
	f32 y = 0;
	if (pParent)
		y = pParent->GetLocalY();

	return y + vLocalPos.y;
}

Vector3f ITransformable::GetLocal() const
{
	Vector3f p = vLocalPos;
	if (pParent)
		p = pParent->GetLocal();

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
