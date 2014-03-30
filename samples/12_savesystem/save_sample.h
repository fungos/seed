#ifndef _SAVESAMPLE_H_
#define _SAVESAMPLE_H_

#include <Seed.h>
using namespace Seed;

class SaveSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener
{
	SEED_DISABLE_COPY(SaveSample)

	public:
		SaveSample();
		virtual ~SaveSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

		// IEventInputPointerListener
		virtual void OnInputPointerRelease(const EventInputPointer *ev) override;

	private:
		bool SaveSystemFlow() const;

	protected:
		Presentation cPres;
		ISceneObject *pImage;
		Camera		*pCamera;

		vec3	vFrom;
		vec3	vCurrent;
		vec3	vTo;

		f32		fElapsed;
		f32		fDir;
		bool	bRotate : 1;
};

#endif // _SAVESAMPLE_H_
