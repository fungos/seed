#ifndef _ROCKET_SAMPLE_H_
#define _ROCKET_SAMPLE_H_

#include <api/rocket/RocketInterface.h>
#include <Rocket/Core.h>
#include <Seed.h>
using namespace Seed;
using namespace Seed::RocketGui;

extern SceneNode *gScene;

class RocketSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener,
					public IEventJobListener
{
	public:
		RocketSample();
		virtual ~RocketSample();

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
		SEED_DISABLE_COPY(RocketSample);

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

		RocketInterface *pI;
		Rocket::Core::Context *pContext;
		Rocket::Core::ElementDocument *pDoc;
};

#endif // _ROCKET_SAMPLE_H_
