#ifndef _TESTBASE_H_
#define _TESTBASE_H_

#include <Seed.h>
using namespace Seed;

extern SceneNode *pScene;

using namespace Seed;

class TestBase : public IGameApp, public IEventSystemListener, public IEventInputKeyboardListener
{
	public:
		TestBase();
		virtual ~TestBase();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Reset();
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

	private:
		SEED_DISABLE_COPY(TestBase);

	protected:
		SceneNode	cScene;
		Viewport	cViewport;
		Renderer	cRenderer;
		Sprite		sptLogo;
		Music		musTheme;
		SoundSource sfxSound;
};

#endif // _TESTBASE_H_
