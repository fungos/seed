#include "tile_sample.h"

SceneNode *gScene;

TileSample::TileSample()
	: pPlayer(nullptr)
	, pCamera(nullptr)
	, pMap(nullptr)
	, cPres()
	, vDir()
	, fSpeed(5.0f)
	, bLoaded(false)
{
}

TileSample::~TileSample()
{
}

bool TileSample::Initialize()
{
	return cPres.Load("tile_sample.config", [&](Presentation *pres, Viewport *aborted) {
		if (!aborted)
		{
			auto vp = pres->GetViewportByName("MainView");
			gScene = vp->GetScene();
			pCamera = vp->GetCamera();

			pPlayer = (ISceneObject *)gScene->GetChildByName("Player");

			pMap = (GameMap *)gScene->GetChildByName("Map");
			pCamera->SetParent(pPlayer);
			vDir = vec3{0.0f, 0.0f, 0.0f};

			/* How to read the various properties in a tiled map */
			auto mapName = pMap->GetProperty("map_name");
			auto layerVelocity = pMap->GetLayerByName("Ground")->GetProperty("velocity");
			auto type = pMap->GetTileSet("Desert")->GetProperty("terrain_type");
			auto tileProp1 = pMap->GetTileSet("Desert")->GetTileProperty(1, "type");
			auto tileProp30 = pMap->GetTileSet("Desert")->GetTileProperty(30, "type");

			pSystem->AddListener(this);
			pInput->AddKeyboardListener(this);

			bLoaded = true;
		}
	});
}

bool TileSample::Update(Seconds dt)
{
	UNUSED(dt)

	if (bLoaded)
	{
		pPlayer->AddPosition(vDir);

		/*
		   Really ugly, you should cache the tiled layer and the tileset to do this,
		   but shows how to access the current tile (player position) properties
		*/
		String prop = pMap->GetTileSet("Desert")->GetTileProperty(pMap->GetLayerByName("Ground")->AsTiled()->GetTileAt(pPlayer->GetPosition()), "type");
		if (prop != "")
			Log("Stepping at tile with property type: %s", prop.c_str());
	}

	return true;
}

bool TileSample::Shutdown()
{
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	cPres.Unload();
	return IGameApp::Shutdown();
}

void TileSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

bool TileSample::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	switch (k)
	{
		case eKey::W:
			vDir.y = -fSpeed;
		break;
		case eKey::S:
			vDir.y = fSpeed;
		break;
		case eKey::A:
			vDir.x = -fSpeed;
		break;
		case eKey::D:
			vDir.x = fSpeed;
		break;
		default: return true;
	}

	return true;
}

bool TileSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	switch (k)
	{
		case eKey::Escape:
			pSystem->Shutdown();
		break;
		case eKey::F1:
			pResourceManager->Print();
		break;
		case eKey::F2:
			pResourceManager->GarbageCollect();
		break;
		case eKey::W:
			vDir.y = 0.0f;
		break;
		case eKey::S:
			vDir.y = 0.0f;
		break;
		case eKey::A:
			vDir.x = 0.0f;
		break;
		case eKey::D:
			vDir.x = 0.0f;
		break;
		default: return true;
	}

	return true;
}
