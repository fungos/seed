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
#include "ParticleManager.h"
#include "Rand.h"
#include "Number.h"
#include "Texture.h"
#include "ParticleEmitterObject.h"
#include "SeedInit.h"
#include "Particle.h"
#include "MathUtil.h"

namespace Seed {

ParticleEmitter::ParticleEmitter()
	: pEmitterObject(NULL)
	, pInfo(NULL)
	, pRes(NULL)
	, sFilename()
	, sSpriteFilename()
	, bParticlesFollowEmitter(false)
	, fAge(0.0f)
	, fRespawnAge(0.0f)
	, fEmissionResidue(0.0f)
	, fInterval(0.0f)
	, vPrevLocation(0.0f, 0.0f, 0.0f)
	, fTx(0.0f)
	, fTy(0.0f)
	, fScale(1.0f)
	, iAnimation(0)
	, bPaused(false)
	, bEnabled(true)
	, nMinFilter(TextureFilterLinear)
	, nMagFilter(TextureFilterLinear)
	, iParticlesAmount(0)
	, arParticles(NULL)
{
}

ParticleEmitter::~ParticleEmitter()
{
	if (pParticleManager)
		pParticleManager->Remove(this);

	this->Unload();
}

void ParticleEmitter::Load(const String &filename, ResourceManager *res)
{
	this->Unload();

	if (bEnabled)
	{
		ASSERT_NULL(res);

		pRes = res;
		sFilename = filename;
		pEmitterObject = reinterpret_cast<ParticleEmitterObject *>(res->Get(filename, Seed::ObjectParticleEmitterObject));
		pInfo = pEmitterObject->GetData();
		iAnimation = pInfo->iTextureFrame;
		fInterval = pInfo->fInterval;
	}
}

void ParticleEmitter::Unload()
{
	for (u32 i = 0; i < iParticlesAmount; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].Unload();
		arParticles[i].bActive = false;
	}

	fInterval = 0.0f;
	iAnimation = 0;
	sRelease(pEmitterObject);
	sFilename	= "";
	pRes		= NULL;
}

void ParticleEmitter::Reset()
{
	ITransformable::Reset();
	vPrevLocation.x = 0.0f;
	vPrevLocation.y = 0.0f;
	vPrevLocation.z = 0.0f;
	fTx = 0.0f;
	fTy = 0.0f;
	fScale = 1.0f;
	fEmissionResidue = 0.0f;
	//iParticlesAlive = 0;
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

void ParticleEmitter::SetParticlesBuffer(Particle *buffer, u32 amount)
{
	if (buffer)
	{
		arParticles = buffer;
		iParticlesAmount = amount;
	}
	else
	{
		// we force to zero to guarantee the iteration in arParticles won't
		// happen and we won't get a segfault trying to access a particle.
		// this is why we won't get the amount parameter no matter what.
		arParticles = NULL;
		iParticlesAmount = 0;
	}
}

void ParticleEmitter::Update(f32 deltaTime)
{
	if (!(bEnabled && pEmitterObject && !bPaused))
		return;

	//deltaTime = 0.017000001f;

	//u32 i = 0;
	f32 ang = 0.0f;
	Particle *par = NULL;
	Vector3f accel(0.0f, 0.0f, 0.0f);
	Vector3f accel2(0.0f, 0.0f, 0.0f);
	Vector3f location = this->GetPosition();

	if (fAge == -2.0f && pInfo->fLifetime != -1.0f && fInterval > 0.0f)// && nParticlesAlive == 0)
	{
		fRespawnAge += deltaTime;

		if (fRespawnAge >= fInterval)
		{
			fAge = 0.0f;
			fRespawnAge = 0.0f;
		}
		//return;
	}

	if (fAge >= 0)
	{
		fAge += deltaTime;
		if (fAge >= pInfo->fLifetime)
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
			//par->SetVisible(false);

			arParticles[i].Unload();
			arParticles[i].bActive = false;

			//iParticlesAlive--;
			//MEMCOPY(par, &arParticles[iParticlesAlive], sizeof(Particle));
			//i--;
			continue;
		}

		accel = par->vPos - location;
		accel.Normalize();
		accel2 = accel;
		accel *= par->fRadialAccel;

		// vecAccel2.Rotate(M_PI_2);
		// the following is faster
		ang = accel2.x;
		accel2.x = -accel2.y;
		accel2.y = ang * 1.25f;

		accel2 *= par->fTangentialAccel;
		par->vVelocity += (accel + accel2) * deltaTime;
		par->vVelocity.y += par->fGravity * deltaTime * 1.25f;

		par->fSpin += par->fSpinDelta * deltaTime;
		par->fSize += par->fSizeDelta * deltaTime;
		par->fColorR += par->fColorDeltaR * deltaTime;
		par->fColorG += par->fColorDeltaG * deltaTime;
		par->fColorB += par->fColorDeltaB * deltaTime;
		par->fColorA += par->fColorDeltaA * deltaTime;

		par->SetColor(par->fColorR, par->fColorG, par->fColorB, par->fColorA);
		//par->SetScale(par->fSize);
		//par->AddPosition(par->vVelocity * deltaTime);
		par->vScale.x = par->fSize;
		par->vScale.y = par->fSize;
		par->fRotation += par->fSpin;
		//if (par->fRotation >= 360.0f)
		//	par->fRotation -= 360.0f;
		//if (par->fRotation < 0.0f)
		//	par->fRotation += 360.0f;

		par->vPos += (par->vVelocity * deltaTime);
		bTransformationChanged = true;
		par->bTransformationChanged = true;
		par->bChanged = true;
		//par++;
	}

	// generate new particles
	if (fAge != -2.0f)
	{
		f32 fParticlesNeeded = pInfo->iEmission * deltaTime + fEmissionResidue;
		u32 iParticlesCreated = static_cast<u32>(Number::Ceil(fParticlesNeeded));
		fEmissionResidue = fParticlesNeeded - iParticlesCreated;

		//par = &arParticles[iParticlesAlive];

		for (u32 i = 0; i < iParticlesCreated; i++)
		{
			//if (arParticles.Size() >= iParticlesAmount)
			//	break;

			bool bFull = true;
			for (u32 j = 0; j < iParticlesAmount; j++)
			{
				if (!arParticles[j].bActive)
				{
					arParticles[j].bActive = true;
					par = &arParticles[j];
					bFull = false;
					break;
				}
			}
			if (bFull)
				break;

			par->fAge = 0.0f;
			par->fTerminalAge = pRand->Get(pInfo->fParticleLifeMin, pInfo->fParticleLifeMax) * 0.70f;

			Vector3f pos = vPrevLocation + (location - vPrevLocation) * pRand->Get(0.0f, 1.0f);

			if (!pInfo->fWidth)
			{
				pos.x += pRand->Get(-0.0002f, 0.0002f);
			}
			else
			{
				pos.x += pRand->Get(pInfo->fWidth);
			}

			if (!pInfo->fHeight)
			{
				pos.y += pRand->Get(-0.0002f, 0.0002f);
			}
			else
			{
				pos.y += pRand->Get(pInfo->fHeight);
			}

			ang = pInfo->fDirection - kPiOver2 + pRand->Get(0, pInfo->fSpread) - pInfo->fSpread / 2.0f;

			if (pInfo->bRelative)
				ang += (vPrevLocation - location).Angle() + kPiOver2;

			par->vVelocity.x = cosf(ang);
			par->vVelocity.y = sinf(ang) * 1.25f;
			par->vVelocity *= pRand->Get(pInfo->fSpeedMin/300.0f, pInfo->fSpeedMax/300.0f) * 0.70f;

			par->fGravity = pRand->Get(pInfo->fGravityMin/900.0f, pInfo->fGravityMax/900.0f) * 3.0f;
			par->fRadialAccel = pRand->Get(pInfo->fRadialAccelMin/900.0f, pInfo->fRadialAccelMax/900.0f) * 4.0f;
			par->fTangentialAccel = pRand->Get(pInfo->fTangentialAccelMin/900.0f, pInfo->fTangentialAccelMax/900.0f) * 3.0f;

			par->fSize = pRand->Get(pInfo->fSizeStart, pInfo->fSizeStart + (pInfo->fSizeEnd - pInfo->fSizeStart) * pInfo->fSizeVar);
			par->fSizeDelta = (pInfo->fSizeEnd - par->fSize) / par->fTerminalAge;

			par->fSpin = pRand->Get(pInfo->fSpinStart, pInfo->fSpinStart + (pInfo->fSpinEnd - pInfo->fSpinStart) * pInfo->fSpinVar) / 15.0f;
			par->fSpinDelta = (pInfo->fSpinEnd - par->fSpin) / par->fTerminalAge;

			par->fColorR = pRand->Get(pInfo->fColorStartR, pInfo->fColorStartR + (pInfo->fColorEndR - pInfo->fColorStartR) * pInfo->fColorVar);
			par->fColorG = pRand->Get(pInfo->fColorStartG, pInfo->fColorStartG + (pInfo->fColorEndG - pInfo->fColorStartG) * pInfo->fColorVar);
			par->fColorB = pRand->Get(pInfo->fColorStartB, pInfo->fColorStartB + (pInfo->fColorEndB - pInfo->fColorStartB) * pInfo->fColorVar);
			par->fColorA = pRand->Get(pInfo->fColorStartA, pInfo->fColorStartA + (pInfo->fColorEndA - pInfo->fColorStartA) * pInfo->fAlphaVar);

			par->fColorDeltaR = (pInfo->fColorEndR - par->fColorR) / par->fTerminalAge;
			par->fColorDeltaG = (pInfo->fColorEndG - par->fColorG) / par->fTerminalAge;
			par->fColorDeltaB = (pInfo->fColorEndB - par->fColorB) / par->fTerminalAge;
			par->fColorDeltaA = (pInfo->fColorEndA - par->fColorA) / par->fTerminalAge;

			par->SetColor(par->fColorR, par->fColorG, par->fColorB, par->fColorA);
			if (pInfo->iBlendMode == 6)
				par->SetBlending(BlendModulate);
			else
				par->SetBlending(BlendAdditive);
			//par->SetScale(par->fSize);
			par->vScale.x = par->fSize;
			par->vScale.y = par->fSize;
			//par->SetPosition(pos);
			par->vPos = pos;
			//par->fRotation = par->fSpin;
			par->bTransformationChanged = true;

			File f(sSpriteFilename);
			Reader r(f);
			par->Load(r, pRes);
			par->SetAnimation(iAnimation);
			par->SetVisible(true);
			//par->SetParent(this);

			//iParticlesAlive++;
			//par++;
		}
	}

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

	for (u32 i = 0; i < iParticlesAmount; i++)
	{
		if (!arParticles[i].bActive)
			continue;

		arParticles[i].Update(deltaTime);
	}
}

void ParticleEmitter::Render()
{
	if (bEnabled && pEmitterObject)
	{
		for (u32 i = 0; i < iParticlesAmount; i++)
		{
			if (!arParticles[i].bActive)
				continue;

			arParticles[i].Render();
		}
	}
}

void ParticleEmitter::SetSprite(const String &filename)
{
	if (bEnabled && pEmitterObject)
	{
		sSpriteFilename = filename;
		for (u32 i = 0; i < iParticlesAmount; i++)
		{
			if (!arParticles[i].bActive)
				continue;

			File f(filename);
			Reader r(f);
			arParticles[i].Load(r, pRes);
			arParticles[i].SetAnimation(iAnimation);
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
	if (bEnabled && pEmitterObject)
	{
		fInterval = pInfo->fInterval;

		if (!bPaused)
		{
			vPrevLocation = vPos;
			if (pInfo->fLifetime == -1.0f)
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
//	iParticlesAlive = 0;
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

const ParticleEmitterInfo *ParticleEmitter::GetEmitterInfo() const
{
	return pInfo;
}

const char *ParticleEmitter::GetObjectName() const
{
	return "ParticleEmitter";
}

int ParticleEmitter::GetObjectType() const
{
	return Seed::ObjectParticleEmitter;
}

} // namespace
