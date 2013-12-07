#ifndef _TESTBASE_H_
#define _TESTBASE_H_

#include <Seed.h>
#include <ShaderManager.h>
#include <ShaderProgram.h>
#include <Shader.h>

using namespace Seed;
using namespace Seed::OpenGL;

extern SceneNode *gScene;

class TestBase : public IGameApp, public IEventSystemListener, public IEventInputKeyboardListener, public IEventJobListener
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

		// IEventJobListener
		virtual void OnJobCompleted(const EventJob *ev);
		virtual void OnJobAborted(const EventJob *ev);

	private:
		SEED_DISABLE_COPY(TestBase);

	protected:
		Seed::RocketGui::RocketInterface *pRocketInterface;
		SceneNode	cScene;
		Viewport	cViewport;
		Camera		cCamera;
		Camera		cCameraTex;
		Texture		cRenderTarget;
		Renderer	cRenderer;
		Sprite		sptLogo;
//		Music		musTheme;
//		SoundSource sfxSound;
//		Movie		mvSample;
//		ParticleEmitter cEmitter;
};

#endif // _TESTBASE_H_
