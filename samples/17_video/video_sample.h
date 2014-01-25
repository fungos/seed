#ifndef _VIDEOSAMPLE_H_
#define _VIDEOSAMPLE_H_

#include <Seed.h>
using namespace Seed;

class VideoSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener
{
	SEED_DISABLE_COPY(VideoSample)

	public:
		VideoSample();
		virtual ~VideoSample();

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
		ISceneObject *pObject;
		Camera		*pCamera;

		Vector3f	vFrom;
		Vector3f	vCurrent;
		Vector3f	vTo;

		f32			fElapsed;
		f32			fDir;
		bool		bRotate : 1;
		bool		bLoaded : 1;
};

#endif // _VIDEOSAMPLE_H_
