#ifndef _PARTICLESAMPLE_H_
#define _PARTICLESAMPLE_H_

#include <Seed.h>
using namespace Seed;

class ParticleSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener
{
	SEED_DISABLE_COPY(ParticleSample)

	public:
		ParticleSample();
		virtual ~ParticleSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

	protected:
		Presentation cPres;
		Sprite *pObject;
		ParticleEmitter *pEmitter;
		s32	iAnimation;
};

#endif // _PARTICLESAMPLE_H_
