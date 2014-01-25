#ifndef _PRESENTATION_SAMPLE_H_
#define _PRESENTATION_SAMPLE_H_

#include <Seed.h>
using namespace Seed;

class PresentationSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener
{
	SEED_DISABLE_COPY(PresentationSample)

	public:
		PresentationSample();
		virtual ~PresentationSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

		// IEventInputPointerListener
		virtual void OnInputPointerRelease(const EventInputPointer *ev) override;

	protected:
		Presentation cPres;
};

#endif // _PRESENTATION_SAMPLE_H_
