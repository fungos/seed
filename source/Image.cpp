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
