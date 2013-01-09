#ifndef _PRESENTATION_SAMPLE_H_
#define _PRESENTATION_SAMPLE_H_

#include <Seed.h>
using namespace Seed;

class PresentationSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventPresentationListener
{
	public:
		PresentationSample();
		virtual ~PresentationSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventPresentationListener
		virtual void OnPresentationLoaded(const EventPresentation *ev);

	private:
		SEED_DISABLE_COPY(PresentationSample);

	protected:
		Presentation cPres;
};

#endif // _PRESENTATION_SAMPLE_H_
