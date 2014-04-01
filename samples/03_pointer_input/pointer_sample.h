#ifndef _POINTERSAMPLE_H_
#define _POINTERSAMPLE_H_

#include <Seed.h>
using namespace Seed;

class PointerSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener
{
	SEED_DISABLE_COPY(PointerSample)

	public:
		PointerSample();
		virtual ~PointerSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

		// IEventInputPointerListener
		virtual void OnInputPointerRelease(const EventInputPointer *ev) override;

	protected:
		Presentation cPres;
		ISceneObject *pObject;
		Camera	*pCamera;

		vec3	vFrom;
		vec3	vCurrent;
		vec3	vTo;

		f32		fElapsed;
		f32		fDir;
		bool	bRotate : 1;
};

#endif // _POINTERSAMPLE_H_
