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

#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__

#include "interface/ISceneObject.h"
#include "Rect.h"
#include "RendererDevice.h"

namespace Seed {

class ITexture;
class ResourceManager;
class Particle;
class Sprite;

ISceneObject *FactoryParticleEmitter();

/// Particle Emitter Configuration
struct SEED_CORE_API EmitterConfiguration
{
	u32		iTextureFrame;
	u32		iEmission; // particles per sec

	f32		fLifetime;

	f32		fParticleLifeMin;
	f32		fParticleLifeMax;

	f32		fDirection;
	f32		fSpread;

	f32		fSpeedMin;
	f32		fSpeedMax;

	f32		fGravityMin;
	f32		fGravityMax;

	f32		fRadialAccelMin;
	f32		fRadialAccelMax;

	f32		fTangentialAccelMin;
	f32		fTangentialAccelMax;

	f32		fSizeStart;
	f32		fSizeEnd;
	f32		fSizeVar;

	f32		fSpinStart;
	f32		fSpinEnd;
	f32		fSpinVar;

	f32		fColorStartR;
	f32		fColorStartG;
	f32		fColorStartB;
	f32		fColorStartA;

	f32		fColorEndR;
	f32		fColorEndG;
	f32		fColorEndB;
	f32		fColorEndA;

	f32		fColorVar;
	f32		fAlphaVar;

	f32		fWidth;
	f32		fHeight;

	f32		fInterval;
	bool	bRelative : 1;
};

/// Particle Emitter
class SEED_CORE_API ParticleEmitter : public ISceneObject
{
	SEED_DISABLE_COPY(ParticleEmitter)
	SEED_DECLARE_RTTI(ParticleEmitter, ISceneObject)

	public:
		ParticleEmitter();
		virtual ~ParticleEmitter();

		virtual void Reset();
		virtual void SetSprite(const String &filename);
		virtual Sprite *GetSprite() const;

		virtual void SetFilter(eTextureFilterType type, eTextureFilter filter);
		virtual const EmitterConfiguration &GetConfig() const;

		virtual void Play();
		virtual bool IsPlaying() const;

		virtual void Stop();
		virtual bool IsStopped() const;

		virtual void Pause();
		virtual bool IsPaused() const;

		virtual void Kill();

		virtual void Disable();
		virtual void Enable();
		virtual bool IsEnabled() const;

		virtual void SetParticlesFollowEmitter(bool bFollow);

		// IRenderable
		virtual void Update(Seconds dt) override;
		virtual void Render(const mat4 &worldTransform) override;

		// IDataObject
		virtual bool Write(Writer &writer) override;
		virtual bool Unload() override;
		virtual ParticleEmitter *Clone() const override;
		virtual void Set(Reader &reader) override;

	private:
		void MoveEverything(const vec3 &pos);

		Particle				*arParticles;
		ITexture				*pTexture;

		EmitterConfiguration	cEmitter;
		Sprite					*pTemplate;
		String					sSprite;
		String					sBlending;
		vec3					vPrevLocation;
		Rect4f					rBoundingBox;

		f32						fAge;
		f32						fRespawnAge;
		f32						fEmissionResidue;
		f32						fInterval;
		f32						fTx;
		f32						fTy;
		f32						fScale;

		f32						fParticleWidhtHalf;
		f32						fParticleHeightHalf;
		u32						iAnimation;
		u32						iParticlesAmount;

		eTextureFilter			nMinFilter;
		eTextureFilter			nMagFilter;

		VertexBuffer			cVertexBuffer;
		sVertex					*pVertex;
		u32						iVertexAmount;

		bool					bParticlesFollowEmitter : 1;
		bool					bPaused : 1;
		bool					bEnabled : 1;
		bool					bAutoPlay : 1;
		bool					bInitialized : 1;
};

} // namespace

#endif // __PARTICLE_EMITTER_H__
