#include "box2d_sample.h"

#define DEG2RAD		0.0174532925199432957f
#define RAD2DEG		57.295779513082320876f
#define PIX2M		0.015f
//#define PIX2M		0.0234375f
#define M2PIX		(1.0f / PIX2M)

SceneNode *gScene;

enum
{
	kJobLoadScene
};

class Box2DQueryCallback : public b2QueryCallback
{
	public:
		std::vector<b2Body *> vFound;

		bool ReportFixture(b2Fixture *fixture)
		{
			vFound.push_back(fixture->GetBody());
			return false;
		}
};

Box2DSample::Box2DSample()
	: pImgBody(NULL)
	, pImgGround(NULL)
	, pWorld(NULL)
	, pBody(NULL)
	, pGround(NULL)
	, pPick(NULL)
{
}

Box2DSample::~Box2DSample()
{
}

bool Box2DSample::Initialize()
{
	/* ------- Rendering Initialization ------- */
	cRenderer.Add(&cScene);

	cViewport.SetHeight(pScreen->GetHeight());
	cViewport.SetWidth(pScreen->GetWidth());
	cViewport.SetRenderer(&cRenderer);

	pViewManager->Add(&cViewport);
	pRendererManager->Add(&cRenderer);
	pSceneManager->Add(&cScene);
	gScene = &cScene;
	/* ------- Rendering Initialization ------- */

	pJobManager->Add(New(FileLoader("box2d_sample.scene", kJobLoadScene, this)));
	pWorld = New(b2World(b2Vec2(0, 10), true));

	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0.0f * PIX2M, 290.0f * PIX2M);
		bodyDef.angle = 0;
		pGround = pWorld->CreateBody(&bodyDef);

		b2PolygonShape boxShape;
		boxShape.SetAsBox(400 * PIX2M, 20 * PIX2M);

		b2FixtureDef fixDef;
		fixDef.shape = &boxShape;
		fixDef.density = 1;
		pGround->CreateFixture(&fixDef);
	}

	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0.0f * PIX2M, 0.0f * PIX2M);
		bodyDef.angle = 0;
		pBody = pWorld->CreateBody(&bodyDef);

		b2PolygonShape boxShape;
		boxShape.SetAsBox(38 * PIX2M, 38 * PIX2M);

		b2FixtureDef fixDef;
		fixDef.shape = &boxShape;
		fixDef.density = 1;
		fixDef.restitution = 0.75f;
		pBody->CreateFixture(&fixDef);
	}

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);
	pInput->AddPointerListener(this);

	return true;
}

bool Box2DSample::Update(f32 dt)
{
	UNUSED(dt)

	pWorld->Step(dt, 8, 3);
	pWorld->ClearForces();

	if (pImgBody)
	{
		b2Vec2 p = pBody->GetPosition();
		f32 a = pBody->GetAngle() * RAD2DEG;
		pImgBody->SetPosition(p.x * M2PIX, p.y * M2PIX);
		pImgBody->SetRotation(a);
	}

	if (pImgGround)
	{
		b2Vec2 p = pGround->GetPosition();
		f32 a = pGround->GetAngle() * RAD2DEG;
		pImgGround->SetPosition(p.x * M2PIX, p.y * M2PIX);
		pImgGround->SetRotation(a);
	}

	return true;
}

bool Box2DSample::Shutdown()
{
	pWorld->DestroyBody(pGround);
	pWorld->DestroyBody(pBody);
	Delete(pWorld);

	pInput->RemovePointerListener(this);
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	pSceneManager->Reset();
	pRendererManager->Reset();
	pViewManager->Reset();
	gScene->Unload();

	IGameApp::Shutdown();

	return true;
}

bool Box2DSample::Reset()
{
	return true;
}

void Box2DSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void Box2DSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
}

void Box2DSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	if (ev->GetReleased() == Seed::ButtonLeft)
	{
		f32 mx = ev->GetX();
		f32 my = ev->GetY();
		mx *= PIX2M;
		my *= PIX2M;
		b2AABB aabb;
		aabb.lowerBound.Set(mx - 0.001f, my - 0.001f);
		aabb.upperBound.Set(mx + 0.001f, my + 0.001f);

		Box2DQueryCallback cb;
		pWorld->QueryAABB(&cb, aabb);

		if (cb.vFound.size())
			pPick = cb.vFound[0];

		if (pPick)
			Log("Found %d", cb.vFound.size());
		else
			Log("Not Found");
	}
}

void Box2DSample::OnJobCompleted(const EventJob *ev)
{
	switch (ev->GetName())
	{
		case kJobLoadScene:
		{
			FileLoader *job = (FileLoader *)ev->GetJob();
			Reader r(job->pFile);
			gScene->Load(r);
			Delete(job);

			pImgBody = (ISceneObject *)gScene->GetChildByName("Panda");
			pImgGround = (ISceneObject *)gScene->GetChildByName("Ground");
			pCamera = (Camera *)gScene->GetChildByName("MainCamera");
			pCamera->Update(0.0f);
			cViewport.SetCamera(pCamera);
		}
		break;
	}
}

void Box2DSample::OnJobAborted(const EventJob *ev)
{
	Job *job = ev->GetJob();
	Delete(job);
}
