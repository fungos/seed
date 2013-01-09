#ifndef _RENDERERSAMPLE_H_
#define _RENDERERSAMPLE_H_

#include <Seed.h>
using namespace Seed;

extern SceneNode *gScene;

class RendererSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener,
					public IEventJobListener
{
	public:
		RendererSample();
		virtual ~RendererSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
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
		SEED_DISABLE_COPY(RendererSample);

	protected:
		ISceneObject *pImage;
		f32			fElapsed;
		f32			fDir;
		bool		bRotate;

		SceneNode	cScene;
		Viewport	cViewport;
		Camera		*pCamera;
		Renderer	cRenderer;

		Vector3f	vFrom;
		Vector3f	vCurrent;
		Vector3f	vTo;
};

#endif // _RENDERERSAMPLE_H_
