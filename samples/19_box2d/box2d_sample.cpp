#include "box2d_sample.h"
#include "Memory.h"
#include <sstream>

#define DEG2RAD		0.0174532925199432957f
#define RAD2DEG		57.295779513082320876f

#define PIX2M		0.01f
#define M2PIX		(1.0f / PIX2M)

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
	: pWorld(nullptr)
	, pGround(nullptr)
	, pPick(nullptr)
	, pMouseJoint(nullptr)
	, cPres()
	, pScene(nullptr)
	, pCamera(nullptr)
	, iId(0)
{
}

Box2DSample::~Box2DSample()
{
}

bool Box2DSample::Initialize()
{
	cPres.Load("box2d_sample.config", [&](Presentation *pres, Renderer *) {
		pScene = pres->GetRendererByName("MainRenderer")->GetScene();
		pCamera = static_cast<Camera *>(pScene->GetChildByName("MainCamera"));

		ISceneObject *obj = pScene->GetChildByName("Ground");
		obj->SetPosition(pGround->GetPosition().x * M2PIX, pGround->GetPosition().y * M2PIX);
		obj->SetVisible(true);
		pGround->SetUserData(obj);

		this->CreateBody(static_cast<Image *>(pScene->GetChildByName("Panda")), 0.0f, 0.0f);

		pSystem->AddListener(this);
		pInput->AddKeyboardListener(this);
		pInput->AddPointerListener(this);
	});

	pWorld = sdNew(b2World(b2Vec2(0.0f, 10.0f)));
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0.0f * PIX2M, 290.0f * PIX2M);
		bodyDef.angle = 0.0f;
		pGround = pWorld->CreateBody(&bodyDef);

		b2PolygonShape boxShape;
		boxShape.SetAsBox(400.0f * PIX2M, 10.0f * PIX2M);

		b2FixtureDef fixDef;
		fixDef.shape = &boxShape;
		fixDef.density = 1.0f;
		pGround->CreateFixture(&fixDef);
	}

	return true;
}

bool Box2DSample::Update(Seconds dt)
{
	pWorld->Step(dt, 8, 3);
	pWorld->ClearForces();

	for (b2Body *b = pWorld->GetBodyList(); b; b = b->GetNext())
	{
		ISceneObject *obj = (ISceneObject *)b->GetUserData();
		if (obj != nullptr)
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
	sdDelete(pWorld);

	cPres.Unload();

	pInput->RemovePointerListener(this);
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void Box2DSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void Box2DSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
	else if (k == eKey::F1)
		pResourceManager->Print();
	else if (k == eKey::F2)
		pResourceManager->GarbageCollect();
}

void Box2DSample::OnInputPointerPress(const EventInputPointer *ev)
{
	Vector3f p;
	p.setX(ev->GetX());
	p.setY(ev->GetY());
	p += pCamera->GetPosition();

	if (ev->GetPressed() == eInputButton::MouseLeft)
	{
		auto mx = p.getX() * PIX2M;
		auto my = p.getY() * PIX2M;

		b2AABB aabb;
		aabb.lowerBound.Set(mx - 0.001f, my - 0.001f);
		aabb.upperBound.Set(mx + 0.001f, my + 0.001f);

		Box2DQueryCallback cb;
		pWorld->QueryAABB(&cb, aabb);

		if (cb.vFound.size())
		{
			pPick = cb.vFound[0];
			Log("Picking an object, found %d objects - using first object", cb.vFound.size());
		}
		else
		{
			pPick = nullptr;
		}

		if (pPick)
		{
			b2MouseJointDef def;
			def.bodyA = pGround;
			def.bodyB = pPick;
			def.target.Set(p.getX() * PIX2M, p.getY() * PIX2M);
			def.collideConnected = true;
			def.maxForce = 300.0f * pPick->GetMass();
			pMouseJoint = (b2MouseJoint *)pWorld->CreateJoint((b2JointDef *)&def);
			pPick->SetAwake(true);
		}
	}
}

void Box2DSample::OnInputPointerMove(const EventInputPointer *ev)
{
	Vector3f p;
	p.setX(ev->GetX());
	p.setY(ev->GetY());
	p += pCamera->GetPosition();

	if (pPick)
	{
		b2Vec2 v;
		v.Set(p.getX() * PIX2M, p.getY() * PIX2M);
		pMouseJoint->SetTarget(v);
	}
}

void Box2DSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	Vector3f p;
	p.setX(ev->GetX());
	p.setY(ev->GetY());
	p += pCamera->GetPosition();

	if (ev->GetReleased() == eInputButton::MouseLeft)
	{
		if (pPick)
		{
			pWorld->DestroyJoint(pMouseJoint);
			pMouseJoint = nullptr;
			pPick = nullptr;
		}
	}
	else if (ev->GetReleased() == eInputButton::MouseRight)
	{
		auto img = sdNew(Image("frame03.png"));
		img->bMarkForDeletion = true;

		std::stringstream ss;
		ss << "Image_" << iId++;
		img->sName = ss.str();

		this->CreateBody(img, p.getX(), p.getY());
		pScene->Add(img);
	}
}

void Box2DSample::DestroyPhysics()
{
	for (b2Body *b = pWorld->GetBodyList(); b; b = b->GetNext())
	{
		ISceneObject *obj = static_cast<ISceneObject *>(b->GetUserData());
		if (obj != nullptr)
		{
			pScene->Remove(obj);
			sdDelete(obj);
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
	auto b = pWorld->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(38.0f * PIX2M, 38.0f * PIX2M);

	b2FixtureDef fixDef;
	fixDef.shape = &boxShape;
	fixDef.density = 1.0f;
	fixDef.restitution = 0.75f;
	b->CreateFixture(&fixDef);

	img->SetPosition(x, y, 0.0f - iId);
}
