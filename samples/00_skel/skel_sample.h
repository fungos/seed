#ifndef _SKEL_SAMPLE_H
#define _SKEL_SAMPLE_H

#include <Seed.h>
using namespace Seed;

class SkelSample : public IGameApp,
							public IEventSystemListener,
							public IEventInputKeyboardListener
{
	public:
		SkelSample();
		virtual ~SkelSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

	private:
		SEED_DISABLE_COPY(SkelSample);
};

#endif // _SKEL_SAMPLE_H