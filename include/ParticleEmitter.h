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

#include "interface/ITransformable.h"
#include "interface/ISceneObject.h"

namespace Seed {

class ITexture;
class ResourceManager;
class ParticleEmitterObject;
class Particle;
struct ParticleEmitterInfo;

/// Particle Emitter
class SEED_CORE_API ParticleEmitter : public ISceneObject
{
	public:
		ParticleEmitter();
		virtual ~ParticleEmitter();

		virtual void SetSprite(const String &filename);
		virtual void SetAnimation(u32 anim);

		virtual void SetFilter(eTextureFilterType type, eTextureFilter filter);

		virtual void Load(const String &filename, ResourceManager *res = pResourceManager);
		virtual void Unload();
		virtual void Reset();

		virtual const ParticleEmitterInfo *GetEmitterInfo() const;

		// Metodos candidatos a uma interface para sistemas Animados
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
		virtual void SetParticlesBuffer(Particle *buffer, u32 amount = 0);

		// IRenderable
		virtual void Update(f32 delta);
		virtual void Render();

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(ParticleEmitter);

		void MoveEverything(const Vector3f &pos);

	private:
		ParticleEmitterObject		*pEmitterObject;
		const ParticleEmitterInfo	*pInfo;
		ResourceManager				*pRes;
		String						sFilename;
		String						sSpriteFilename;
		bool						bParticlesFollowEmitter;

		f32							fAge;
		f32							fRespawnAge;
		f32							fEmissionResidue;
		f32							fInterval;

		Vector3f					vPrevLocation;
		f32							fTx;
		f32							fTy;
		f32							fScale;
		u32							iAnimation;

		bool						bPaused;
		bool						bEnabled;

		eTextureFilter				nMinFilter;
		eTextureFilter				nMagFilter;

		u32							iParticlesAmount;
		Particle					*arParticles; //arParticles[SEED_PARTICLES_MAX];
};

} // namespace

#endif // __PARTICLE_EMITTER_H__
