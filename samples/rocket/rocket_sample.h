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
					public IEventJobListener
{
	public:
		RocketSample();
		virtual ~RocketSample();

		virtual bool Initialize();
		virtual bool Shutdown();

		bool InitializeGUI();
		void ReleaseGUI();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventJobListener
		virtual void OnJobCompleted(const EventJob *ev);
		virtual void OnJobAborted(const EventJob *ev);

	private:
		SEED_DISABLE_COPY(RocketSample);

	protected:
		SceneNode	cScene;
		Viewport	cViewport;
		Camera		*pCamera;
		Renderer	cRenderer;

		RocketInterface *pI;
		Rocket::Core::Context *pContext;
		Rocket::Core::ElementDocument *pDoc;
};

#endif // _ROCKET_SAMPLE_H_
