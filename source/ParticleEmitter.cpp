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

#include "ParticleEmitter.h"
#include "Rand.h"
#include "Number.h"
#include "Texture.h"
#include "SeedInit.h"
#include "Configuration.h"
#include "Particle.h"
#include "MathUtil.h"
#include "RendererDevice.h"
#include "Sprite.h"

namespace Seed {

ISceneObject *FactoryParticleEmitter()
{
	return New(ParticleEmitter());
}

ParticleEmitter::ParticleEmitter()
	: pRes(NULL)
	, arParticles(NULL)
	, pTexture(NULL)
	, cEmitter()
	, pTemplate(NULL)
	, sSprite()
	, sBlending()
	, vPrevLocation(0.0f, 0.0f, 0.0f)
	, rBoundingBox()
	, fAge(0.0f)
	, fRespawnAge(0.0f)
	, fEmissionResidue(0.0f)
	, fInterval(0.0f)
	, fTx(0.0f)
	, fTy(0.0f)
	, fScale(1.0f)
	, fParticleWidhtHalf(0.0f)
	, fParticleHeightHalf(0.0f)
	, iAnimation(0)
	, iParticlesAmount(0)
	, nMinFilter(TextureFilterLinear)
	, nMagFilter(TextureFilterLinear)
	, cVertexBuffer()
	, pVertex(NULL)
	, iVertexAmount(0)
	, bParticlesFollowEmitter(false)
	, bPaused(false)
	, bEnabled(true)
	, bAutoPlay(false)
	, bInitialized(false)
{
	cVertexBuffer.Configure(BufferTargetArray, BufferUsageEveryFrameChange);
}

ParticleEmitter::~ParticleEmitter()
{
	this->Unload();
}

bool ParticleEmitter::Unload()
{
	iParticlesAmount = 0;
	DeleteArray(arParticles);
	Delete(pTemplate);
	Free(pVertex);

	fInterval = 0.0f;
	iAnimation = 0;
	bInitialized = false;
	bAutoPlay = false;
	bEnabled = true;
	bPaused = false;

	memset(&cEmitter,'\0', sizeof(cEmitter));

	return true;
}

void ParticleEmitter::Reset()
{
	ITransformable::Reset();
	vPrevLocation = Vector3f();
	fTx = 0.0f;
	fTy = 0.0f;
	fScale = 1.0f;
	fEmissionResidue = 0.0f;
	fAge = -2.0f;
	fRespawnAge = 0.0f;
	bPaused = false;
	bInitialized = false;
}

void ParticleEmitter::Update(f32 deltaTime)
{
	if (!bInitialized)
		return;

	if (!(bEnabled && !bPaused))
		return;

	rBoundingBox.x1 = 99999.0f;
	rBoundingBox.y1 = 99999.0f;
	rBoundingBox.x2 = -99999.0f;
	rBoundingBox.y2 =  -99999.0f;

	f32 ang = 0.0f;
	Particle *par = NULL;
	Vector3f accel(0.0f, 0.0f, 0.0f);
	Vector3f accel2(0.0f, 0.0f, 0.0f);
	Vector3f location = this->GetPosition();

	if (fAge == -2.0f && cEmitter.fLifetime != -1.0f && fInterval > 0.0f)
	{
		fRespawnAge += deltaTime;

		if (fRespawnAge >= fInterval)
		{
			fAge = 0.0f;
			fRespawnAge = 0.0f;
		}
	}

	if (fAge >= 0)
	{
		fAge += deltaTime;
		if (fAge >= cEmitter.fLifetime)
			fAge = -2.0f;
	}

	for (u32 i = 0; i < iParticlesAmount; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		par = &arParticles[i];

		par->fAge += deltaTime;
		if (par->fAge >= par->fTerminalAge)
		{
			arParticles[i].bActive = false;
			continue;
		}

		accel = par->vPosition - location;
		accel = normalize(accel);
		accel2 = accel;
		accel *= par->fRadialAccel;

		ang = accel2.getX();
		accel2.setX(-accel2.getY());
		accel2.setY(ang);

		accel2 *= par->fTangentialAccel;
		par->vVelocity += (accel + accel2) * deltaTime;
		par->vVelocity.setY(par->vVelocity.getY() + par->fGravity * deltaTime);

		par->fSpin += par->fSpinDelta * deltaTime;
		par->fSize += par->fSizeDelta * deltaTime;
		par->fColorR += par->fColorDeltaR * deltaTime;
		par->fColorG += par->fColorDeltaG * deltaTime;
		par->fColorB += par->fColorDeltaB * deltaTime;
		par->fColorA += par->fColorDeltaA * deltaTime;

		par->vScale.setX(par->fSize);
		par->vScale.setY(par->fSize);
		par->fRotation += par->fSpin;

		par->vPosition += (par->vVelocity * deltaTime);
		rBoundingBox.Encapsulate(par->vPosition.getX(), par->vPosition.getY());
	}

	// Create more particles
	if (fAge != -2.0f)
	{
		f32 fParticlesNeeded = cEmitter.iEmission * deltaTime + fEmissionResidue;
		u32 iParticlesCreated = static_cast<u32>(Number::Ceil(fParticlesNeeded));
		fEmissionResidue = fParticlesNeeded - iParticlesCreated;

		for (u32 i = 0; i < iParticlesCreated; i++)
		{
			bool bFull = true;
			for (u32 j = 0; j < iParticlesAmount; j++)
			{
				if (!arParticles[j].bActive)
				{
					par = &arParticles[j];
					par->bActive = true;
					bFull = false;
					break;
				}
			}
			if (bFull)
				break;

			par->fAge = 0.0f;
			par->fTerminalAge = pRand->Get(cEmitter.fParticleLifeMin, cEmitter.fParticleLifeMax) * 0.70f;

			Vector3f pos = vPrevLocation + (location - vPrevLocation) * pRand->Get(0.0f, 1.0f);

			pos.setX(pos.getX() + pRand->Get(cEmitter.fWidth) - cEmitter.fWidth / 2.0f);
			pos.setY(pos.getY() + pRand->Get(cEmitter.fHeight) - cEmitter.fHeight / 2.0f);

			ang = cEmitter.fDirection - kPiOver2 + pRand->Get(0, cEmitter.fSpread) - cEmitter.fSpread / 2.0f;
			if (cEmitter.bRelative)
				ang += VectorAngle(vPrevLocation - location) + kPiOver2;

			par->vVelocity.setX(cosf(ang));
			par->vVelocity.setY(sinf(ang));
			par->vVelocity *= pRand->Get(cEmitter.fSpeedMin, cEmitter.fSpeedMax);

			par->fGravity = pRand->Get(cEmitter.fGravityMin, cEmitter.fGravityMax);
			par->fRadialAccel = pRand->Get(cEmitter.fRadialAccelMin, cEmitter.fRadialAccelMax);
			par->fTangentialAccel = pRand->Get(cEmitter.fTangentialAccelMin, cEmitter.fTangentialAccelMax);

			par->fSize = pRand->Get(cEmitter.fSizeStart, cEmitter.fSizeStart + (cEmitter.fSizeEnd - cEmitter.fSizeStart) * cEmitter.fSizeVar);
			par->fSizeDelta = (cEmitter.fSizeEnd - par->fSize) / par->fTerminalAge;

			par->fSpin = pRand->Get(cEmitter.fSpinStart, cEmitter.fSpinStart + (cEmitter.fSpinEnd - cEmitter.fSpinStart) * cEmitter.fSpinVar);
			par->fSpinDelta = (cEmitter.fSpinEnd - par->fSpin) / par->fTerminalAge;

			par->fColorR = pRand->Get(cEmitter.fColorStartR, cEmitter.fColorStartR + (cEmitter.fColorEndR - cEmitter.fColorStartR) * cEmitter.fColorVar);
			par->fColorG = pRand->Get(cEmitter.fColorStartG, cEmitter.fColorStartG + (cEmitter.fColorEndG - cEmitter.fColorStartG) * cEmitter.fColorVar);
			par->fColorB = pRand->Get(cEmitter.fColorStartB, cEmitter.fColorStartB + (cEmitter.fColorEndB - cEmitter.fColorStartB) * cEmitter.fColorVar);
			par->fColorA = pRand->Get(cEmitter.fColorStartA, cEmitter.fColorStartA + (cEmitter.fColorEndA - cEmitter.fColorStartA) * cEmitter.fAlphaVar);

			par->fColorDeltaR = (cEmitter.fColorEndR - par->fColorR) / par->fTerminalAge;
			par->fColorDeltaG = (cEmitter.fColorEndG - par->fColorG) / par->fTerminalAge;
			par->fColorDeltaB = (cEmitter.fColorEndB - par->fColorB) / par->fTerminalAge;
			par->fColorDeltaA = (cEmitter.fColorEndA - par->fColorA) / par->fTerminalAge;

			par->vScale.setX(par->fSize);
			par->vScale.setY(par->fSize);
			par->vPosition.setX(pos.getX());
			par->vPosition.setY(pos.getY());
			rBoundingBox.Encapsulate(par->vPosition.getX(), par->vPosition.getY());
		}
	}

	rBoundingBox.x2++;
	rBoundingBox.y2++;

	ITexture *img = pTemplate->GetTexture();
	if (img)
	{
		img->SetFilter(TextureFilterTypeMag, nMagFilter);
		img->SetFilter(TextureFilterTypeMin, nMinFilter);
	}

	if (bParticlesFollowEmitter)
		MoveEverything(vPos);

	vPrevLocation = location;
	ITransformable::UpdateTransform();

	memset(pVertex, '\0', sizeof(sVertex) * 6 * iParticlesAmount);
	iVertexAmount = 0;
	for (u32 i = 0; i < iParticlesAmount; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		Particle *p = &arParticles[i];
		Color c(p->fColorR * 255, p->fColorG * 255, p->fColorB * 255, p->fColorA * 255);
		{
			pVertex[iVertexAmount + 0].cCoords = pTemplate->cVertex[0].cCoords;
			pVertex[iVertexAmount + 0].cColor = c;
			pVertex[iVertexAmount + 0].cVertex = p->vPosition + Vector3f(-fParticleWidhtHalf, -fParticleHeightHalf, 1.0f);
			pVertex[iVertexAmount + 1].cCoords = pTemplate->cVertex[1].cCoords;
			pVertex[iVertexAmount + 1].cColor = c;
			pVertex[iVertexAmount + 1].cVertex = p->vPosition + Vector3f(fParticleWidhtHalf, -fParticleHeightHalf, 1.0f);
			pVertex[iVertexAmount + 2].cCoords = pTemplate->cVertex[2].cCoords;
			pVertex[iVertexAmount + 2].cColor = c;
			pVertex[iVertexAmount + 2].cVertex = p->vPosition + Vector3f(-fParticleWidhtHalf, fParticleHeightHalf, 1.0f);

			pVertex[iVertexAmount + 3].cCoords = pTemplate->cVertex[1].cCoords;
			pVertex[iVertexAmount + 3].cColor = c;
			pVertex[iVertexAmount + 3].cVertex = p->vPosition + Vector3f(fParticleWidhtHalf, -fParticleHeightHalf, 1.0f);
			pVertex[iVertexAmount + 4].cCoords = pTemplate->cVertex[2].cCoords;
			pVertex[iVertexAmount + 4].cColor = c;
			pVertex[iVertexAmount + 4].cVertex = p->vPosition + Vector3f(-fParticleWidhtHalf, fParticleHeightHalf, 1.0f);
			pVertex[iVertexAmount + 5].cCoords = pTemplate->cVertex[3].cCoords;
			pVertex[iVertexAmount + 5].cColor = c;
			pVertex[iVertexAmount + 5].cVertex = p->vPosition + Vector3f(fParticleWidhtHalf, fParticleHeightHalf, 1.0f);
		}

		iVertexAmount += 6;
	}

	cVertexBuffer.SetVertexData(pVertex, iVertexAmount);
	vBoundingBox = Vector3f(rBoundingBox.Width(), rBoundingBox.Height(), 1.0f);
}

void ParticleEmitter::Render(const Matrix4f &worldTransform)
{
	if (bEnabled && arParticles)
	{
		ePacketFlags flags = FlagNone;//static_cast<ePacketFlags>((pConfiguration->bDebugSprite ? FlagWireframe : FlagNone));
		SEED_ASSERT(pTexture);
		RendererPacket packet;
		packet.nMeshType = Seed::Triangles;
		packet.pVertexBuffer = &cVertexBuffer;
		packet.pTexture = pTexture;
		packet.nBlendMode = eBlendOperation;
		packet.pTransform = &worldTransform;
		packet.cColor = cColor;
		packet.iFlags = flags;
		packet.vPivot = vTransformedPivot;

		pRendererDevice->UploadData(&packet);

//		{
//			Color p;
//			p.r = 255;
//			p.g = 255;
//			p.b = 0;
//			p.a = 255;
//			pRendererDevice->DrawCircle(rBoundingBox.x1 + rBoundingBox.Width() / 2, rBoundingBox.y1 + rBoundingBox.Height() / 2, 10.0f, p);
//		}

//		//if (pConfiguration->bDebugSprite)
//		{
//			Color p;
//			p.r = 255;
//			p.g = 0;
//			p.b = 0;
//			p.a = 255;
//			pRendererDevice->DrawRect(rBoundingBox.x1, rBoundingBox.y1, rBoundingBox.x2, rBoundingBox.y2, p);
//		}

//		for (u32 i = 0; i < iParticlesAmount * 6; i++)
//		{
//			Color p;
//			p.r = 255;
//			p.g = 0;
//			p.b = 255;
//			p.a = 255;
//			pRendererDevice->DrawCircle(pVertexData[i].cVertex.getX(), pVertexData[i].cVertex.getY(), 2.0f, p);
//		}
	}
}

void ParticleEmitter::SetSprite(const String &filename)
{
	sSprite = filename;
	File f(filename);
	Reader r(f);

	if (pTemplate)
		Delete(pTemplate);

	pTemplate = New(Sprite);
	pTemplate->Load(r, pRes);
	pTemplate->SetAnimation(iAnimation);
	pTexture = pTemplate->GetTexture();

	fParticleWidhtHalf = pTemplate->GetWidth() / 2.0f;
	fParticleHeightHalf = pTemplate->GetHeight() / 2.0f;
}

void ParticleEmitter::SetAnimation(u32 anim)
{
	iAnimation = anim;
	pTemplate->SetAnimation(anim);
	pTexture = pTemplate->GetTexture();
}

void ParticleEmitter::Play()
{
	if (bEnabled)
	{
		fInterval = cEmitter.fInterval;

		if (!bPaused)
		{
			vPrevLocation = vPos;
			if (cEmitter.fLifetime == -1.0f)
				fAge = -1.0f;
			else
				fAge = 0.0f;

			fRespawnAge = 0.0f;
		}
		else
		{
			bPaused = false;
		}
	}
}

bool ParticleEmitter::IsPlaying() const
{
	return !(this->IsStopped() || this->IsPaused());
}

void ParticleEmitter::Stop()
{
	fRespawnAge = 0.0f;
	fAge = -2.0f;
	fInterval = 0.0f;
	bPaused = false;
}

bool ParticleEmitter::IsStopped() const
{
	return (fAge == -2.0f);
}

void ParticleEmitter::Pause()
{
	bPaused = true;
}

bool ParticleEmitter::IsPaused() const
{
	return bPaused;
}

void ParticleEmitter::Kill()
{
	fRespawnAge = 0.0f;
	fAge = -2.0f;
	bPaused = false;
}

void ParticleEmitter::Enable()
{
	bEnabled = true;
}

void ParticleEmitter::Disable()
{
	bEnabled = false;
}

bool ParticleEmitter::IsEnabled() const
{
	return bEnabled;
}

void ParticleEmitter::SetFilter(eTextureFilterType type, eTextureFilter filter)
{
	if (type == Seed::TextureFilterTypeMin)
	{
		nMinFilter = filter;
	}
	else if (type == Seed::TextureFilterTypeMag)
	{
		nMagFilter = filter;
	}
}

void ParticleEmitter::MoveEverything(const Vector3f &pos)
{
	Vector3f dpos = pos - vPrevLocation;
	vPrevLocation = pos;

	for (u32 i = 0; i < iParticlesAmount; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].vPosition += dpos;
	}
}

void ParticleEmitter::SetParticlesFollowEmitter(bool bFollow)
{
	bParticlesFollowEmitter = bFollow;
}

const EmitterConfiguration &ParticleEmitter::GetConfig() const
{
	return cEmitter;
}

bool ParticleEmitter::Load(Reader &reader, ResourceManager *res)
{
	this->Unload();

	if (bEnabled)
	{
		SEED_ASSERT(res);
		pRes = res;

		ITransformable::Unserialize(reader);
		IRenderable::Unserialize(reader);

		cEmitter.bRelative = reader.ReadBool("bRelative", false);
		bAutoPlay = reader.ReadBool("bAutoPlay", false);

		cEmitter.fLifetime			= reader.ReadF32("fLifetime", 0.0f);
		cEmitter.fParticleLifeMin	= reader.ReadF32("fParticleLifeMin", 0.0f);
		cEmitter.fParticleLifeMax	= reader.ReadF32("fParticleLifeMax", 0.0f);
		cEmitter.fDirection			= reader.ReadF32("fDirection", 0.0f);
		cEmitter.fSpread			= reader.ReadF32("fSpread", 0.0f);
		cEmitter.fSpeedMin			= reader.ReadF32("fSpeedMin", 0.0f);
		cEmitter.fSpeedMax			= reader.ReadF32("fSpeedMax", 0.0f);
		cEmitter.fGravityMin		= reader.ReadF32("fGravityMin", 0.0f);
		cEmitter.fGravityMax		= reader.ReadF32("fGravityMax", 0.0f);
		cEmitter.fRadialAccelMin	= reader.ReadF32("fRadialAccelMin", 0.0f);
		cEmitter.fRadialAccelMax	= reader.ReadF32("fRadialAccelMax", 0.0f);
		cEmitter.fTangentialAccelMin	= reader.ReadF32("fTangentialAccelMin", 0.0f);
		cEmitter.fTangentialAccelMax	= reader.ReadF32("fTangentialAccelMax", 0.0f);
		cEmitter.fSizeStart			= reader.ReadF32("fSizeStart", 0.0f);
		cEmitter.fSizeEnd			= reader.ReadF32("fSizeEnd", 0.0f);
		cEmitter.fSizeVar			= reader.ReadF32("fSizeVar", 0.0f);
		cEmitter.fSpinStart			= reader.ReadF32("fSpinStart", 0.0f);
		cEmitter.fSpinEnd			= reader.ReadF32("fSpinEnd", 0.0f);
		cEmitter.fSpinVar			= reader.ReadF32("fSpinVar", 0.0f);
		cEmitter.fColorVar			= reader.ReadF32("fColorVar", 0.0f);
		cEmitter.fAlphaVar			= reader.ReadF32("fAlphaVar", 0.0f);
		cEmitter.fWidth				= reader.ReadF32("fWidth", 0.0f);
		cEmitter.fHeight			= reader.ReadF32("fHeight", 0.0f);
		cEmitter.fInterval			= reader.ReadF32("fInterval", 0.0f);

		if (reader.SelectNode("cColorStart"))
		{
			cEmitter.fColorStartR = (reader.ReadU32("r", 255)) / 255.f;
			cEmitter.fColorStartG = (reader.ReadU32("g", 255)) / 255.f;
			cEmitter.fColorStartB = (reader.ReadU32("b", 255)) / 255.f;
			cEmitter.fColorStartA = (reader.ReadU32("a", 255)) / 255.f;
			reader.UnselectNode();
		}

		if (reader.SelectNode("cColorEnd"))
		{
			cEmitter.fColorEndR = (reader.ReadU32("r", 255)) / 255.f;
			cEmitter.fColorEndG = (reader.ReadU32("g", 255)) / 255.f;
			cEmitter.fColorEndB = (reader.ReadU32("b", 255)) / 255.f;
			cEmitter.fColorEndA = (reader.ReadU32("a", 255)) / 255.f;
			reader.UnselectNode();
		}

		iAnimation = reader.ReadU32("iAnimation", 0);
		cEmitter.iEmission = reader.ReadU32("iEmission", 0);
		sBlending = reader.ReadString("sBlending", "None");
		sName = reader.ReadString("sName", "");
		sSprite = reader.ReadString("sSprite", "");
		this->SetSprite(sSprite);

		fInterval = cEmitter.fInterval;

		SEED_ASSERT_MSG(cEmitter.iEmission, "iEmission must be greater than 0.");
		iParticlesAmount = cEmitter.iEmission;
		arParticles = NewArray(Particle, iParticlesAmount);
		pVertex = (sVertex *)Alloc(sizeof(sVertex) * iParticlesAmount * 6);
		bInitialized = true;

		if (bAutoPlay)
			this->Play();
	}

	return bInitialized;
}

bool ParticleEmitter::Write(Writer &writer)
{
	writer.OpenNode();
		writer.WriteString("sType", this->GetObjectName().c_str());
		writer.WriteString("sName", sName.c_str());
		writer.WriteString("sSprite", sSprite.c_str());

		ITransformable::Serialize(writer);
//		IRenderable::Serialize(writer);

		writer.WriteString("sBlending", sBlending.c_str());
		writer.OpenNode("cColorStart");
			writer.WriteU32("r", static_cast<u8>(cEmitter.fColorStartR * 255.0f));
			writer.WriteU32("g", static_cast<u8>(cEmitter.fColorStartG * 255.0f));
			writer.WriteU32("b", static_cast<u8>(cEmitter.fColorStartB * 255.0f));
			writer.WriteU32("a", static_cast<u8>(cEmitter.fColorStartA * 255.0f));
		writer.CloseNode();

		writer.OpenNode("cColorEnd");
			writer.WriteU32("r", static_cast<u8>(cEmitter.fColorEndR * 255.0f));
			writer.WriteU32("g", static_cast<u8>(cEmitter.fColorEndG * 255.0f));
			writer.WriteU32("b", static_cast<u8>(cEmitter.fColorEndB * 255.0f));
			writer.WriteU32("a", static_cast<u8>(cEmitter.fColorEndA * 255.0f));
		writer.CloseNode();

		writer.WriteF32("fLifetime", cEmitter.fLifetime);
		writer.WriteF32("fParticleLifeMin", cEmitter.fParticleLifeMin);
		writer.WriteF32("fParticleLifeMax", cEmitter.fParticleLifeMax);
		writer.WriteF32("fDirection", cEmitter.fDirection);
		writer.WriteF32("fSpread", cEmitter.fSpread);
		writer.WriteF32("fSpeedMin", cEmitter.fSpeedMin);
		writer.WriteF32("fSpeedMax", cEmitter.fSpeedMax);
		writer.WriteF32("fGravityMin", cEmitter.fGravityMin);
		writer.WriteF32("fGravityMax", cEmitter.fGravityMax);
		writer.WriteF32("fRadialAccelMin", cEmitter.fRadialAccelMin);
		writer.WriteF32("fRadialAccelMax", cEmitter.fRadialAccelMax);
		writer.WriteF32("fTangentialAccelMin", cEmitter.fTangentialAccelMin);
		writer.WriteF32("fTangentialAccelMax", cEmitter.fTangentialAccelMax);
		writer.WriteF32("fSizeStart", cEmitter.fSizeStart);
		writer.WriteF32("fSizeEnd", cEmitter.fSizeEnd);
		writer.WriteF32("fSizeVar", cEmitter.fSizeVar);
		writer.WriteF32("fSpinStart", cEmitter.fSpinStart);
		writer.WriteF32("fSpinEnd", cEmitter.fSpinEnd);
		writer.WriteF32("fSpinVar", cEmitter.fSpinVar);
		writer.WriteF32("fColorVar", cEmitter.fColorVar);
		writer.WriteF32("fAlphaVar", cEmitter.fAlphaVar);
		writer.WriteF32("fWidth", cEmitter.fWidth);
		writer.WriteF32("fHeight", cEmitter.fHeight);
		writer.WriteF32("fInterval", cEmitter.fInterval);

		writer.WriteBool("bRelative", cEmitter.bRelative);
		writer.WriteBool("bAutoPlay", bAutoPlay);

		writer.WriteU32("iAnimation", iAnimation);
		writer.WriteU32("iEmission", cEmitter.iEmission);
	writer.CloseNode();

	return true;
}

const String ParticleEmitter::GetObjectName() const
{
	return "ParticleEmitter";
}

int ParticleEmitter::GetObjectType() const
{
	return Seed::TypeParticleEmitter;
}

} // namespace
