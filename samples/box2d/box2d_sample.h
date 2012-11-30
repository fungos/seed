#ifndef _BOX2DSAMPLE_H_
#define _BOX2DSAMPLE_H_

#include <Box2D/Box2D.h>
#include <Seed.h>
using namespace Seed;

extern SceneNode *gScene;

struct PhysicsEntity;

class Box2DSample : public IGameApp,
					public IEventSystemListener,
					public IEventInputKeyboardListener,
					public IEventInputPointerListener,
					public IEventJobListener
{
	DECLARE_CONTAINER_TYPE(Vector, PhysicsEntity)
	public:
		Box2DSample();
		virtual ~Box2DSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Reset();
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

		// IEventInputPointerListener
		virtual void OnInputPointerRelease(const EventInputPointer *ev);

		// IEventJobListener
		virtual void OnJobCompleted(const EventJob *ev);
		virtual void OnJobAborted(const EventJob *ev);

	private:
		SEED_DISABLE_COPY(Box2DSample);

		void CreateEntity(Image *img, f32 x, f32 y);
		void DestroyEntities();
		void UpdateEntities();

	protected:
		ISceneObject *pImgGround;
		b2World		*pWorld;
		b2Body		*pGround;
		b2Body		*pPick;

		SceneNode	cScene;
		Viewport	cViewport;
		Camera		*pCamera;
		Renderer	cRenderer;

		PhysicsEntityVector vEntities;
		u32			iId;
};

#endif // _BOX2DSAMPLE_H_
