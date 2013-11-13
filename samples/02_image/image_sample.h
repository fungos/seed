#ifndef _IMAGESAMPLE_H_
#define _IMAGESAMPLE_H_

#include <Seed.h>
using namespace Seed;

class ImageSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener
{
	SEED_DISABLE_COPY(ImageSample)

	public:
		ImageSample();
		virtual ~ImageSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

	protected:
		Presentation cPres;
		ISceneObject *pImage;
		Camera		*pCamera;
};

#endif // _IMAGESAMPLE_H_
