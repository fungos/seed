#ifndef _SHADERSAMPLE_H_
#define _SHADERSAMPLE_H_

#include <Seed.h>
#include <ShaderManager.h>
#include <ShaderProgram.h>
#include <Shader.h>

using namespace Seed;
using namespace Seed::OpenGL;

extern SceneNode *gScene;

class ShaderSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener
{
	SEED_DISABLE_COPY(ShaderSample)

	public:
		ShaderSample();
		virtual ~ShaderSample();

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
		IShaderProgram *pProgram;
		IShader *pVertexShader;
		IShader *pPixelShader;
		f32 fElapsedTime;
		bool bInitialized;
};

#endif // _SHADERSAMPLE_H_
