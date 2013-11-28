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
#include "Memory.h"
#include <algorithm>

#define TAG "[Camera] "

namespace Seed {

ISceneObject *FactoryCamera()
{
	return sdNew(Camera);
}

Camera::Camera()
	: pTexture(nullptr)
	, aMesh()
	, nProjection(eProjection::Orthogonal)
	, rViewArea()
{
}

Camera::~Camera()
{
	this->Unload();
}

void Camera::SetProjection(eProjection type)
{
	nProjection = type;
}

bool Camera::Contains(ITransformable *obj, Matrix4f &worldMatrix)
{
	auto ret = false;
	if (nProjection == eProjection::Orthogonal)
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

void Camera::Update(Seconds dt)
{
	UNUSED(dt)
	bTransformationChanged = this->IsChanged();
	if (bTransformationChanged)
	{
		auto x2 = vBoundingBox.getX() * 0.5f;
		auto y2 = vBoundingBox.getY() * 0.5f;
		auto x1 = -x2;
		auto y1 = -y2;
		auto z = vPos.getZ();

		aMesh[0].cVertex = Vector3f{x1, y1, z};
		aMesh[1].cVertex = Vector3f{x2, y1, z};
		aMesh[2].cVertex = Vector3f{x1, y2, z};
		aMesh[3].cVertex = Vector3f{x2, y2, z};

		this->UpdateTransform();

		mInverse = inverse(mTransform);
	}

	if (bColorChanged)
	{
		bColorChanged = false;

		auto p = cColor;
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

	WARNING(IMPL - Camera::Render(const Matrix4f &worldTransform) - Camera representation)
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

// TODO: TEST
bool Camera::SetTexture(ITexture *target)
{
	if (pTexture)
		sdRelease(pTexture);

	pTexture = target;

	if (!target)
		return false;

	// if it is not already a render target, try to enable it as one, otherwise abort here
	if (target->GetRenderTarget() == 0 && !target->EnableRenderTarget(true))
		return false;

	sdAcquire(target);
	pTexture = target;

	return true;
}

ITexture *Camera::GetTexture() const
{
	return pTexture;
}

bool Camera::IsInView(ITransformable *obj, Matrix4f &worldTransform)
{
	SEED_ASSERT(obj);
	worldTransform = mInverse * obj->mTransform;
	auto op = worldTransform.getTranslation();
	auto ox = op.getX();
	auto oy = op.getY();
	auto box = Rect4f{ox, oy, obj->GetWidth(), obj->GetHeight()};
	return rBoundingBox.Intersect(ox, oy, box.CircleRadius());
}

bool Camera::IsInFrustum(ITransformable *obj, Matrix4f &worldTransform)
{
	UNUSED(obj)
	UNUSED(worldTransform)
	WARNING(IMPL - Camera::IsInFrustum(ITransformable *obj, Matrix4f &worldTransform))

	return true;
}

bool Camera::Unload()
{
	sdRelease(pTexture);

	sName = this->GetTypeName();
	nProjection = eProjection::Orthogonal;

	return true;
}

Camera *Camera::Clone() const
{
	auto obj = sdNew(Camera);
	obj->GenerateCloneName(sName);

	obj->pTexture = nullptr; // we cannot share a render target..?
	obj->nProjection = nProjection;
	obj->rViewArea = rViewArea;
	obj->rBoundingBox = rBoundingBox;
	obj->mInverse = mInverse;

	memcpy(&obj->aMesh, &aMesh, sizeof(aMesh));

	// ISceneObject
	obj->bMarkForDeletion = true;

	// ITransformable
	obj->pParent = pParent;
	obj->mTransform = mTransform;
	obj->vPos = vPos;
	obj->vPivot = vPivot;
	obj->vTransformedPivot = vTransformedPivot;
	obj->vScale = vScale;
	obj->vBoundingBox = vBoundingBox;
	obj->fRotation = fRotation;
	obj->bTransformationChanged = bTransformationChanged;

	// IRenderable
	obj->nBlendOperation = nBlendOperation;
	obj->cColor = cColor;
	obj->bColorChanged = bColorChanged;
	obj->bVisible = bVisible;

	return obj;
}

void Camera::Set(Reader &reader)
{
	sName = reader.ReadString("sName", sName.c_str());

	String proj = reader.ReadString("sProjection", "");
	if (proj != "")
	{
		std::transform(proj.begin(), proj.end(), proj.begin(), ::tolower);
		if (proj == "perspective")
			nProjection = eProjection::Perspective;
		else if (proj == "orthogonal")
			nProjection = eProjection::Orthogonal;
	}

	ITransformable::Unserialize(reader);
	IRenderable::Unserialize(reader);
}

bool Camera::Write(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("sType", this->GetTypeName());
		writer.WriteString("sName", sName.c_str());
		writer.WriteString("sProjection", nProjection == eProjection::Orthogonal ? "Orthogonal" : "Perspective");

		ITransformable::Serialize(writer);
		IRenderable::Serialize(writer);
	writer.CloseNode();

	return true;
}

} // namespace

