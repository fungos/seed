#include "bullet_sample.h"

BulletSample::BulletSample()
	: pDynamicsWorld(nullptr)
	, pCollisionConfiguration(nullptr)
	, pDispatcher(nullptr)
	, pOverlappingPairCache(nullptr)
	, pSolver(nullptr)
	, pGroundShape(nullptr)
	, arCollisionShapes()
	, cGroundTransform()
	, cPres()
	, pScene(nullptr)
	, pCamera(nullptr)
{
}

BulletSample::~BulletSample()
{
}

bool BulletSample::Initialize()
{
	cPres.Load("bullet_sample.config", [&](Presentation *, Renderer *) { /* TODO */ });

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
	arCollisionShapes.push_back(pGroundShape);

	cGroundTransform.setIdentity();
	cGroundTransform.setOrigin(btVector3(0,-56,0));

	auto mass = btScalar(0.0f);

	// Rigidbody is dynamic if and only if mass is non zero, otherwise static
	auto isDynamic = bool(mass != 0.f);

	auto localInertia = btVector3(0,0,0);
	if (isDynamic)
		pGroundShape->calculateLocalInertia(mass,localInertia);

	// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	auto myMotionStateRigidBody = new btDefaultMotionState(cGroundTransform);
	auto rbInfoRigidBody = btRigidBody::btRigidBodyConstructionInfo(mass, myMotionStateRigidBody, pGroundShape, localInertia);
	auto rigidBody = new btRigidBody(rbInfoRigidBody);

	// Add the body to the dynamics world
	pDynamicsWorld->addRigidBody(rigidBody);

	/*
	 * Create a dynamic rigidbody
	 */

	//auto colShape = new btBoxShape(btVector3(1,1,1));
	auto colShape = new btSphereShape(btScalar(1.));
	arCollisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	auto startTransform = btTransform();
	startTransform.setIdentity();

	mass = 1.f;

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	isDynamic = (mass != 0.f);

	localInertia = btVector3(0,0,0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass,localInertia);

	startTransform.setOrigin(btVector3(2,10,0));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	auto myMotionState = new btDefaultMotionState(startTransform);
	auto rbInfo = btRigidBody::btRigidBodyConstructionInfo(mass,myMotionState,colShape,localInertia);
	auto body = new btRigidBody(rbInfo);

	pDynamicsWorld->addRigidBody(body);

	return true;
}

bool BulletSample::Update(f32 dt)
{
	UNUSED(dt)
	pDynamicsWorld->stepSimulation(1.f/60.f,10);

	//print positions of all objects
	for (int j=pDynamicsWorld->getNumCollisionObjects()-1; j>=0 ;j--)
	{
		auto obj = pDynamicsWorld->getCollisionObjectArray()[j];
		auto body = btRigidBody::upcast(obj);
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
	sdDelete(pDynamicsWorld);
	sdDelete(pSolver);
	sdDelete(pOverlappingPairCache);
	sdDelete(pDispatcher);
	sdDelete(pCollisionConfiguration);

	sdDelete(pGroundShape);
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
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
	else if (k == eKey::F1)
		pResourceManager->Print();
	else if (k == eKey::F2)
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

void BulletSample::DestroyPhysics()
{
	//TODO
}

void BulletSample::CreateBody(Image *img, f32 x, f32 y)
{
	//TODO
}



