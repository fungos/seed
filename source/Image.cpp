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

namespace Seed {

ISceneObject *FactoryImage()
{
	return New(Image());
}

Image::Image()
	: pTexture(NULL)
	, pRes(NULL)
	, sFilename(NULL)
	, iHalfWidth(0)
	, iHalfHeight(0)
	, iWidth(0)
	, iHeight(0)
	, cVertexBuffer()
	, vert()
	, bDynamic(false)
{
	cVertexBuffer.SetVertexData(vert, 4);
}

Image::~Image()
{
}

void Image:: Update(f32 delta)
{
	UNUSED(delta);
	if (this->IsChanged())
	{
		f32  x1, y1, x2, y2, z;
		x2 = vBoundingBox.getX() * 0.5f;
		y2 = vBoundingBox.getY() * 0.5f;
		x1 = -x2;
		y1 = -y2;
		z = vPos.getZ();

		vert[0].cVertex = Vector3f(x1, y1, z);
		vert[1].cVertex = Vector3f(x2, y1, z);
		vert[2].cVertex = Vector3f(x1, y2, z);
		vert[3].cVertex = Vector3f(x2, y2, z);

		this->UpdateTransform();

		Vector4f tmp;
		for (u32 i = 0; i < 4; i++)
		{
			tmp = mTransform * Vector4f(vert[i].cVertex, 1.0f);
			vert[i].cVertex = tmp.getXYZ();
		}
	}
}

void Image::Render(const Matrix4f &worldTransform)
{
	if (pTexture && pTexture->GetData())
	{
		RendererPacket packet;
		packet.pTransform = &mTransform; // &worldTransform; // FIXME: ortho or billboard
		packet.nMeshType = Seed::TriangleStrip;
		packet.pVertexBuffer = &cVertexBuffer;
		packet.pTexture = pTexture;
		packet.nBlendMode = eBlendOperation;
		packet.cColor = cColor;
		pRendererDevice->UploadData(&packet);
	}
}

bool Image::Load(ITexture *texture)
{
	SEED_ASSERT(texture);

	if (!bDynamic)
		sRelease(pTexture);

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

bool Image::Load(Reader &reader, ResourceManager *res)
{
	SEED_ASSERT(res);

	bool ret = false;

	if (this->Unload())
	{
		sName = reader.ReadString("sName", "image");
		sFilename = reader.ReadString("sFilename", "");

		ITransformable::Unserialize(reader);
		IRenderable::Unserialize(reader);

		pRes = res;

		pTexture = static_cast<ITexture *>(res->Get(sFilename, Seed::TypeTexture));
		this->UpdateCoords();

		bDynamic = false;
	}

	return ret;
}

bool Image::Unload()
{
	if (!bDynamic)
		sRelease(pTexture);
	pTexture = NULL;

	return true;
}

bool Image::Write(Writer &writer)
{
	bool ret = !bDynamic;
	if (ret)
	{
		writer.OpenNode();
			writer.WriteString("sType", this->GetObjectName().c_str());
			writer.WriteString("sName", sName.c_str());
			writer.WriteString("sFilename", sFilename.c_str());

			ITransformable::Serialize(writer);
			IRenderable::Serialize(writer);
		writer.CloseNode();
	}

	return ret;
}

int Image::GetObjectType() const
{
	return Seed::TypeImage;
}

const String Image::GetObjectName() const
{
	return "Image";
}

} // namespace
