#ifndef _SKEL_SAMPLE_H
#define _SKEL_SAMPLE_H

#include <Seed.h>
using namespace Seed;

class SkelSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener
{
	SEED_DISABLE_COPY(SkelSample)

	public:
		SkelSample();
		virtual ~SkelSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;
};

#endif // _SKEL_SAMPLE_H
