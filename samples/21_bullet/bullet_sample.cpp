#include "bullet_sample.h"

BulletSample::BulletSample()
	: pDynamicsWorld(NULL)
	, pCollisionConfiguration(NULL)
	, pDispatcher(NULL)
	, pOverlappingPairCache(NULL)
	, pSolver(NULL)
	, cPres()
	, pScene(NULL)
	, pCamera(NULL)
{
}

BulletSample::~BulletSample()
{
}

bool BulletSample::Initialize()
{
	cPres.Load("bullet_sample.config", this);

	/*
	 * Create a World
	 */
	pCollisionConfiguration		= new btDefaultCollisionConfiguration();
	pDispatcher					= new btCollisionDispatcher(pCollisionConfiguration);
	pOverlappingPairCache		= new btDbvtBroadphase();
	pSolver						= new btSequentialImpulseConstraintSolver;
	pDynamicsWorld				= new btDiscreteDynamicsWorld(pDispatcher, pOverlappingPairCache, pSolver, pCollisionConfiguration);

	pDynamicsWorld->setGravity(btVector3(0,-10,0));

	/*
	 * Create a few basic rigid bodies
	 */
	pGroundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));

	// Keep track of the shapes, we release memory at exit.
	// Make sure to re-use collision shapes among rigid bodies whenever possible!
	pCollisionShapes.push_back(pGroundShape);

	pGroundTransform.setIdentity();
	pGroundTransform.setOrigin(btVector3(0,-56,0));

	btScalar mass;
	mass = 0.;

	// Rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic;
	isDynamic = (mass != 0.f);

	btVector3 localInertia;
	localInertia = btVector3(0,0,0);
	if (isDynamic)
		pGroundShape->calculateLocalInertia(mass,localInertia);

	// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionStateRigidBody = new btDefaultMotionState(pGroundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfoRigidBody(mass, myMotionStateRigidBody, pGroundShape, localInertia);
	btRigidBody* rigidBody = new btRigidBody(rbInfoRigidBody);

	// Add the body to the dynamics world
	pDynamicsWorld->addRigidBody(rigidBody);

	/*
	 * Create a dynamic rigidbody
	 */

	//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
	btCollisionShape* colShape = new btSphereShape(btScalar(1.));
	pCollisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	mass = 1.f;

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	isDynamic = (mass != 0.f);

	localInertia = btVector3(0,0,0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass,localInertia);

	startTransform.setOrigin(btVector3(2,10,0));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	pDynamicsWorld->addRigidBody(body);

	return true;
}

bool BulletSample::Update(f32 dt)
{
	pDynamicsWorld->stepSimulation(1.f/60.f,10);

	//print positions of all objects
	for (int j=pDynamicsWorld->getNumCollisionObjects()-1; j>=0 ;j--)
	{
		btCollisionObject* obj = pDynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);
			printf("world pos = %f,%f,%f\n",float(trans.getOrigin().getX()),float(trans.getOrigin().getY()),float(trans.getOrigin().getZ()));
		}
	}

	return true;
}

bool BulletSample::Shutdown()
{
	this->DestroyPhysics();
	Delete(pDynamicsWorld);
	Delete(pSolver);
	Delete(pOverlappingPairCache);
	Delete(pDispatcher);
	Delete(pCollisionConfiguration);

	Delete(pGroundShape);
	cPres.Unload();

	pInput->RemovePointerListener(this);
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void BulletSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void BulletSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
}

void BulletSample::OnInputPointerPress(const EventInputPointer *ev)
{
	//TODO
}

void BulletSample::OnInputPointerMove(const EventInputPointer *ev)
{
	//TODO
}

void BulletSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	//TODO
}

void BulletSample::OnPresentationLoaded(const EventPresentation *ev)
{
	//TODO
}

void BulletSample::DestroyPhysics()
{
	//TODO
}

void BulletSample::CreateBody(Image *img, f32 x, f32 y)
{
	//TODO
}



