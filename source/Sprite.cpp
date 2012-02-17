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

/*! \file Sprite.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Sprite class interface
*/

#include "Defines.h"
#include "Sprite.h"
#include "Screen.h"
#include "Log.h"
#include "Enum.h"
#include "RendererDevice.h"
#include "interface/ITexture.h"

#define TAG "[Sprite] "

namespace Seed {

Sprite::Sprite()
	: IBasicMesh()
	, ppAnimations(NULL)
	, ppAnimationFrames(NULL)
	, pFrame(NULL)
	, pFrameTexture(NULL)
	, bInitialized(false)
	, bChanged(false)
	, bAnimation(false)
	, bLoop(false)
	, bPlaying(false)
	, fAspectHalfWidth(0.0f)
	, fAspectHalfHeight(0.0f)
	, fAspectWidth(0.0f)
	, fAspectHeight(0.0f)
	, iCurrentAnimation(0)
	, iCurrentFrame(0)
	, iAnimations(0)
	, iFrames(0)
	, fCurrentFrameRate(0.0f)
	, fFrameTime(0.0f)
	, iHalfWidth(0)
	, iHalfHeight(0)
	, iWidth(0)
	, iHeight(0)
	, fTexS0(0.0f)
	, fTexS1(0.0f)
	, fTexT0(0.0f)
	, fTexT1(0.0f)
	, pRes(NULL)
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
	fCurrentFrameRate = 1.0f / SPRITE_GLOBAL_FRAME_TIME;
	fFrameTime		= 0.0f;
	iWidth 			= 0;
	iHeight 		= 0;
	iHalfWidth 		= 0;
	iHalfHeight 	= 0;
	fTexS0			= 0;
	fTexS1			= 0;
	fTexT0			= 0;
	fTexT1			= 0;

	this->SetPriority(0);
}

bool Sprite::Unload()
{
	if (ppAnimations)
	{
		for (u32 i = 0; i < iAnimations; i++)
		{
			Animation *a = ppAnimations[i];
			a->Unload();
			ppAnimations[i] = NULL;
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

bool Sprite::Load(const void *data)
{
	return this->Load(data, pResourceManager);
}

bool Sprite::Load(const void *data, ResourceManager *res)
{
	ASSERT_NULL(data);
	ASSERT_NULL(res);

	if (this->Unload())
	{
		pRes = res;

//		pSprite = static_cast<SpriteObject *>(res->Get(filename, Seed::ObjectSprite));
		this->SetRotation(0);
		this->SetAnimation(0u);

		bChanged = true;
		bInitialized = true;

		ppAnimations = (Animation **)Alloc(iAnimations * sizeof(Animation *));
		for (u32 i = 0; i < iAnimations; i++)
		{
			Animation *a = New(Animation);
			a->Load(data);
			ppAnimations[i] = a;
		}

		pAnimation = ppAnimations[iCurrentAnimation];
		ppAnimationFrames = pAnimation->GetFrames();
		iFrames = pAnimation->GetFrameCount();
		pFrame = ppAnimationFrames[iCurrentFrame];
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
	iFrames = pAnimation->iFrames;
	bLoop = ((pAnimation->iFlags & Seed::FlagLooped) == Seed::FlagLooped);
	bAnimation = ((pAnimation->iFlags & Seed::FlagAnimated) == Seed::FlagAnimated);
	fFrameTime = 0.0f;

	pFrame = ppAnimationFrames[iCurrentFrame];

	sRelease(pFrameTexture);
	pFrameTexture = pFrame->pTexture; //static_cast<ITexture *>(pRes->Get(pFrame->pFilename, Seed::ObjectTexture));

	this->ReconfigureFrame();

	if (this->bAnimation)
		this->Play();
}

// WARNING: pFrameImage must be valid here.
// FIXME: This can be simplified, do it someday.
void Sprite::ReconfigureFrame()
{
	ASSERT_NULL(pFrameTexture);

	fCurrentFrameRate = 1.0f / static_cast<f32>(pFrame->iTime);
	if (pFrame->iWidth == 0)
		iWidth = pFrameTexture->GetWidthInPixel();
	else
		iWidth = static_cast<u16>(pFrame->iWidth);

	if (pFrame->iHeight == 0)
		iHeight = pFrameTexture->GetHeightInPixel();
	else
		iHeight = static_cast<u16>(pFrame->iHeight);

	f32 w = (iWidth / static_cast<f32>(pFrame->iResolutionWidth));
	f32 h = (iHeight / static_cast<f32>(pFrame->iResolutionHeight));

	ITransformable2D::SetWidth(w); // set normalized width
	ITransformable2D::SetHeight(h); // set normalized height

	fAspectWidth = w;
	fAspectHeight = h * pScreen->GetAspectRatio();
	fAspectHalfWidth = fAspectWidth / 2.0f;
	fAspectHalfHeight = fAspectHeight / 2.0f;

	u32 iX = pFrame->iX;
	u32 iY = pFrame->iY;

	f32 rInvWidth = 1.0F / pFrameTexture->GetAtlasWidthInPixel(); // full width from image, not only frame area
	f32 rInvHeight = 1.0F / pFrameTexture->GetAtlasHeightInPixel(); // full height from image, not only frame area

	// Normalized Pixel Half Width/Height for pixel based vertex rendering
	iHalfWidth = static_cast<s32>(pScreen->GetWidth() * (w / 2.0f));
	iHalfHeight = static_cast<s32>(pScreen->GetHeight() *  (h / 2.0f));

	/*
	FIXME: 0.1f from hell
	*/
	fTexS0 = static_cast<f32>((iX + 0.1f) * rInvWidth);
	fTexS1 = static_cast<f32>((iX + 0.1f + iWidth) * rInvWidth);
	fTexT0 = static_cast<f32>((iY + 0.1f) * rInvHeight);
	fTexT1 = static_cast<f32>((iY + 0.1f + iHeight) * rInvHeight);

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

bool Sprite::SetAnimation(const char *name)
{
	bool ret = false;
	if (bInitialized)
	{
		Animation *pNewAnimation = NULL;
		u32 i = 0;
		for (; i < iAnimations; i++)
		{
			Animation *p = ppAnimations[i];
			if (!STRCMP(p->pName, name))
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

const char *Sprite::GetAnimationName() const
{
	const char *ret = NULL;
	if (pAnimation)
		ret = pAnimation->pName;

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
	fCurrentFrameRate = 1.0f / static_cast<f32>(pFrame->iTime);
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

		if (fFrameTime > fCurrentFrameRate)
		{
			fFrameTime -= fCurrentFrameRate;

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
			sRelease(pFrameTexture);
			pFrameTexture = pFrame->pTexture;

			this->ReconfigureFrame();
		}
	}

	if (!bChanged && !this->IsChanged())
		return;

	uPixel p = iColor;
	p.rgba.r = iColor.argb.r;
	p.rgba.g = iColor.argb.g;
	p.rgba.b = iColor.argb.b;
	p.rgba.a = iColor.argb.a;

	if (!arCustomVertexData)
	{
		arCurrentVertexData = &vert[0];

		vert[0].cVertex = Vector3f(-fAspectHalfWidth, -fAspectHalfHeight, (f32)iPriority);
		vert[0].iColor = p;
		vert[0].cCoords = Point2f(fTexS0, fTexT0);

		vert[1].cVertex = Vector3f(+fAspectHalfWidth, -fAspectHalfHeight, (f32)iPriority);
		vert[1].iColor = p;
		vert[1].cCoords = Point2f(fTexS1, fTexT0);

		vert[2].cVertex = Vector3f(-fAspectHalfWidth, +fAspectHalfHeight, (f32)iPriority);
		vert[2].iColor = p;
		vert[2].cCoords = Point2f(fTexS0, fTexT1);

		vert[3].cVertex = Vector3f(+fAspectHalfWidth, +fAspectHalfHeight, (f32)iPriority);
		vert[3].iColor = p;
		vert[3].cCoords = Point2f(fTexS1, fTexT1);
	}
	else
	{
		arCurrentVertexData = arCustomVertexData;
	}

	f32 ratio = pScreen->GetAspectRatio();
	f32 x = fAspectHalfWidth + Sprite::GetX();
	f32 y = fAspectHalfHeight + Sprite::GetY() * ratio;

	f32 lx = Sprite::GetLocalX();
	f32 ly = Sprite::GetLocalY() * ratio;

	Matrix4x4f t1, t2, r, s;
	t1 = TranslationMatrix(lx + x, ly + y, 0.0f);
	r = RotationMatrix(AxisZ, DegToRad(Sprite::GetRotation()));
	s = ScaleMatrix(Sprite::GetScaleX(), Sprite::GetScaleY(), 1.0f);
	t2 = TranslationMatrix(-lx, -ly, 0.0f);
	Matrix4x4f transform = ((t1 * r) * s) * t2;

	if (bTransformationChanged || !arCustomVertexData)
	{
		for (u32 i = 0; i < iNumVertices; i++)
		{
			transform.Transform(arCurrentVertexData[i].cVertex);
		}
	}

	bChanged = false;
	bTransformationChanged = false;
}

u32 Sprite::GetWidthInPixel() const
{
	return iWidth;
}

u32 Sprite::GetHeightInPixel() const
{
	return iHeight;
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
