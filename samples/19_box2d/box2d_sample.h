#ifndef _BOX2DSAMPLE_H_
#define _BOX2DSAMPLE_H_

#include <Box2D/Box2D.h>
#include <Seed.h>
using namespace Seed;

class Box2DSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener
{
	SEED_DISABLE_COPY(Box2DSample)

	public:
		Box2DSample();
		virtual ~Box2DSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

		// IEventInputPointerListener
		virtual void OnInputPointerPress(const EventInputPointer *ev) override;
		virtual void OnInputPointerRelease(const EventInputPointer *ev) override;
		virtual void OnInputPointerMove(const EventInputPointer *ev) override;

	private:
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
