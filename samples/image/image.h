#ifndef _IMAGESAMPLE_H_
#define _IMAGESAMPLE_H_

#include <Seed.h>
using namespace Seed;

extern SceneNode *gScene;

class ImageSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener,
					public IEventJobListener
{
	public:
		ImageSample();
		virtual ~ImageSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Reset();
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventInputPointerListener
		virtual void OnInputPointerRelease(const EventInputPointer *ev);

		// IEventJobListener
		virtual void OnJobCompleted(const EventJob *ev);
		virtual void OnJobAborted(const EventJob *ev);

	private:
		SEED_DISABLE_COPY(ImageSample);

	protected:
		ISceneObject	*pImage;
		f32			fElapsed;
		f32			fDir;
		bool		bRotate;

		SceneNode	cScene;
		Viewport	cViewport;
		Camera		cCamera;
		Renderer	cRenderer;

		Vector3f	vFrom;
		Vector3f	vCurrent;
		Vector3f	vTo;
};

#endif // _IMAGESAMPLE_H_
