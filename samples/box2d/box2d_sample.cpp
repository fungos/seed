#include "box2d_sample.h"

#define DEG2RAD		0.0174532925199432957f
#define RAD2DEG		57.295779513082320876f
#define PIX2M		0.01f
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

struct PhysicsEntity
{
	Image *pImage;
	b2Body *pBody;
};

Box2DSample::Box2DSample()
	: pImgGround(NULL)
	, pWorld(NULL)
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
	pWorld = New(b2World(b2Vec2(0, 10)));

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

	this->UpdateEntities();

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
	this->DestroyEntities();

	pWorld->DestroyBody(pGround);
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
	else if (ev->GetReleased() == Seed::ButtonRight)
	{
		Image *img = New(Image("frame03.png"));
		img->sName = "Image" + iId++;

		Vector3f p;
		p.setX(ev->GetX());
		p.setY(ev->GetY());
		p += pCamera->GetPosition();
		Log(">Click at %f, %f", ev->GetX(), ev->GetY());
		this->CreateEntity(img, p.getX(), p.getY());
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
			pCamera->Update(0.0f);
			cViewport.SetCamera(pCamera);

			pImgGround = (ISceneObject *)gScene->GetChildByName("Ground");
			this->CreateEntity((Image *)gScene->GetChildByName("Panda"), 0, 0);
		}
		break;
	}
}

void Box2DSample::OnJobAborted(const EventJob *ev)
{
	Job *job = ev->GetJob();
	Delete(job);
}

void Box2DSample::UpdateEntities()
{
	PhysicsEntityVectorIterator it = vEntities.begin();
	PhysicsEntityVectorIterator end = vEntities.end();
	for (; it != end; ++it)
	{
		PhysicsEntity *obj = (*it);
		b2Vec2 p = obj->pBody->GetPosition();
		f32 a = obj->pBody->GetAngle() * RAD2DEG;
		obj->pImage->SetPosition(p.x * M2PIX, p.y * M2PIX);
		obj->pImage->SetRotation(a);
	}
}

void Box2DSample::DestroyEntities()
{
	PhysicsEntityVectorIterator it = vEntities.begin();
	PhysicsEntityVectorIterator end = vEntities.end();
	for (; it != end; ++it)
	{
		PhysicsEntity *obj = (*it);

		gScene->Remove(obj->pImage);
		Delete(obj->pImage);
		pWorld->DestroyBody(obj->pBody);
		Delete(obj);
	}

	PhysicsEntityVector().swap(vEntities);
}

void Box2DSample::CreateEntity(Image *img, f32 x, f32 y)
{
	if (!img)
		return;

	Log(">Create at %f, %f -> %f, %f", x, y, x * PIX2M, y * PIX2M);
	PhysicsEntity *obj = New(PhysicsEntity());
	obj->pImage = img;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x * PIX2M, y * PIX2M);
	bodyDef.angle = 0;
	obj->pBody = pWorld->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(38 * PIX2M, 38 * PIX2M);

	b2FixtureDef fixDef;
	fixDef.shape = &boxShape;
	fixDef.density = 1;
	fixDef.restitution = 0.75f;
	obj->pBody->CreateFixture(&fixDef);

	img->SetPosition(x, y);
	gScene->Add(img);

	vEntities.push_back(obj);
}
