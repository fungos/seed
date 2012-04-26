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
#include "Particle.h"
#include "MathUtil.h"
#include "RendererDevice.h"

namespace Seed {

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
	, iAnimation(0)
	, iParticlesAmount(0)
	, nMinFilter(TextureFilterLinear)
	, nMagFilter(TextureFilterLinear)
	, bParticlesFollowEmitter(false)
	, bPaused(false)
	, bEnabled(true)
{
}

ParticleEmitter::~ParticleEmitter()
{
	this->Unload();
}

bool ParticleEmitter::Unload()
{
	if (arParticles)
	{
		for (u32 i = 0; i < iParticlesAmount; i++)
		{
			if (!arParticles[i].bActive)
				continue;

			arParticles[i].Unload();
			arParticles[i].bActive = false;
		}
	}

	iParticlesAmount = 0;
	DeleteArray(arParticles);
	Delete(pTemplate);

	fInterval = 0.0f;
	iAnimation = 0;

	memset(&cEmitter,'\0', sizeof(cEmitter));

//	if (arCurrentVertexData)
//		Free(arCurrentVertexData);

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

	for (u32 i = 0; i < iParticlesAmount; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].Unload();
		arParticles[i].bActive = false;
	}
}

void ParticleEmitter::Update(f32 deltaTime)
{
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
			arParticles[i].Unload();
			arParticles[i].bActive = false;
			continue;
		}

		accel = par->vPos - location;
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

		par->SetColor(par->fColorR, par->fColorG, par->fColorB, par->fColorA);
		par->vScale.setX(par->fSize);
		par->vScale.setY(par->fSize);
		par->fRotation += par->fSpin;

		par->vPos += (par->vVelocity * deltaTime);
		bTransformationChanged = true;
		par->bTransformationChanged = true;
		par->bChanged = true;
		rBoundingBox.Encapsulate(par->vPos.getX(), par->vPos.getY());
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
					arParticles[j] = *pTemplate;
					par = &arParticles[j];

					par->bActive = true;
					par->SetVisible(true);

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

			// Update particle sprite
			par->SetColor(par->fColorR, par->fColorG, par->fColorB, par->fColorA);
			par->SetBlendingByName(sBlending);
			par->vScale.setX(par->fSize);
			par->vScale.setY(par->fSize);
			par->vPos.setX(pos.getX());
			par->vPos.setY(pos.getY());
			par->bTransformationChanged = true;
			par->bColorChanged = true;
			rBoundingBox.Encapsulate(par->vPos.getX(), par->vPos.getY());
		}
	}

	rBoundingBox.x2++;
	rBoundingBox.y2++;

	ITexture *img = NULL;
	if (arParticles)
		 img = arParticles[0].GetTexture();

	if (img)
	{
		img->SetFilter(TextureFilterTypeMag, nMagFilter);
		img->SetFilter(TextureFilterTypeMin, nMinFilter);
	}

	if (bParticlesFollowEmitter)
		MoveEverything(vPos);

	vPrevLocation = location;
	bTransformationChanged = false;

	iNumVertices = 0;
	for (u32 i = 0; i < iParticlesAmount; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].Update(deltaTime);
//		memcpy(arCurrentVertexData, arParticles[i].arCurrentVertexData, sizeof(sVertex) * 4);
//		iNumVertices += 4;
//		pTexture = arParticles[i].pFrameTexture;
	}
}

void ParticleEmitter::Render()
{
	if (bEnabled && arParticles)
	{
		for (u32 i = 0; i < iParticlesAmount; i++)
		{
			if (!arParticles[i].bActive)
				continue;

			arParticles[i].Render();
		}

		//if (pConfiguration->bDebugSprite)
		{
			uPixel p;
			p.rgba.r = 255;
			p.rgba.g = 0;
			p.rgba.b = 0;
			p.rgba.r = 255;

			f32 x = this->GetX();
			f32 y = this->GetY();
			pRendererDevice->DrawRect(x + rBoundingBox.x1, y + rBoundingBox.y1, x + rBoundingBox.x2, y + rBoundingBox.y2, p);
		}

//		ePacketFlags flags = static_cast<ePacketFlags>((pConfiguration->bDebugSprite ? FlagWireframe : FlagNone));

//		SEED_ASSERT(pTexture);

//		RendererPacket packet;
//		packet.iSize = iNumVertices;
//		packet.nMeshType = nMeshType;
//		packet.pVertexData = arCurrentVertexData;
//		packet.pTexture = pTexture;
//		packet.nBlendMode = eBlendOperation;
//		packet.pTransform = &mTransform;
//		packet.iColor = iColor;
//		packet.iFlags = flags;
//		packet.vPivot = vTransformedPivot;

//		pRendererDevice->UploadData(&packet);
	}
}

void ParticleEmitter::SetSprite(const String &filename)
{
	if (bEnabled)
	{
		sSprite = filename;
		File f(filename);
		Reader r(f);

		if (pTemplate)
			Delete(pTemplate);

		pTemplate->Load(r, pRes);
		pTemplate->SetParent(this);

		for (u32 i = 0; i < iParticlesAmount; i++)
		{
			if (!arParticles[i].bActive)
				continue;

			arParticles[i] = *pTemplate;
		}
	}
}

void ParticleEmitter::SetAnimation(u32 anim)
{
	iAnimation = anim;
	for (u32 i = 0; i < iParticlesAmount; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].SetAnimation(anim);
	}
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

	for (u32 i = 0; i < iParticlesAmount; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].Unload();
		arParticles[i].bActive = false;
	}
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

		arParticles[i].AddPosition(dpos);
	}
}

void ParticleEmitter::SetParticlesFollowEmitter(bool bFollow)
{
	this->bParticlesFollowEmitter = bFollow;
}

const EmitterConfiguration &ParticleEmitter::GetConfig() const
{
	return cEmitter;
}

bool ParticleEmitter::Load(Reader &reader, ResourceManager *res)
{
	bool ret = false;

	this->Unload();

	if (bEnabled)
	{
		SEED_ASSERT(res);
		pRes = res;

		cEmitter.bRelative = reader.ReadBool("relative", false);

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

		if (reader.SelectNode("sColorStart"))
		{
			cEmitter.fColorStartR = (reader.ReadU32("r", 255)) / 255.f;
			cEmitter.fColorStartG = (reader.ReadU32("g", 255)) / 255.f;
			cEmitter.fColorStartB = (reader.ReadU32("b", 255)) / 255.f;
			cEmitter.fColorStartA = (reader.ReadU32("a", 255)) / 255.f;
			reader.UnselectNode();
		}

		if (reader.SelectNode("sColorEnd"))
		{
			cEmitter.fColorEndR = (reader.ReadU32("r", 255)) / 255.f;
			cEmitter.fColorEndG = (reader.ReadU32("g", 255)) / 255.f;
			cEmitter.fColorEndB = (reader.ReadU32("b", 255)) / 255.f;
			cEmitter.fColorEndA = (reader.ReadU32("a", 255)) / 255.f;
			reader.UnselectNode();
		}

		iAnimation = reader.ReadU32("iAnimation", 0);
		cEmitter.iEmission = reader.ReadU32("iEmission", 0);
		sBlending = reader.ReadString("blending", "None");
		sSprite = reader.ReadString("sSprite", "");
		{
			File f(sSprite);
			Reader r(f);
			pTemplate = New(Particle);
			pTemplate->Load(r, pRes);
			pTemplate->SetAnimation(iAnimation);
			pTemplate->SetParent(this);
		}

		pTexture = pTemplate->GetTexture();
		fInterval = cEmitter.fInterval;

		arParticles = NewArray(Particle, cEmitter.iEmission);
		iParticlesAmount = cEmitter.iEmission;

		ret = true;
	}

	return ret;
}

bool ParticleEmitter::Write(Writer &writer)
{
	bool ret = false;
	#warning "Implement ParticleEmitter writer"
	return ret;
}

const String ParticleEmitter::GetObjectName() const
{
	return "ParticleEmitter";
}

int ParticleEmitter::GetObjectType() const
{
	return Seed::ObjectParticleEmitter;
}

} // namespace
