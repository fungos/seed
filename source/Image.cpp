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

#include "Image.h"
#include "RendererDevice.h"
#include "Texture.h"
#include "Screen.h"
#include "Matrix4x4.h"
#include "SeedInit.h"

namespace Seed {

Image::Image()
	: pTexture(NULL)
	, pRes(NULL)
	, sFilename(NULL)
	, iHalfWidth(0)
	, iHalfHeight(0)
	, iWidth(0)
	, iHeight(0)
	, bDynamic(false)
{
}

Image::~Image()
{
}

bool Image::Unload()
{
	if (!bDynamic)
		sRelease(pTexture);
	pTexture = NULL;

	return true;
}

bool Image::Load(const String &filename)
{
	return this->Load(filename, pResourceManager);
}

bool Image::Load(const String &filename, ResourceManager *res)
{
	ASSERT_NULL(res);

	if (this->Unload())
	{
		sFilename = filename;
		pRes = res;

		pTexture = static_cast<ITexture *>(res->Get(filename, Seed::ObjectTexture));
		this->UpdateCoords();
		bDynamic = false;
	}

	return true;
}

void Image:: Update(f32 delta)
{
	UNUSED(delta);
	if (this->IsChanged())
	{
		vert[0].cVertex = Vector3f(-iHalfWidth, -iHeight, vPos.z);
		vert[0].iColor = iColor;

		vert[1].cVertex = Vector3f(+iHalfWidth, -iHalfHeight, vPos.z);
		vert[1].iColor = iColor;

		vert[2].cVertex = Vector3f(-iHalfWidth, +iHalfHeight, vPos.z);
		vert[2].iColor = iColor;

		vert[3].cVertex = Vector3f(+iHalfWidth, +iHalfHeight, vPos.z);
		vert[3].iColor = iColor;

		f32 x = iHalfWidth + this->GetX();
		f32 y = iHalfHeight + this->GetY() * pScreen->GetAspectRatio();
		f32 lx = this->GetLocalX();
		f32 ly = this->GetLocalY();

		Matrix4x4f t1, t2, r, s;
		t1 = TranslationMatrix(lx + x, ly + y, 0.0f);
		r = RotationMatrix(AxisZ, DegToRad(this->GetRotation()));
		s = ScaleMatrix(this->GetScaleX(), this->GetScaleY(), 1.0f);
		t2 = TranslationMatrix(-lx, -ly, 0.0f);
		Matrix4x4f transform = ((t1 * r) * s) * t2;

		transform.Transform(vert[0].cVertex);
		transform.Transform(vert[1].cVertex);
		transform.Transform(vert[2].cVertex);
		transform.Transform(vert[3].cVertex);

		bTransformationChanged = false;
	}
}

void Image::Render()
{
	if (pTexture && pTexture->GetData())
	{
		RendererPacket packet;
		packet.iSize = 4;
		packet.nMeshType = Seed::TriangleStrip;
		packet.pVertexData = &vert;
		packet.pTexture = pTexture;
		packet.nBlendMode = eBlendOperation;
		packet.iColor = iColor;
		pRendererDevice->UploadData(&packet);
	}
}

bool Image::Load(ITexture *texture)
{
	ASSERT_NULL(texture);

	if (!bDynamic)
	{
		sRelease(pTexture);
	}
	pTexture = NULL;

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
	ITransformable::SetWidth(pTexture->GetWidth()); // set normalized width
	ITransformable::SetHeight(pTexture->GetHeight()); // set normalized height

	f32 rInvWidth = 1.0f / pTexture->GetAtlasWidth(); // full width from image, not only frame area
	f32 rInvHeight = 1.0f / pTexture->GetAtlasHeight(); // full height from image, not only frame area

	iHalfWidth = iWidth >> 1;
	iHalfHeight = iHeight >> 1;

	f32 s0 = 0.0f;
	f32 s1 = static_cast<f32>(iWidth * rInvWidth);
	f32 t0 = 0.0f;
	f32 t1 = static_cast<f32>(iHeight * rInvHeight);

	vert[0].cCoords = Point2f(s0, t0);
	vert[1].cCoords = Point2f(s1, t0);
	vert[2].cCoords = Point2f(s0, t1);
	vert[3].cCoords = Point2f(s1, t1);
}

int Image::GetObjectType() const
{
	return Seed::ObjectImage;
}

const char *Image::GetObjectName() const
{
	return "Image";
}

} // namespace
