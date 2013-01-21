#ifndef _ROCKET_SAMPLE_H_
#define _ROCKET_SAMPLE_H_

#include <api/rocket/RocketInterface.h>
#include <Rocket/Core.h>
#include <Seed.h>
using namespace Seed;
using namespace Seed::RocketGui;

class RocketSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventPresentationListener
{
	public:
		RocketSample();
		virtual ~RocketSample();

		virtual bool Initialize();
		virtual bool Shutdown();

		bool LoadGUI(const String &doc);
		bool ReloadGUI();
		bool UnloadGUI();

		bool InitializeGUI();
		void ReleaseGUI();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventPresentationListener
		virtual void OnPresentationLoaded(const EventPresentation *ev);

	private:
		SEED_DISABLE_COPY(RocketSample);

	protected:
		SceneNode *pScene;
		RocketInterface *pI;
		Rocket::Core::Context *pContext;
		Rocket::Core::ElementDocument *pDoc;
		Presentation cPres;

		String sDocument;
		bool bLoaded;
};

#endif // _ROCKET_SAMPLE_H_
