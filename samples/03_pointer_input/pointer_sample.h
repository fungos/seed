#ifndef _POINTERSAMPLE_H_
#define _POINTERSAMPLE_H_

#include <Seed.h>
using namespace Seed;

class PointerSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener,
					public IEventPresentationListener
{
	public:
		PointerSample();
		virtual ~PointerSample();

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
		SEED_DISABLE_COPY(PointerSample);

	protected:
		Presentation cPres;
		ISceneObject *pImage;
		Camera		*pCamera;

		f32			fElapsed;
		f32			fDir;
		bool		bRotate;

		Vector3f	vFrom;
		Vector3f	vCurrent;
		Vector3f	vTo;
};

#endif // _POINTERSAMPLE_H_
