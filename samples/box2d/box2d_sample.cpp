#include "box2d_sample.h"

#define DEG2RAD		0.0174532925199432957f
#define RAD2DEG		57.295779513082320876f

#define PIX2M		0.01f
//#define PIX2M		0.0234375f	// PvM
//#define PIX2M		0.03125f	// Cocos2D
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
	: pWorld(NULL)
	, pGround(NULL)
	, pPick(NULL)
	, iId(0)
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
	pWorld = New(b2World(b2Vec2(0.0f, 10.0f)));

	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0.0f * PIX2M, 290.0f * PIX2M);
		bodyDef.angle = 0.0f;
		pGround = pWorld->CreateBody(&bodyDef);

		b2PolygonShape boxShape;
		boxShape.SetAsBox(200.0f * PIX2M, 10.0f * PIX2M);

		b2FixtureDef fixDef;
		fixDef.shape = &boxShape;
		fixDef.density = 1.0f;
		pGround->CreateFixture(&fixDef);
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

	for (b2Body *b = pWorld->GetBodyList(); b; b = b->GetNext())
	{
		ISceneObject *obj = (ISceneObject *)b->GetUserData();
		if (obj != NULL)
		{
			b2Vec2 p = b->GetPosition();
			f32 a = b->GetAngle() * RAD2DEG;
			obj->SetPosition(p.x * M2PIX, p.y * M2PIX);
			obj->SetRotation(a);
		}
	}

	return true;
}

bool Box2DSample::Shutdown()
{
	this->DestroyPhysics();
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
//		f32 mx = ev->GetX();
//		f32 my = ev->GetY();
//		mx *= PIX2M;
//		my *= PIX2M;
//		b2AABB aabb;
//		aabb.lowerBound.Set(mx - 0.001f, my - 0.001f);
//		aabb.upperBound.Set(mx + 0.001f, my + 0.001f);

//		Box2DQueryCallback cb;
//		pWorld->QueryAABB(&cb, aabb);

//		if (cb.vFound.size())
//			pPick = cb.vFound[0];

//		if (pPick)
//			Log("Found %d", cb.vFound.size());
//		else
//			Log("Not Found");
	}
	else if (ev->GetReleased() == Seed::ButtonRight)
	{
		Image *img = New(Image("frame03.png"));
		img->sName = "Image" + iId++;

		Vector3f p;
		p.setX(ev->GetX());
		p.setY(ev->GetY());
		p += pCamera->GetPosition();
		Log(">Click at %f, %f", ev->GetX(), ev->GetY());
		this->CreateBody(img, p.getX(), p.getY());
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

			pCamera = (Camera *)gScene->GetChildByName("MainCamera");
//			pCamera->SetPosition(0.0f, 0.0f);
			cViewport.SetCamera(pCamera);

			ISceneObject *obj = gScene->GetChildByName("Ground");
//			obj->SetZ(33333);
			pGround->SetUserData(obj);
			this->CreateBody((Image *)gScene->GetChildByName("Panda"), 0.0f, 0.0f);
		}
		break;
	}
}

void Box2DSample::OnJobAborted(const EventJob *ev)
{
	Job *job = ev->GetJob();
	Delete(job);
}

void Box2DSample::DestroyPhysics()
{
	for (b2Body *b = pWorld->GetBodyList(); b; b = b->GetNext())
	{
		ISceneObject *obj = (ISceneObject *)b->GetUserData();
		if (obj != NULL)
		{
			gScene->Remove(obj);
			Delete(obj);
			pWorld->DestroyBody(b);
		}
	}
}

void Box2DSample::CreateBody(Image *img, f32 x, f32 y)
{
	if (!img)
		return;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x * PIX2M, y * PIX2M);
	bodyDef.angle = 0;
	bodyDef.userData = img;
	b2Body *b = pWorld->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(1.0f * PIX2M, 1.0f * PIX2M);

	b2FixtureDef fixDef;
	fixDef.shape = &boxShape;
	fixDef.density = 1.0f;
	fixDef.restitution = 0.75f;
	b->CreateFixture(&fixDef);

	img->SetPosition(x, y);
	gScene->Add(img);
}
