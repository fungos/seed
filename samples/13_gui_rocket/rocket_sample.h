#ifndef _ROCKET_SAMPLE_H_
#define _ROCKET_SAMPLE_H_

#include <api/rocket/RocketInterface.h>
#include <Rocket/Core.h>
#include <Seed.h>
using namespace Seed;
using namespace Seed::RocketGui;

class RocketSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener
{
	SEED_DISABLE_COPY(RocketSample)

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
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

	protected:
		SceneNode *pScene;
		RocketInterface *pI;
		Rocket::Core::Context *pContext;
		Rocket::Core::ElementDocument *pDoc;
		Presentation cPres;

		String sDocument;
		bool bLoaded : 1;
};

#endif // _ROCKET_SAMPLE_H_
