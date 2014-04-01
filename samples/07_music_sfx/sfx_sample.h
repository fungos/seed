#ifndef _SFXSAMPLE_H_
#define _SFXSAMPLE_H_

#include <Seed.h>
using namespace Seed;

class SfxSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener
{
	SEED_DISABLE_COPY(SfxSample)

	public:
		SfxSample();
		virtual ~SfxSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

		// IEventInputPointerListener
		virtual void OnInputPointerRelease(const EventInputPointer *ev) override;

	private:
		Presentation cPres;
		Music musTheme;
};

#endif // _SFXSAMPLE_H_
