#ifndef _SAVESAMPLE_H_
#define _SAVESAMPLE_H_

#include <Seed.h>
using namespace Seed;

class SaveSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener,
					public IEventPresentationListener
{
	SEED_DISABLE_COPY(SaveSample)

	public:
		SaveSample();
		virtual ~SaveSample();

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
		bool SaveSystemFlow() const;

	protected:
		Presentation cPres;
		ISceneObject *pImage;
		Camera		*pCamera;

		Vector3f	vFrom;
		Vector3f	vCurrent;
		Vector3f	vTo;

		f32			fElapsed;
		f32			fDir;
		bool		bRotate : 1;
};

#endif // _SAVESAMPLE_H_
