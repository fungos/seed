#ifndef _BULLETSAMPLE_H
#define _BULLETSAMPLE_H

#include <bullet/btBulletDynamicsCommon.h>
#include <Seed.h>

using namespace Seed;

class BulletSample :	public IGameApp,
						public IEventSystemListener,
						public IEventInputKeyboardListener,
						public IEventInputPointerListener
{
	SEED_DISABLE_COPY(BulletSample)

	public:
		BulletSample();
		virtual ~BulletSample();

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
		btDiscreteDynamicsWorld				*pDynamicsWorld;
		btDefaultCollisionConfiguration		*pCollisionConfiguration;
		btCollisionDispatcher				*pDispatcher;
		btBroadphaseInterface				*pOverlappingPairCache;
		btSequentialImpulseConstraintSolver	*pSolver;

		btCollisionShape*							pGroundShape;
		btAlignedObjectArray<btCollisionShape*>		arCollisionShapes;
		btTransform									cGroundTransform;


		Presentation	cPres;
		SceneNode		*pScene;
		Camera			*pCamera;

};


#endif // _BULLETSAMPLE_H
