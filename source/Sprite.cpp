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

#include "Defines.h"
#include "Sprite.h"
#include "Animation.h"
#include "Frame.h"
#include "Screen.h"
#include "Log.h"
#include "Enum.h"
#include "RendererDevice.h"
#include "SeedInit.h"
#include "interface/ITexture.h"

#define TAG "[Sprite] "

namespace Seed {

Sprite::Sprite()
	: IBasicMesh()
	, ppAnimations(NULL)
	, ppAnimationFrames(NULL)
	, pFrame(NULL)
	, pFrameTexture(NULL)
	, iCurrentAnimation(0)
	, iCurrentFrame(0)
	, iAnimations(0)
	, iFrames(0)
	, vert()
	, sName()
	, bInitialized(false)
	, bChanged(false)
	, bAnimation(false)
	, bLoop(false)
	, bPlaying(false)
{
	iNumVertices = 4;
}

Sprite::~Sprite()
{
	this->Unload();
}

void Sprite::Reset()
{
	pFrameTexture	= NULL;
	ppAnimations	= NULL;
	pFrame			= NULL;
	ppAnimationFrames = NULL;
	bInitialized	= false;
	bChanged 		= false;
	bAnimation		= false;
	bLoop			= false;
	bVisible 		= true;
	bPlaying 		= false;

	iCurrentFrame	= 0;
	iFrames 		= 0;
	fFrameTime		= 0.0f;

	this->SetPriority(0);
}

bool Sprite::Unload()
{
	if (ppAnimations)
	{
		for (u32 i = 0; i < iAnimations; i++)
		{
			Animation *a = ppAnimations[i];
			ppAnimations[i] = NULL;
			Delete(a);
		}

		Free(ppAnimations);
	}

	pFrameTexture = NULL;
	ppAnimations = NULL;
	pFrame	= NULL;
	ppAnimationFrames = NULL;
	bInitialized = false;

	return true;
}

bool Sprite::Load(Reader &reader, ResourceManager *res)
{
	ASSERT_NULL(res);

	if (this->Unload())
	{
		sName = reader.ReadString("name", "sprite");
		iAnimations = reader.SelectArray("animations");

		ppAnimations = (Animation **)Alloc(iAnimations * sizeof(Animation *));
		for (u32 i = 0; i < iAnimations; i++)
		{
			Animation *a = New(Animation);
			reader.SelectNext();
			a->Load(reader, res);
			ppAnimations[i] = a;
		}

		bChanged = true;
		bInitialized = true;

		this->SetRotation(0);
		this->SetAnimation(0u);
	}

	return true;
}

void Sprite::Initialize()
{
	this->ReconfigureAnimation();
	bInitialized = true;
}

void Sprite::ReconfigureAnimation()
{
	bLoop = pAnimation->bLoop;
	bAnimation = pAnimation->bAnimated;
	fFrameTime = 0.0f;

	iFrames = pAnimation->iFrames;
	pFrame = ppAnimationFrames[iCurrentFrame];
	pFrameTexture = pFrame->pTexture;

	this->ReconfigureFrame();

	if (this->bAnimation)
		this->Play();
}

void Sprite::ReconfigureFrame()
{
	ASSERT_NULL(pFrameTexture);

	ITransformable::SetWidth(pFrame->iWidth);
	ITransformable::SetHeight(pFrame->iHeight);
	ITransformable::SetLocalX(pFrame->iHalfWidth);
	ITransformable::SetLocalY(pFrame->iHalfHeight);

	bChanged = true;
}

bool Sprite::SetAnimation(u32 index)
{
	bool ret = false;
	if (bInitialized && index < iAnimations)
	{
		Animation *pNewAnimation = ppAnimations[index];
		if (pNewAnimation)
		{
			pAnimation = pNewAnimation;
			ppAnimationFrames = pAnimation->GetFrames();
			iCurrentAnimation = index;
			iCurrentFrame = 0;

			this->ReconfigureAnimation();

			ret = true;
		}
		else
		{
			//Log(TAG "Warning animation %d not found.", index);
		}
	}
	return ret;
}

bool Sprite::SetAnimation(String name)
{
	bool ret = false;
	if (bInitialized)
	{
		Animation *pNewAnimation = NULL;
		u32 i = 0;
		for (; i < iAnimations; i++)
		{
			Animation *p = ppAnimations[i];
			if (p->sName == name)
			{
				pNewAnimation = p;
				break;
			}
		}

		if (pNewAnimation)
		{
			pAnimation = pNewAnimation;
			ppAnimationFrames = pAnimation->GetFrames();
			iCurrentAnimation = i;
			iCurrentFrame = 0;
			this->ReconfigureAnimation();

			ret = true;
		}
		else
		{
			//Log(TAG "Warning animation '%s' not found.", name);
		}
	}
	return ret;
}

u32 Sprite::GetAnimationCount() const
{
	return iAnimations;
}

u32 Sprite::GetAnimation() const
{
	return iCurrentAnimation;
}

const String Sprite::GetAnimationName() const
{
	String ret;
	if (pAnimation)
		ret = pAnimation->sName;

	return ret;
}

void Sprite::GotoAndStop(u32 frame)
{
	if (frame >= iFrames)
		return;

	iCurrentFrame = frame;
	bPlaying = false;
	this->ReconfigureAnimation();

	if (this->bVisible)
		bChanged = true;
}

void Sprite::GotoAndPlay(u32 frame)
{
	if (frame >= iFrames)
		return;

	iCurrentFrame = frame;
	bPlaying = true;
	this->ReconfigureAnimation();

	if (this->bVisible)
		bChanged = true;
}

void Sprite::Stop()
{
	bPlaying = false;
}

void Sprite::Play()
{
	bPlaying = true;
	bChanged = true;
}

bool Sprite::IsFinished() const
{
	return (iCurrentFrame == iFrames - 1);
}

void Sprite::NextFrame()
{
	if (iCurrentFrame < iFrames - 1)
		iCurrentFrame++;
	else if (bLoop)
		iCurrentFrame = 0;

	this->ReconfigureAnimation();

	if (this->bVisible)
		bChanged = true;
}

void Sprite::PreviousFrame()
{
	if (iCurrentFrame > 0)
		iCurrentFrame--;
	else if (bLoop)
		iCurrentFrame = iFrames - 1;

	this->ReconfigureAnimation();

	if (this->bVisible)
		bChanged = true;
}

void Sprite::SetCurrentFrame(u32 frame)
{
	if (frame >= iFrames || frame == iCurrentFrame)
		return;

	iCurrentFrame = frame;
	this->ReconfigureAnimation();

	bChanged = true;
}

u32 Sprite::GetCurrentFrame() const
{
	return iCurrentFrame;
}

u32 Sprite::GetFrameCount() const
{
	return iFrames;
}

void Sprite::SetLoop(bool loop)
{
	this->bLoop = loop;
}

bool Sprite::IsLoop() const
{
	return bLoop;
}

bool Sprite::IsAnimated() const
{
	return bAnimation;
}

void Sprite::Update(f32 delta)
{
	//IBasicMesh::Update(delta);

	if (bPlaying && bAnimation)
	{
		fFrameTime += delta;

		f32 rate = pFrame->fFrameRate;
		if (fFrameTime > rate)
		{
			fFrameTime -= rate;
			if (iCurrentFrame + 1 == iFrames)
			{
				if (bLoop)
				{
					iCurrentFrame = 0;
				}
				else
				{
					bChanged = false;
				}
			}
			else
				iCurrentFrame++;

			pFrame = ppAnimationFrames[iCurrentFrame];
			pFrameTexture = pFrame->pTexture;

			this->ReconfigureFrame();
		}
	}

	if (!bChanged && !this->IsChanged())
		return;

	if (arCustomVertexData)
	{
		arCurrentVertexData = arCustomVertexData;
	}
	else if (bTransformationChanged)
	{
		arCurrentVertexData = &vert[0];

		f32 u0, u1, v0, v1, x, y, w, h, z;
		uPixel p = iColor;
		p.rgba.r = iColor.argb.r;
		p.rgba.g = iColor.argb.g;
		p.rgba.b = iColor.argb.b;
		p.rgba.a = iColor.argb.a;

		x = vPos.x;
		y = vPos.y;
		z = vPos.z;
		w = vBoundingBox.x;
		h = vBoundingBox.y;
		u0 = pFrame->fTexS0;
		v0 = pFrame->fTexT0;
		u1 = pFrame->fTexS1;
		v1 = pFrame->fTexT1;

		vert[0].cVertex = Vector3f(x, y, z);
		vert[0].iColor = p;
		vert[0].cCoords = Point2f(u0, v0);

		vert[1].cVertex = Vector3f(w, y, z);
		vert[1].iColor = p;
		vert[1].cCoords = Point2f(u1, v0);

		vert[2].cVertex = Vector3f(x, h, z);
		vert[2].iColor = p;
		vert[2].cCoords = Point2f(u0, v1);

		vert[3].cVertex = Vector3f(w, h, z);
		vert[3].iColor = p;
		vert[3].cCoords = Point2f(u1, v1);

		Matrix4x4f t1, t2, r, s;
		t1 = TranslationMatrix(vPos + vLocalPos);
		r = RotationMatrix(AxisZ, DegToRad(Sprite::GetRotation()));
		s = ScaleMatrix(vScale);
		t2 = TranslationMatrix(-vLocalPos);
		Matrix4x4f transform = ((t1 * r) * s) * t2;

		for (u32 i = 0; i < iNumVertices; i++)
			transform.Transform(arCurrentVertexData[i].cVertex);
	}

	bChanged = false;
	bTransformationChanged = false;
}

PIXEL Sprite::GetPixel(u32 x, u32 y) const
{
	PIXEL ret = 0;

	if (pFrame && pFrameTexture)
		ret = pFrameTexture->GetPixel(x + pFrame->iX, y + pFrame->iY);

	return ret;
}

u8 Sprite::GetPixelAlpha(u32 x, u32 y) const
{
	u8 ret = 0;
	if (pFrame && pFrameTexture)
		ret = pFrameTexture->GetPixelAlpha(x + pFrame->iX, y + pFrame->iY);

	return ret;
}

const void *Sprite::GetData() const
{
	return pFrameTexture->GetData();
}

ITexture *Sprite::GetTexture() const
{
	return pFrameTexture;
}

void Sprite::Render()
{
	if (!this->bInitialized)
		return;

	ASSERT(pFrameTexture);

	RendererPacket packet;
	packet.iSize = iNumVertices;
	packet.nMeshType = nMeshType;
	packet.pVertexData = arCurrentVertexData;
	packet.pTexture = pFrameTexture;
	packet.nBlendMode = this->eBlendOperation;
	packet.iColor = this->iColor;
	pRendererDevice->UploadData(&packet);

	if (pConfiguration->bDebugSprite)
		pRendererDevice->DrawRect(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight(), PIXEL_COLOR(255, 255, 255, 255));
}

const char *Sprite::GetObjectName() const
{
	return "Sprite";
}

int Sprite::GetObjectType() const
{
	return Seed::ObjectSprite;
}

} // namespace
