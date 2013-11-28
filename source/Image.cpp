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

#include "Image.h"
#include "RendererDevice.h"
#include "Texture.h"
#include "Screen.h"
#include "MathUtil.h"
#include "SeedInit.h"
#include "Configuration.h"
#include "Memory.h"

namespace Seed {

ISceneObject *FactoryImage()
{
	return sdNew(Image);
}

Image::Image()
	: pTexture(nullptr)
	, sFilename()
	, iHalfWidth(0)
	, iHalfHeight(0)
	, iWidth(0)
	, iHeight(0)
	, cVertexBuffer()
	, vert()
	, bDynamic(false)
{
	cVertexBuffer.SetData(vert, 4);
}

Image::Image(const String &filename, ResourceManager *res)
	: pTexture(nullptr)
	, sFilename()
	, iHalfWidth(0)
	, iHalfHeight(0)
	, iWidth(0)
	, iHeight(0)
	, cVertexBuffer()
	, vert()
	, bDynamic(false)
{
	cVertexBuffer.SetData(vert, 4);
	this->Load(filename, res);
}

Image::~Image()
{
	pRendererDevice->DestroyHardwareBuffer(&cVertexBuffer);
	this->Unload();
}

void Image::Update(Seconds dt)
{
	UNUSED(dt);
	if (!this->IsChanged())
		return;

	auto x2 = vBoundingBox.getX() * 0.5f;
	auto y2 = vBoundingBox.getY() * 0.5f;
	auto x1 = -x2;
	auto y1 = -y2;
	auto z = vPos.getZ();

	vert[0].cVertex = Vector3f{x1, y1, z};
	vert[1].cVertex = Vector3f{x2, y1, z};
	vert[2].cVertex = Vector3f{x1, y2, z};
	vert[3].cVertex = Vector3f{x2, y2, z};

	this->UpdateTransform();
}

void Image::Render(const Matrix4f &worldTransform)
{
	if (!pTexture || !pTexture->GetData())
		return;

	auto flags = static_cast<ePacketFlags>((pConfiguration->bDebugSprite ? ePacketFlags::Wireframe : ePacketFlags::None));
	RendererPacket packet;
	packet.pTransform = &worldTransform; // FIXME: ortho or billboard
	packet.nMeshType = eMeshType::TriangleStrip;
	packet.pVertexBuffer = &cVertexBuffer;
	packet.pTexture = pTexture;
	packet.nBlendMode = nBlendOperation;
	packet.cColor = cColor;
	packet.nFlags = flags;
	packet.vPivot = vTransformedPivot;

	auto box= Rect4f{0, 0, this->GetWidth(), this->GetHeight()};
	packet.fRadius = box.CircleRadius();

	pRendererDevice->UploadData(&packet);
}

bool Image::Load(ITexture *texture)
{
	SEED_ASSERT(texture);

	if (!bDynamic)
		sdRelease(pTexture);

	pTexture = nullptr;

	sFilename = "[dynamic texture]";
	pTexture = texture;
	this->UpdateCoords();

	bDynamic = true;

	return true;
}

void Image::UpdateCoords()
{
	iWidth = pTexture->GetWidth();
	iHeight = pTexture->GetHeight();
	ITransformable::SetWidth(f32(pTexture->GetWidth())); // set normalized width
	ITransformable::SetHeight(f32(pTexture->GetHeight())); // set normalized height

	auto rInvWidth = 1.0f / pTexture->GetAtlasWidth(); // full width from image, not only frame area
	auto rInvHeight = 1.0f / pTexture->GetAtlasHeight(); // full height from image, not only frame area

	iHalfWidth = iWidth >> 1;
	iHalfHeight = iHeight >> 1;

	auto s0 = 0.0f;
	auto s1 = f32{iWidth * rInvWidth};
	auto t0 = 0.0f;
	auto t1 = f32{iHeight * rInvHeight};

	vert[0].cCoords = Point2f{s0, t0};
	vert[1].cCoords = Point2f{s1, t0};
	vert[2].cCoords = Point2f{s0, t1};
	vert[3].cCoords = Point2f{s1, t1};
}

bool Image::Load(const String &filename, ResourceManager *res)
{
	SEED_ASSERT(res);
	if (!this->Unload())
		return false;

	sName = filename;
	sFilename = filename;
	pRes = res;

	pTexture = static_cast<ITexture *>(res->Get(sFilename, ITexture::GetTypeId()));
	this->UpdateCoords();

	bDynamic = false;

	return true;
}


void Image::Set(Reader &reader)
{
	sName = reader.ReadString("sName", sName.c_str());
	auto filename = String(reader.ReadString("sResource", sFilename.c_str()));

	ITransformable::Unserialize(reader);
	IRenderable::Unserialize(reader);

	if (filename != sFilename)
	{
		sFilename = filename;

		sdRelease(pTexture);
		pTexture = static_cast<ITexture *>(pRes->Get(sFilename, ITexture::GetTypeId()));
		this->UpdateCoords();
	}

	SEED_ASSERT_FMT(pTexture, "Image '%s': Has no texture.", sName.c_str());
	bDynamic = false;
}

bool Image::Unload()
{
	pRendererDevice->DestroyHardwareBuffer(&cVertexBuffer);
	if (!bDynamic)
		sdRelease(pTexture);
	pTexture = nullptr;
	sFilename = "";
	sName = this->GetTypeName();

	return true;
}

Image *Image::Clone() const
{
	auto obj = sdNew(Image);
	obj->GenerateCloneName(sName);

	if (!bDynamic)
		sdAcquire(pTexture);

	obj->pTexture = pTexture;
	obj->pRes = pRes;
	obj->sFilename = sFilename;

	obj->iHalfWidth = iHalfWidth;
	obj->iHalfHeight = iHalfHeight;
	obj->iWidth = iWidth;
	obj->iHeight = iHeight;
	obj->bDynamic = bDynamic;

	memcpy(&obj->vert, &vert, sizeof(sVertex));
	obj->cVertexBuffer.SetData(obj->vert, 4);

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

	obj->UpdateCoords();

	return obj;
}

bool Image::Write(Writer &writer)
{
	if (bDynamic)
		return false;

	writer.OpenNode();
		writer.WriteString("sType", this->GetTypeName());
		writer.WriteString("sName", sName.c_str());
		writer.WriteString("sResource", sFilename.c_str());

		ITransformable::Serialize(writer);
		IRenderable::Serialize(writer);
	writer.CloseNode();

	return true;
}

} // namespace
