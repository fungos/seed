#ifndef _BOX2DSAMPLE_H_
#define _BOX2DSAMPLE_H_

#include <Box2D/Box2D.h>
#include <Seed.h>
using namespace Seed;

class Box2DSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener,
					public IEventPresentationListener
{
	public:
		Box2DSample();
		virtual ~Box2DSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventInputPointerListener
		virtual void OnInputPointerPress(const EventInputPointer *ev);
		virtual void OnInputPointerRelease(const EventInputPointer *ev);
		virtual void OnInputPointerMove(const EventInputPointer *ev);

		// IEventPresentationListener
		virtual void OnPresentationLoaded(const EventPresentation *ev);

	private:
		SEED_DISABLE_COPY(Box2DSample);

		void CreateBody(Image *img, f32 x, f32 y);
		void DestroyPhysics();

	protected:
		b2World		*pWorld;
		b2Body		*pGround;
		b2Body		*pPick;
		b2MouseJoint *pMouseJoint;

		Presentation cPres;
		SceneNode	*pScene;
		Camera		*pCamera;

		u32			iId;
};

#endif // _BOX2DSAMPLE_H_
