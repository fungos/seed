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

#include "Defines.h"
#include "Camera.h"
#include "Log.h"
#include "Enum.h"
#include "interface/ITexture.h"
#include "Screen.h"
#include <algorithm>

#define TAG "[Camera] "

namespace Seed {

ISceneObject *FactoryCamera()
{
	return New(Camera());
}

Camera::Camera()
	: pTexture(NULL)
	, aMesh()
	, nProjection(Seed::Orthogonal)
	, rViewArea()
{
}

Camera::~Camera()
{
}

void Camera::SetProjection(eProjection type)
{
	nProjection = type;
}

bool Camera::Contains(ITransformable *obj, Matrix4f &worldMatrix)
{
	bool ret = false;
	if (nProjection == Seed::Orthogonal)
		ret = this->IsInView(obj, worldMatrix);
	else
		ret = this->IsInFrustum(obj, worldMatrix);

	return ret;
}

void Camera::SetView(const Rect4f &rect)
{
	rViewArea = rect;
	rBoundingBox = Rect4f(0.0f, 0.0f, static_cast<f32>(pScreen->GetWidth()), static_cast<f32>(pScreen->GetHeight()));
}

void Camera::Update(f32 delta)
{
	UNUSED(delta)
	bTransformationChanged = this->IsChanged();
	if (bTransformationChanged)
	{
		f32  x1, y1, x2, y2, z;
		x2 = vBoundingBox.getX() * 0.5f;
		y2 = vBoundingBox.getY() * 0.5f;
		x1 = -x2;
		y1 = -y2;
		z = vPos.getZ();

		aMesh[0].cVertex = Vector3f(x1, y1, z);
		aMesh[1].cVertex = Vector3f(x2, y1, z);
		aMesh[2].cVertex = Vector3f(x1, y2, z);
		aMesh[3].cVertex = Vector3f(x2, y2, z);

		this->UpdateTransform();

		mInverse = inverse(mTransform);
	}

	if (bColorChanged)
	{
		bColorChanged = false;

		Color p = cColor;
		aMesh[0].cColor = p;
		aMesh[1].cColor = p;
		aMesh[2].cColor = p;
		aMesh[3].cColor = p;
	}

	bTransformationChanged = false;
}

void Camera::Render(const Matrix4f &worldTransform)
{
	UNUSED(worldTransform)

	//#warning IMPL - Camera::Render(const Matrix4f &worldTransform) - Camera representation
//	RendererPacket packet;
//	packet.iSize = iNumVertices;
//	packet.nMeshType = nMeshType;
//	packet.pVertexData = &aMesh;
//	packet.pTexture = pFrameTexture;
//	packet.nBlendMode = eBlendOperation;
//	packet.pTransform = &mTransform;
//	packet.cColor = cColor;
//	packet.iFlags = flags;
//	packet.vPivot = vTransformedPivot;

//	pRendererDevice->UploadData(&packet);
}

bool Camera::SetTexture(ITexture *target)
{
	bool ret = false;
	if (target)
	{
		ret = (target->GetRenderTarget() > 0);
		if (!ret)
			ret = target->EnableRenderTarget(true);
	}

	if (ret)
		pTexture = target;

	return ret;
}

ITexture *Camera::GetTexture() const
{
	return pTexture;
}

bool Camera::IsInView(ITransformable *obj, Matrix4f &worldTransform)
{
	worldTransform = mInverse * obj->mTransform;
	Vector3f op = worldTransform.getTranslation();

	f32 ox = op.getX();
	f32 oy = op.getY();
	Rect4f box(ox, oy, obj->GetWidth(), obj->GetHeight());
	return rBoundingBox.Intersect(ox, oy, box.CircleRadius());
}

bool Camera::IsInFrustum(ITransformable *obj, Matrix4f &worldTransform)
{
	UNUSED(obj)
	UNUSED(worldTransform)

	//#warning IMPL - Camera::IsInFrustum(ITransformable *obj, Matrix4f &worldTransform)

	return true;
}

const String Camera::GetClassName() const
{
	return "Camera";
}

int Camera::GetObjectType() const
{
	return Seed::TypeCamera;
}

bool Camera::Unload()
{
	return true;
}

bool Camera::Load(Reader &reader, ResourceManager *res)
{
	UNUSED(res);
	bool ret = false;

	if (this->Unload())
	{
		sName = reader.ReadString("sName", "MainCamera");

		String proj = reader.ReadString("sProjection", "Orthogonal");
		std::transform(proj.begin(), proj.end(), proj.begin(), ::tolower);
		if (proj == "perspective")
			nProjection = Seed::Perspective;
		else
			nProjection = Seed::Orthogonal;

		ITransformable::Unserialize(reader);
		IRenderable::Unserialize(reader);

		ret = true;
	}

	return ret;
}

bool Camera::Write(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("sType", this->GetClassName().c_str());
		writer.WriteString("sName", sName.c_str());
		writer.WriteString("sProjection", nProjection == Seed::Orthogonal ? "Orthogonal" : "Perspective");

		ITransformable::Serialize(writer);
		IRenderable::Serialize(writer);
	writer.CloseNode();

	return true;
}

} // namespace

