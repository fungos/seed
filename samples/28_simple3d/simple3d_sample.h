#ifndef _SIMPLE3DSAMPLE_H_
#define _SIMPLE3DSAMPLE_H_

#include <Seed.h>

using namespace Seed;
using namespace Seed::OpenGL;

extern SceneNode *gScene;

class Simple3DSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener
{
	SEED_DISABLE_COPY(Simple3DSample)

	public:
		Simple3DSample();
		virtual ~Simple3DSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

	protected:
		Presentation cPres;
		ISceneObject *pObject;
		f32 fElapsedTime;
		bool bInitialized;
};

#endif // _SIMPLE3DSAMPLE_H_
