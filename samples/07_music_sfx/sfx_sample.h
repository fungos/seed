#ifndef _SFXSAMPLE_H_
#define _SFXSAMPLE_H_

#include <Seed.h>
using namespace Seed;

class SfxSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener,
					public IEventPresentationListener
{
	SEED_DISABLE_COPY(SfxSample)

	public:
		SfxSample();
		virtual ~SfxSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventInputPointerListener
		virtual void OnInputPointerRelease(const EventInputPointer *ev);

		// IEventPresentationListener
		virtual void OnPresentationLoaded(const EventPresentation *ev);

	private:
		Presentation cPres;
		Music musTheme;
};

#endif // _SFXSAMPLE_H_
