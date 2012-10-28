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
#include "Sprite.h"
#include "Animation.h"
#include "Frame.h"
#include "Screen.h"
#include "Log.h"
#include "Enum.h"
#include "RendererDevice.h"
#include "SeedInit.h"
#include "interface/ITexture.h"
#include "Profiler.h"
#include "Configuration.h"

#define TAG "[Sprite] "

namespace Seed {

ISceneObject *FactorySprite()
{
	return New(Sprite());
}

Sprite::Sprite()
	: ISceneObject()
	, pvFrames(NULL)
	, pAnimation(NULL)
	, pFrame(NULL)
	, pFrameTexture(NULL)
	, vAnimations()
	, iCurrentAnimation(0)
	, iCurrentFrame(0)
	, iFrames(0)
	, fFrameTime(0.0f)
	, cVertexBuffer()
	, bInitialized(false)
	, bChanged(false)
	, bAnimation(false)
	, bLoop(false)
	, bPlaying(false)
	, bFinished(false)
	, bIsCopy(false)
{
	cVertexBuffer.SetVertexData(cVertex, 4);
}

Sprite::~Sprite()
{
	this->Unload();
}

Sprite::Sprite(const Sprite &other)
	: ISceneObject()
	, pvFrames(other.pvFrames)
	, pAnimation(other.pAnimation)
	, pFrame(other.pFrame)
	, pFrameTexture(other.pFrameTexture)
	, vAnimations(other.vAnimations)
	, iCurrentAnimation(other.iCurrentAnimation)
	, iCurrentFrame(other.iCurrentFrame)
	, iFrames(other.iFrames)
	, fFrameTime(other.fFrameTime)
	, cVertexBuffer()
	, bInitialized(other.bInitialized)
	, bChanged(other.bChanged)
	, bAnimation(other.bAnimation)
	, bLoop(other.bLoop)
	, bPlaying(other.bPlaying)
	, bFinished(other.bFinished)
	, bIsCopy(true)
{
	cVertex[0] = other.cVertex[0];
	cVertex[1] = other.cVertex[1];
	cVertex[2] = other.cVertex[2];
	cVertex[3] = other.cVertex[3];

	// ITransformable
	pParent = other.pParent;
	mTransform = other.mTransform;
	vPos = other.vPos;
	vPivot = other.vPivot;
	vTransformedPivot = other.vTransformedPivot;
	vScale = other.vScale;
	vBoundingBox = other.vBoundingBox;
	fRotation = other.fRotation;
	bTransformationChanged = other.bTransformationChanged;

	// IRenderable
	eBlendOperation = other.eBlendOperation;
	cColor = other.cColor;
	bColorChanged = other.bColorChanged;
	bVisible = other.bVisible;
}

Sprite &Sprite::operator=(const Sprite &other)
{
	if (this != &other)
	{
		pvFrames = other.pvFrames;
		pAnimation = other.pAnimation;
		pFrame = other.pFrame;
		pFrameTexture = other.pFrameTexture;

		vAnimations = other.vAnimations;

		iCurrentAnimation = other.iCurrentAnimation;
		iCurrentFrame = other.iCurrentFrame;
		iFrames = other.iFrames;
		fFrameTime = other.fFrameTime;

		cVertex[0] = other.cVertex[0];
		cVertex[1] = other.cVertex[1];
		cVertex[2] = other.cVertex[2];
		cVertex[3] = other.cVertex[3];

		bInitialized = other.bInitialized;
		bChanged = other.bChanged;
		bAnimation = other.bAnimation;
		bLoop = other.bLoop;
		bPlaying = other.bPlaying;
		bFinished = other.bFinished;

		// ISceneObject
		sName = other.sName;

		// ITransformable
		pParent = other.pParent;
		mTransform = other.mTransform;
		vPos = other.vPos;
		vPivot = other.vPivot;
		vTransformedPivot = other.vTransformedPivot;
		vScale = other.vScale;
		vBoundingBox = other.vBoundingBox;
		fRotation = other.fRotation;
		bTransformationChanged = other.bTransformationChanged;

		// IRenderable
		eBlendOperation = other.eBlendOperation;
		cColor = other.cColor;
		bColorChanged = other.bColorChanged;
		bVisible = other.bVisible;
		bIsCopy = true;
	}

	return *this;
}

void Sprite::Reset()
{
	if (!bIsCopy)
	{
		AnimationVectorIterator it = vAnimations.begin();
		AnimationVectorIterator end = vAnimations.end();
		for (; it != end; ++it)
			Delete(*it);
	}

	AnimationVector().swap(vAnimations);

	pFrameTexture	= NULL;
	pAnimation		= NULL;
	pFrame			= NULL;
	pvFrames		= NULL;
	bInitialized	= false;
	bChanged		= false;
	bAnimation		= false;
	bLoop			= false;
	bVisible		= true;
	bPlaying		= false;

	iCurrentFrame	= 0;
	iFrames			= 0;
	fFrameTime		= 0.0f;

	this->SetZ(0);
}

void Sprite::operator+=(Animation *anim)
{
	SEED_ASSERT(anim);
	vAnimations += anim;
}

void Sprite::operator-=(Animation *anim)
{
	SEED_ASSERT(anim);
	vAnimations -= anim;
}

//void Sprite::Initialize()
//{
//	this->ReconfigureAnimation();
//	bInitialized = true;
//}

void Sprite::ReconfigureAnimation()
{
	bLoop = pAnimation->bLoop;
	bAnimation = pAnimation->bAnimated;
	fFrameTime = 0.0f;

	iFrames = pAnimation->iFrames;
	pFrame = pvFrames->at(iCurrentFrame);
	pFrameTexture = pFrame->pTexture;

	this->ReconfigureFrame();

	if (bAnimation)
		this->Play();
}

void Sprite::ReconfigureFrame()
{
	SEED_ASSERT(pFrameTexture);

	ITransformable::SetWidth(pFrame->iWidth);
	ITransformable::SetHeight(pFrame->iHeight);

	f32 u0, u1, v0, v1;

	u0 = pFrame->fTexS0;
	v0 = pFrame->fTexT0;
	u1 = pFrame->fTexS1;
	v1 = pFrame->fTexT1;

	cVertex[0].cCoords = Point2f(u0, v0);
	cVertex[1].cCoords = Point2f(u1, v0);
	cVertex[2].cCoords = Point2f(u0, v1);
	cVertex[3].cCoords = Point2f(u1, v1);

	bChanged = true;
}

bool Sprite::SetAnimation(u32 index)
{
	bool ret = false;
	if (bInitialized && index < vAnimations.Size())
	{
		Animation *pNewAnimation = vAnimations[index];
		if (pNewAnimation)
		{
			pAnimation = pNewAnimation;
			pvFrames = pAnimation->GetFrames();
			iCurrentAnimation = index;
			iCurrentFrame = 0;

			this->ReconfigureAnimation();

			ret = true;
		}
		else
		{
			Log(TAG "Warning animation %d not found.", index);
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

		u32 anims = (u32)vAnimations.Size();
		for (i = 0; i < anims; i++)
		{
			Animation *p = vAnimations[i];
			if (p->sName == name)
			{
				pNewAnimation = p;
				break;
			}
		}

		if (pNewAnimation)
		{
			pAnimation = pNewAnimation;
			pvFrames = pAnimation->GetFrames();
			iCurrentAnimation = i;
			iCurrentFrame = 0;
			this->ReconfigureAnimation();

			ret = true;
		}
		else
		{
			Log(TAG "Warning animation '%s' not found.", name.c_str());
		}
	}
	return ret;
}

u32 Sprite::GetAnimationCount() const
{
	return (u32)vAnimations.Size();
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
}

void Sprite::GotoAndPlay(u32 frame)
{
	if (frame >= iFrames)
		return;

	iCurrentFrame = frame;
	bPlaying = true;
	this->ReconfigureAnimation();
}

void Sprite::Stop()
{
	bPlaying = false;
}

void Sprite::Play()
{
	bPlaying = true;
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
}

void Sprite::PreviousFrame()
{
	if (iCurrentFrame > 0)
		iCurrentFrame--;
	else if (bLoop)
		iCurrentFrame = iFrames - 1;

	this->ReconfigureAnimation();
}

void Sprite::SetCurrentFrame(u32 frame)
{
	if (frame >= iFrames || frame == iCurrentFrame)
		return;

	iCurrentFrame = frame;
	this->ReconfigureAnimation();
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
	bLoop = loop;
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
	if (!bInitialized)
		return;

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
					iCurrentFrame = 0;
				else
					bChanged = false;
			}
			else
				iCurrentFrame++;

			Frame *next = pvFrames->at(iCurrentFrame);
			if (next != pFrame)
			{
				pFrame = next;
				pFrameTexture = pFrame->pTexture;

				this->ReconfigureFrame();
			}
		}
	}

	bTransformationChanged = this->IsChanged();
	bChanged |= bTransformationChanged;
	if (!bChanged)
		return;

	bChanged = false;
	if (bTransformationChanged)
	{
		f32  x1, y1, x2, y2, z;
		x2 = vBoundingBox.getX() * 0.5f;
		y2 = vBoundingBox.getY() * 0.5f;
		x1 = -x2;
		y1 = -y2;
		z = vPos.getZ();

		cVertex[0].cVertex = Vector3f(x1, y1, z);
		cVertex[1].cVertex = Vector3f(x2, y1, z);
		cVertex[2].cVertex = Vector3f(x1, y2, z);
		cVertex[3].cVertex = Vector3f(x2, y2, z);

		this->UpdateTransform();
	}

	if (bColorChanged)
	{
		bColorChanged = false;

		Color p = cColor;
		cVertex[0].cColor = p;
		cVertex[1].cColor = p;
		cVertex[2].cColor = p;
		cVertex[3].cColor = p;
	}
}

void Sprite::Render(const Matrix4f &worldTransform)
{
	if (!bInitialized)
		return;

	ePacketFlags flags = static_cast<ePacketFlags>((pConfiguration->bDebugSprite ? FlagWireframe : FlagNone));

	SEED_ASSERT(pFrameTexture);

	RendererPacket packet;
	packet.nMeshType = Seed::TriangleStrip;
	packet.pVertexBuffer = &cVertexBuffer;
	packet.pTexture = pFrameTexture;
	packet.nBlendMode = eBlendOperation;
	packet.pTransform = &worldTransform;
	packet.cColor = cColor;
	packet.iFlags = flags;
	packet.vPivot = vTransformedPivot;

	Rect4f box(0, 0, this->GetWidth(), this->GetHeight());
	packet.fRadius = box.CircleRadius();

	pRendererDevice->UploadData(&packet);
}

Color Sprite::GetPixel(u32 x, u32 y) const
{
	Color ret;

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

const String Sprite::GetObjectName() const
{
	return "Sprite";
}

int Sprite::GetObjectType() const
{
	return Seed::TypeSprite;
}

bool Sprite::Unload()
{
	if (!bIsCopy)
	{
		AnimationVectorIterator it = vAnimations.begin();
		AnimationVectorIterator end = vAnimations.end();
		for (; it != end; ++it)
			Delete(*it);
	}

	AnimationVector().swap(vAnimations);

	pFrameTexture	= NULL;
	pAnimation		= NULL;
	pFrame			= NULL;
	pvFrames		= NULL;
	bInitialized	= false;
	bChanged		= false;

	return true;
}

bool Sprite::Load(Reader &reader, ResourceManager *res)
{
	SEED_ASSERT(res);

	bool ret = false;

	if (this->Unload())
	{
		sName = reader.ReadString("sName", "sprite");
		u32 anims = reader.SelectArray("aAnimations");
		if (anims)
		{
			for (u32 i = 0; i < anims; i++)
			{
				Animation *a = New(Animation);
				reader.SelectNext();
				a->Load(reader, res);
				vAnimations += a;
			}
			reader.UnselectArray();

			bInitialized = true;

			ITransformable::Unserialize(reader);
			IRenderable::Unserialize(reader);

			s32 anim = reader.ReadS32("iAnimation", -1);
			if (anim == -1)
			{
				String sanim = reader.ReadString("sAnimation", "");
				if (sanim == "")
					this->SetAnimation(0u);
				else
					this->SetAnimation(sanim);
			}
			else
				this->SetAnimation(anim);

			ret = true;
		}
		else
		{
			Log(TAG " WARNING: Animations not found in the sprite '%s'", sName.c_str());
		}
	}

	return ret;
}

bool Sprite::Write(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("sType", this->GetObjectName().c_str());
		writer.WriteString("sName", sName.c_str());
		writer.WriteS32("iAnimation", this->GetAnimation());

		ITransformable::Serialize(writer);
		IRenderable::Serialize(writer);

		writer.OpenArray("aAnimations");
		u32 anims  = (u32)vAnimations.Size();
		for (u32 i = 0; i < anims; i++)
		{
			Animation *anim = vAnimations[i];
			anim->Write(writer);
		}
		writer.CloseArray();
	writer.CloseNode();

	return true;
}

} // namespace
