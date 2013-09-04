#ifndef _TESTBASE_OGLES2_H_
#define _TESTBASE_OGLES2_H_

#include <Seed.h>

#include <ShaderManager.h>
#include <ShaderProgram.h>
#include <Shader.h>

using namespace Seed;
using namespace Seed::GLSL;

extern SceneNode *gScene;

class TestBaseOGLES2 : public IGameApp, public IEventSystemListener, public IEventInputKeyboardListener, public IEventJobListener
{
	public:
		TestBaseOGLES2();
		virtual ~TestBaseOGLES2();

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
		SEED_DISABLE_COPY(TestBaseOGLES2);

	protected:
		SceneNode                   cScene;
		Viewport                    cViewport;
		Camera                      cCamera;
		Camera                      cCameraTex;
		Texture                     cRenderTarget;
		Renderer                    cRenderer;
		Sprite                      sptLogo;
};

#endif // _TESTBASE_H_
