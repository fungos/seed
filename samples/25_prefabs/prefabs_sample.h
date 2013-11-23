#ifndef _PREFABSSAMPLE_H_
#define _PREFABSSAMPLE_H_

#include <Seed.h>
using namespace Seed;

class PrefabsSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener
{
	SEED_DISABLE_COPY(PrefabsSample)

	public:
		PrefabsSample();
		virtual ~PrefabsSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

	protected:
		Presentation cPres;
		ParticleEmitter *pEmitter;
		Sprite *pSprite;
		s32	iAnimation;
};

#endif // _PREFABSSAMPLE_H_
