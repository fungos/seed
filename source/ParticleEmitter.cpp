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
#include "RendererDevice.h"
#include "Sprite.h"
#include "JobManager.h"
#include "File.h"
#include "Memory.h"
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>

namespace Seed {

ISceneObject *FactoryParticleEmitter()
{
	return sdNew(ParticleEmitter);
}

ParticleEmitter::ParticleEmitter()
	: arParticles(nullptr)
	, pTexture(nullptr)
	, cEmitter()
	, pTemplate(nullptr)
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
	, nMinFilter(eTextureFilter::Linear)
	, nMagFilter(eTextureFilter::Linear)
	, cVertexBuffer()
	, pVertex(nullptr)
	, iVertexAmount(0)
	, bParticlesFollowEmitter(false)
	, bPaused(false)
	, bEnabled(true)
	, bAutoPlay(false)
	, bInitialized(false)
{
	cVertexBuffer.Configure(eBufferUsage::EveryFrameChange);
}

ParticleEmitter::~ParticleEmitter()
{
	pRendererDevice->DestroyHardwareBuffer(&cVertexBuffer);
	this->Unload();
}

bool ParticleEmitter::Unload()
{
	iParticlesAmount = 0;
	// template, vertex, particles
	sdDelete(pTemplate);
	sdFree(pVertex);
	sdDeleteArray(arParticles);

	fInterval = 0.0f;
	iAnimation = 0;
	bInitialized = false;
	bAutoPlay = false;
	bEnabled = true;
	bPaused = false;

	memset(&cEmitter,'\0', sizeof(cEmitter));
	sName = this->GetTypeName();

	return true;
}

ParticleEmitter *ParticleEmitter::Clone() const
{
	auto obj = sdNew(ParticleEmitter);

	memcpy(&obj->cEmitter, &cEmitter, sizeof(cEmitter));

	auto size = sizeof(sVertex) * iParticlesAmount * 6;
	// particles, vertex, template
	obj->arParticles = sdNewArray(Particle, iParticlesAmount);
	obj->pVertex = static_cast<sVertex *>(sdAlloc(size));
	if (pTemplate)
		obj->pTemplate = pTemplate->Clone();
	memcpy(obj->pVertex, pVertex, size);

	obj->GenerateCloneName(sName);

	sdAcquire(pTexture);
	obj->pTexture = pTexture;

	obj->cVertexBuffer.Configure(eBufferUsage::EveryFrameChange);
	obj->cVertexBuffer.SetData(obj->pVertex, iVertexAmount);

	obj->iVertexAmount = iVertexAmount;
	obj->pRes = pRes;

	obj->sSprite = sSprite;
	obj->sBlending = sBlending;
	obj->vPrevLocation = vPrevLocation;
	obj->rBoundingBox = rBoundingBox;

	obj->fAge = fAge;
	obj->fRespawnAge = fRespawnAge;
	obj->fEmissionResidue = fEmissionResidue;
	obj->fInterval = fInterval;
	obj->fTx = fTx;
	obj->fTy = fTy;
	obj->fScale = fScale;

	obj->fParticleWidhtHalf = fParticleWidhtHalf;
	obj->fParticleHeightHalf = fParticleHeightHalf;
	obj->iAnimation = iAnimation;
	obj->iParticlesAmount = iParticlesAmount;

	obj->nMinFilter = nMinFilter;
	obj->nMagFilter = nMagFilter;

	obj->bParticlesFollowEmitter = bParticlesFollowEmitter;
	obj->bPaused = bPaused;
	obj->bEnabled = bEnabled;
	obj->bAutoPlay = bAutoPlay;
	obj->bInitialized = bInitialized;

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

void ParticleEmitter::Reset()
{
	ITransformable::Reset();
	vPrevLocation = vec3();
	fTx = 0.0f;
	fTy = 0.0f;
	fScale = 1.0f;
	fEmissionResidue = 0.0f;
	fAge = -2.0f;
	fRespawnAge = 0.0f;
	bPaused = false;
	bInitialized = false;
}

void ParticleEmitter::Update(Seconds dt)
{
	if (!bInitialized)
		return;

	if (!(bEnabled && !bPaused))
		return;

	rBoundingBox.x1 = 99999.0f;
	rBoundingBox.y1 = 99999.0f;
	rBoundingBox.x2 = -99999.0f;
	rBoundingBox.y2 =  -99999.0f;

	auto ang = 0.0f;
	auto par = static_cast<Particle *>(nullptr);
	auto accel = vec3{0.0f, 0.0f, 0.0f};
	auto accel2 = vec3{0.0f, 0.0f, 0.0f};
	auto location = this->GetPosition();

	if (fAge == -2.0f && cEmitter.fLifetime != -1.0f && fInterval > 0.0f)
	{
		fRespawnAge += dt;

		if (fRespawnAge >= fInterval)
		{
			fAge = 0.0f;
			fRespawnAge = 0.0f;
		}
	}

	if (fAge >= 0)
	{
		fAge += dt;
		if (fAge >= cEmitter.fLifetime)
			fAge = -2.0f;
	}

	for (decltype(iParticlesAmount) i = 0; i < iParticlesAmount; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		par = &arParticles[i];

		par->fAge += dt;
		if (par->fAge >= par->fTerminalAge)
		{
			arParticles[i].bActive = false;
			continue;
		}

		accel = par->vPosition - location;
		accel = glm::normalize(accel);
		accel2 = accel;
		accel *= par->fRadialAccel;

		ang = accel2.x;
		accel2.x = -accel2.y;
		accel2.y = ang;

		accel2 *= par->fTangentialAccel;
		par->vVelocity += (accel + accel2) * dt;
		par->vVelocity.y = par->vVelocity.y + par->fGravity * dt;

		par->fSpin += par->fSpinDelta * dt;
		par->fSize += par->fSizeDelta * dt;
		par->fColorR += par->fColorDeltaR * dt;
		par->fColorG += par->fColorDeltaG * dt;
		par->fColorB += par->fColorDeltaB * dt;
		par->fColorA += par->fColorDeltaA * dt;

		par->vScale.x = par->vScale.y = par->fSize;
		par->fRotation += par->fSpin;

		par->vPosition += (par->vVelocity * dt);
		rBoundingBox.Encapsulate(par->vPosition.x, par->vPosition.y);
	}

	// Create more particles
	if (fAge != -2.0f)
	{
		auto fParticlesNeeded = cEmitter.iEmission * dt + fEmissionResidue;
		auto iParticlesCreated = static_cast<u32>(Number::Ceil(fParticlesNeeded));
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

			auto pos = vPrevLocation + (location - vPrevLocation) * pRand->Get(0.0f, 1.0f);

			pos.x = pos.x + pRand->Get(cEmitter.fWidth) - cEmitter.fWidth / 2.0f;
			pos.y = pos.y + pRand->Get(cEmitter.fHeight) - cEmitter.fHeight / 2.0f;

			ang = cEmitter.fDirection - glm::half_pi<f32>() + pRand->Get(0, cEmitter.fSpread) - cEmitter.fSpread / 2.0f;
			if (cEmitter.bRelative)
			{
				auto v = vPrevLocation - location;
				ang += atan2f(v.y, v.x) + glm::half_pi<f32>();
			}

			par->vVelocity.x = cosf(ang);
			par->vVelocity.y = sinf(ang);
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

			par->vScale.x = par->vScale.y = par->fSize;
			par->vPosition.x = pos.x;
			par->vPosition.y = pos.y;
			rBoundingBox.Encapsulate(par->vPosition.x, par->vPosition.y);
		}
	}

	rBoundingBox.x2++;
	rBoundingBox.y2++;
	vBoundingBox = vec3{rBoundingBox.Width(), rBoundingBox.Height(), 1.0f};

	if (bParticlesFollowEmitter)
		MoveEverything(vPos);

	vPrevLocation = location;
	this->UpdateTransform();

	if (pTemplate)
	{
		pTemplate->Update(dt);
		pTexture = pTemplate->GetTexture();
		if (pTexture)
		{
			pTexture->SetFilter(eTextureFilterType::Mag, nMagFilter);
			pTexture->SetFilter(eTextureFilterType::Min, nMinFilter);
		}

		memset(pVertex, '\0', sizeof(sVertex) * 6 * iParticlesAmount);
		iVertexAmount = 0;
		for (u32 i = 0; i < iParticlesAmount; i++)
		{
			if (!arParticles[i].bActive)
				continue;

			auto p = &arParticles[i];
			auto c = Color{u8(p->fColorR * 255), u8(p->fColorG * 255), u8(p->fColorB * 255), u8(p->fColorA * 255)};
			{
				pVertex[iVertexAmount + 0].cCoords = pTemplate->cVertex[0].cCoords;
				pVertex[iVertexAmount + 0].cColor = c;
				pVertex[iVertexAmount + 0].cVertex = p->vPosition + vec3{-fParticleWidhtHalf, -fParticleHeightHalf, 1.0f};
				pVertex[iVertexAmount + 1].cCoords = pTemplate->cVertex[1].cCoords;
				pVertex[iVertexAmount + 1].cColor = c;
				pVertex[iVertexAmount + 1].cVertex = p->vPosition + vec3{fParticleWidhtHalf, -fParticleHeightHalf, 1.0f};
				pVertex[iVertexAmount + 2].cCoords = pTemplate->cVertex[2].cCoords;
				pVertex[iVertexAmount + 2].cColor = c;
				pVertex[iVertexAmount + 2].cVertex = p->vPosition + vec3{-fParticleWidhtHalf, fParticleHeightHalf, 1.0f};

				pVertex[iVertexAmount + 3].cCoords = pTemplate->cVertex[1].cCoords;
				pVertex[iVertexAmount + 3].cColor = c;
				pVertex[iVertexAmount + 3].cVertex = p->vPosition + vec3{fParticleWidhtHalf, -fParticleHeightHalf, 1.0f};
				pVertex[iVertexAmount + 4].cCoords = pTemplate->cVertex[2].cCoords;
				pVertex[iVertexAmount + 4].cColor = c;
				pVertex[iVertexAmount + 4].cVertex = p->vPosition + vec3{-fParticleWidhtHalf, fParticleHeightHalf, 1.0f};
				pVertex[iVertexAmount + 5].cCoords = pTemplate->cVertex[3].cCoords;
				pVertex[iVertexAmount + 5].cColor = c;
				pVertex[iVertexAmount + 5].cVertex = p->vPosition + vec3{fParticleWidhtHalf, fParticleHeightHalf, 1.0f};
			}

			iVertexAmount += 6;
		}

		cVertexBuffer.SetData(pVertex, iVertexAmount);
	}
}

void ParticleEmitter::Render(const mat4 &worldTransform)
{
	if (bEnabled && arParticles && pTexture)
	{
		auto flags = ePacketFlags::None;//static_cast<ePacketFlags>((pConfiguration->bDebugSprite ? FlagWireframe : FlagNone));
		RendererPacket packet;
		packet.nMeshType = eMeshType::Triangles;
		packet.pVertexBuffer = &cVertexBuffer;
		packet.pTexture = pTexture;
		packet.nBlendMode = nBlendOperation;
		packet.pTransform = &worldTransform;
		packet.cColor = cColor;
		packet.nFlags = flags;
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

	auto cb = [&](Job *self) {
		if (self->GetState() == eJobState::Completed)
		{
			auto job = static_cast<FileLoader *>(self);
			Reader r(job->pFile);

			if (pTemplate)
				sdDelete(pTemplate);

			pTemplate = sdNew(Sprite);
			pTemplate->Load(r, pRes);
			pTemplate->SetAnimation(iAnimation);
			pTemplate->Play();
			pTexture = pTemplate->GetTexture();

			fParticleWidhtHalf = pTemplate->GetWidth() / 2.0f;
			fParticleHeightHalf = pTemplate->GetHeight() / 2.0f;
		}

		sdDelete(self);
	};

	pJobManager->Add(sdNew(FileLoader(sSprite, cb)));
}

Sprite *ParticleEmitter::GetSprite() const
{
	return pTemplate;
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
	if (type == eTextureFilterType::Min)
	{
		nMinFilter = filter;
	}
	else if (type == eTextureFilterType::Mag)
	{
		nMagFilter = filter;
	}
}

void ParticleEmitter::MoveEverything(const vec3 &pos)
{
	auto dpos = pos - vPrevLocation;
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

void ParticleEmitter::Set(Reader &reader)
{
	ITransformable::Unserialize(reader);
	IRenderable::Unserialize(reader);

	cEmitter.bRelative = reader.ReadBool("bRelative", cEmitter.bRelative);
	bAutoPlay = reader.ReadBool("bAutoPlay", bAutoPlay);

	cEmitter.fLifetime			= reader.ReadF32("fLifetime", cEmitter.fLifetime);
	cEmitter.fParticleLifeMin	= reader.ReadF32("fParticleLifeMin", cEmitter.fParticleLifeMin);
	cEmitter.fParticleLifeMax	= reader.ReadF32("fParticleLifeMax", cEmitter.fParticleLifeMax);
	cEmitter.fDirection			= reader.ReadF32("fDirection", cEmitter.fDirection);
	cEmitter.fSpread			= reader.ReadF32("fSpread", cEmitter.fSpread);
	cEmitter.fSpeedMin			= reader.ReadF32("fSpeedMin", cEmitter.fSpeedMin);
	cEmitter.fSpeedMax			= reader.ReadF32("fSpeedMax", cEmitter.fSpeedMax);
	cEmitter.fGravityMin		= reader.ReadF32("fGravityMin", cEmitter.fGravityMin);
	cEmitter.fGravityMax		= reader.ReadF32("fGravityMax", cEmitter.fGravityMax);
	cEmitter.fRadialAccelMin	= reader.ReadF32("fRadialAccelMin", cEmitter.fRadialAccelMin);
	cEmitter.fRadialAccelMax	= reader.ReadF32("fRadialAccelMax", cEmitter.fRadialAccelMax);
	cEmitter.fTangentialAccelMin	= reader.ReadF32("fTangentialAccelMin", cEmitter.fTangentialAccelMin);
	cEmitter.fTangentialAccelMax	= reader.ReadF32("fTangentialAccelMax", cEmitter.fTangentialAccelMax);
	cEmitter.fSizeStart			= reader.ReadF32("fSizeStart", cEmitter.fSizeStart);
	cEmitter.fSizeEnd			= reader.ReadF32("fSizeEnd", cEmitter.fSizeEnd);
	cEmitter.fSizeVar			= reader.ReadF32("fSizeVar", cEmitter.fSizeVar);
	cEmitter.fSpinStart			= reader.ReadF32("fSpinStart", cEmitter.fSpinStart);
	cEmitter.fSpinEnd			= reader.ReadF32("fSpinEnd", cEmitter.fSpinEnd);
	cEmitter.fSpinVar			= reader.ReadF32("fSpinVar", cEmitter.fSpinVar);
	cEmitter.fColorVar			= reader.ReadF32("fColorVar", cEmitter.fColorVar);
	cEmitter.fAlphaVar			= reader.ReadF32("fAlphaVar", cEmitter.fAlphaVar);
	cEmitter.fWidth				= reader.ReadF32("fWidth", cEmitter.fWidth);
	cEmitter.fHeight			= reader.ReadF32("fHeight", cEmitter.fHeight);
	cEmitter.fInterval			= reader.ReadF32("fInterval", cEmitter.fInterval);

	if (reader.SelectNode("cColorStart"))
	{
		cEmitter.fColorStartR = (reader.ReadU32("r", u32(cEmitter.fColorStartR * 255))) / 255.f;
		cEmitter.fColorStartG = (reader.ReadU32("g", u32(cEmitter.fColorStartG * 255))) / 255.f;
		cEmitter.fColorStartB = (reader.ReadU32("b", u32(cEmitter.fColorStartB * 255))) / 255.f;
		cEmitter.fColorStartA = (reader.ReadU32("a", u32(cEmitter.fColorStartA * 255))) / 255.f;
		reader.UnselectNode();
	}

	if (reader.SelectNode("cColorEnd"))
	{
		cEmitter.fColorEndR = (reader.ReadU32("r", u32(cEmitter.fColorEndR * 255))) / 255.f;
		cEmitter.fColorEndG = (reader.ReadU32("g", u32(cEmitter.fColorEndG * 255))) / 255.f;
		cEmitter.fColorEndB = (reader.ReadU32("b", u32(cEmitter.fColorEndB * 255))) / 255.f;
		cEmitter.fColorEndA = (reader.ReadU32("a", u32(cEmitter.fColorEndA * 255))) / 255.f;
		reader.UnselectNode();
	}

	iAnimation = reader.ReadU32("iAnimation", iAnimation);
	cEmitter.iEmission = reader.ReadU32("iEmission", cEmitter.iEmission);

	sBlending = reader.ReadString("sBlending", sBlending.c_str());
	sName = reader.ReadString("sName", sName.c_str());
	auto sprite = String(reader.ReadString("sSprite", sSprite.c_str()));
	if (sSprite != sprite)
		this->SetSprite(sprite);

	fInterval = cEmitter.fInterval;

	SEED_ASSERT_FMT(cEmitter.iEmission, "ParticleEmitter '%s': iEmission must be greater than 0.", sName.c_str());
	iParticlesAmount = cEmitter.iEmission;

	// particle, vertex, template
	sdDelete(pVertex);
	sdDeleteArray(arParticles);

	arParticles = sdNewArray(Particle, iParticlesAmount);
	pVertex = (sVertex *)sdAlloc(sizeof(sVertex) * iParticlesAmount * 6);

	bInitialized = true;
	if (bEnabled && bAutoPlay)
		this->Play();
}

bool ParticleEmitter::Write(Writer &writer)
{
	if (pTemplate)
		iAnimation = pTemplate->GetCurrentAnimation();

	writer.OpenNode();
		writer.WriteString("sType", this->GetTypeName());
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

} // namespace
