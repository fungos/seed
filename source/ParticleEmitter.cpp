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
#include "SeedInit.h"
#include "Particle.h"
#include "MathUtil.h"

namespace Seed {

ParticleEmitter::ParticleEmitter()
	: pRes(NULL)
	, cEmitter()
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

bool ParticleEmitter::Unload()
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
		arParticles = NULL;
		iParticlesAmount = 0;
	}
}

void ParticleEmitter::Update(f32 deltaTime)
{
	if (!(bEnabled && !bPaused))
		return;

	//deltaTime = 0.017000001f;

	//u32 i = 0;
	f32 ang = 0.0f;
	Particle *par = NULL;
	Vector3f accel(0.0f, 0.0f, 0.0f);
	Vector3f accel2(0.0f, 0.0f, 0.0f);
	Vector3f location = this->GetPosition();

	if (fAge == -2.0f && cEmitter.fLifetime != -1.0f && fInterval > 0.0f)// && nParticlesAlive == 0)
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
			//par->SetVisible(false);

			arParticles[i].Unload();
			arParticles[i].bActive = false;

			//iParticlesAlive--;
			//MEMCOPY(par, &arParticles[iParticlesAlive], sizeof(Particle));
			//i--;
			continue;
		}

		accel = par->vPos - location;
		accel = normalize(accel);
		accel2 = accel;
		accel *= par->fRadialAccel;

		// vecAccel2.Rotate(M_PI_2);
		// the following is faster
		ang = accel2.getX();
		accel2.setX(-accel2.getY());
		accel2.setY(ang * 1.25f);

		accel2 *= par->fTangentialAccel;
		par->vVelocity += (accel + accel2) * deltaTime;
		par->vVelocity.setY(par->vVelocity.getY() + par->fGravity * deltaTime * 1.25f);

		par->fSpin += par->fSpinDelta * deltaTime;
		par->fSize += par->fSizeDelta * deltaTime;
		par->fColorR += par->fColorDeltaR * deltaTime;
		par->fColorG += par->fColorDeltaG * deltaTime;
		par->fColorB += par->fColorDeltaB * deltaTime;
		par->fColorA += par->fColorDeltaA * deltaTime;

		par->SetColor(par->fColorR, par->fColorG, par->fColorB, par->fColorA);
		//par->SetScale(par->fSize);
		//par->AddPosition(par->vVelocity * deltaTime);
		par->vScale.setX(par->fSize);
		par->vScale.setY(par->fSize);
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
		f32 fParticlesNeeded = cEmitter.iEmission * deltaTime + fEmissionResidue;
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
			par->fTerminalAge = pRand->Get(cEmitter.fParticleLifeMin, cEmitter.fParticleLifeMax) * 0.70f;

			Vector3f pos = vPrevLocation + (location - vPrevLocation) * pRand->Get(0.0f, 1.0f);

			if (!cEmitter.fWidth)
			{
				pos.setX(pos.getX() + pRand->Get(-0.0002f, 0.0002f));
			}
			else
			{
				pos.setX(pos.getX() + pRand->Get(cEmitter.fWidth));
			}

			if (!cEmitter.fHeight)
			{
				pos.setY(pos.getY() + pRand->Get(-0.0002f, 0.0002f));
			}
			else
			{
				pos.setY(pos.getY() + pRand->Get(cEmitter.fHeight));
			}

			ang = cEmitter.fDirection - kPiOver2 + pRand->Get(0, cEmitter.fSpread) - cEmitter.fSpread / 2.0f;

			if (cEmitter.bRelative)
				ang += VectorAngle(vPrevLocation - location) + kPiOver2;

			par->vVelocity.setX(cosf(ang));
			par->vVelocity.setY(sinf(ang) * 1.25f);
			par->vVelocity *= pRand->Get(cEmitter.fSpeedMin/300.0f, cEmitter.fSpeedMax/300.0f) * 0.70f;

			par->fGravity = pRand->Get(cEmitter.fGravityMin/900.0f, cEmitter.fGravityMax/900.0f) * 3.0f;
			par->fRadialAccel = pRand->Get(cEmitter.fRadialAccelMin/900.0f, cEmitter.fRadialAccelMax/900.0f) * 4.0f;
			par->fTangentialAccel = pRand->Get(cEmitter.fTangentialAccelMin/900.0f, cEmitter.fTangentialAccelMax/900.0f) * 3.0f;

			par->fSize = pRand->Get(cEmitter.fSizeStart, cEmitter.fSizeStart + (cEmitter.fSizeEnd - cEmitter.fSizeStart) * cEmitter.fSizeVar);
			par->fSizeDelta = (cEmitter.fSizeEnd - par->fSize) / par->fTerminalAge;

			par->fSpin = pRand->Get(cEmitter.fSpinStart, cEmitter.fSpinStart + (cEmitter.fSpinEnd - cEmitter.fSpinStart) * cEmitter.fSpinVar) / 15.0f;
			par->fSpinDelta = (cEmitter.fSpinEnd - par->fSpin) / par->fTerminalAge;

			par->fColorR = pRand->Get(cEmitter.fColorStartR, cEmitter.fColorStartR + (cEmitter.fColorEndR - cEmitter.fColorStartR) * cEmitter.fColorVar);
			par->fColorG = pRand->Get(cEmitter.fColorStartG, cEmitter.fColorStartG + (cEmitter.fColorEndG - cEmitter.fColorStartG) * cEmitter.fColorVar);
			par->fColorB = pRand->Get(cEmitter.fColorStartB, cEmitter.fColorStartB + (cEmitter.fColorEndB - cEmitter.fColorStartB) * cEmitter.fColorVar);
			par->fColorA = pRand->Get(cEmitter.fColorStartA, cEmitter.fColorStartA + (cEmitter.fColorEndA - cEmitter.fColorStartA) * cEmitter.fAlphaVar);

			par->fColorDeltaR = (cEmitter.fColorEndR - par->fColorR) / par->fTerminalAge;
			par->fColorDeltaG = (cEmitter.fColorEndG - par->fColorG) / par->fTerminalAge;
			par->fColorDeltaB = (cEmitter.fColorEndB - par->fColorB) / par->fTerminalAge;
			par->fColorDeltaA = (cEmitter.fColorEndA - par->fColorA) / par->fTerminalAge;

			par->SetColor(par->fColorR, par->fColorG, par->fColorB, par->fColorA);
			if (cEmitter.iBlendMode == 6)
				par->SetBlending(BlendModulate);
			else
				par->SetBlending(BlendAdditive);
			//par->SetScale(par->fSize);
			par->vScale.setX(par->fSize);
			par->vScale.setY(par->fSize);
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
	if (bEnabled)
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
	if (bEnabled)
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

		// Json Load into cEmitter
		#warning "Implement ParticleEmitter reader"

		iAnimation = cEmitter.iTextureFrame;
		fInterval = cEmitter.fInterval;

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
