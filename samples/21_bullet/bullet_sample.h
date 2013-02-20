#ifndef _BULLETSAMPLE_H
#define _BULLETSAMPLE_H

#include <bullet/btBulletDynamicsCommon.h>
#include <Seed.h>

using namespace Seed;

class BulletSample :	public IGameApp,
						public IEventSystemListener,
						public IEventInputKeyboardListener,
						public IEventInputPointerListener,
						public IEventPresentationListener
{
	public:
		BulletSample();
		virtual ~BulletSample();

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
		SEED_DISABLE_COPY(BulletSample);

		void CreateBody(Image *img, f32 x, f32 y);
		void DestroyPhysics();

	protected:
		btDiscreteDynamicsWorld				*pDynamicsWorld;
		btDefaultCollisionConfiguration		*pCollisionConfiguration;
		btCollisionDispatcher				*pDispatcher;
		btBroadphaseInterface				*pOverlappingPairCache;
		btSequentialImpulseConstraintSolver	*pSolver;

		btCollisionShape*							pGroundShape;
		btAlignedObjectArray<btCollisionShape*>		pCollisionShapes;
		btTransform									pGroundTransform;


		Presentation	cPres;
		SceneNode		*pScene;
		Camera			*pCamera;

};


#endif // _BULLETSAMPLE_H