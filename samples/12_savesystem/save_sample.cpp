#include "save_sample.h"

#define MY_GAME_ID				0xBADF00D
#define MY_GAME_NAME_LEN		20
#define MY_GAME_RANKING_SIZE	9

struct MySlotStruct
{
	char	pcName[MY_GAME_NAME_LEN];
	u32		iLevel;
	f32		fPosX;
	f32		fPosY;
};

struct MyRankingStruct
{
	char	pcName[MY_GAME_NAME_LEN];
	u32		iScore;
	u32		iTime;
};

struct MySharedStruct
{
	MyRankingStruct arEntries[MY_GAME_RANKING_SIZE];
};

SaveSample::SaveSample()
	: cPres()
	, pImage(nullptr)
	, pCamera(nullptr)
	, vFrom(0.0f, 0.0f, 0.0f)
	, vCurrent(0.0f, 0.0f, 0.0f)
	, vTo(0.0f, 0.0f, 0.0f)
	, fElapsed(0.0f)
	, fDir(1.0f)
	, bRotate(false)
{
}

SaveSample::~SaveSample()
{
}

#ifndef strlcpy
inline void strlcpy(char *dest, const char *src, size_t len)
{
	strncpy(dest, src, len);
	dest[len-1] = 0;
}
#endif

bool SaveSample::Initialize()
{
	IGameApp::Initialize();

	if (this->SaveSystemFlow())
	{
		MySlotStruct slot, slot2, slot3;
		memset(&slot, '\0', sizeof(slot));
		memset(&slot2, '\0', sizeof(slot2));
		memset(&slot3, '\0', sizeof(slot3));

		MySharedStruct shared;
		memset(&shared, '\0', sizeof(shared));

		for (u32 i = 0; i < MY_GAME_RANKING_SIZE; i++)
		{
			strlcpy(shared.arEntries[i].pcName, "Player 1", MY_GAME_NAME_LEN);

			shared.arEntries[i].iTime = shared.arEntries[i].iScore = 100 - (i * 10);
		}
		Log("Save file created.");

		strlcpy(slot.pcName, "Player 1", MY_GAME_NAME_LEN);
		slot.iLevel = 10;
		pSaveSystem->Save(1, &slot);

		strlcpy(slot.pcName, "Player 2", MY_GAME_NAME_LEN);
		slot.iLevel = 20;
		pSaveSystem->Save(2, &slot);

		strlcpy(slot.pcName, "Player 3", MY_GAME_NAME_LEN);
		slot.iLevel = 30;
		pSaveSystem->Save(3, &slot, &shared);

		pSaveSystem->Load(2, &slot2, &shared);
		pSaveSystem->Load(3, &slot3);
		if (memcmp(&slot, &slot2, sizeof(slot)) == 0)
		{
			Log("Equal");
		}
		else
		{
			Log("Different");
		}

		if (memcmp(&slot, &slot3, sizeof(slot)) == 0)
		{
			Log("Equal");
		}
		else
		{
			Log("Different");
		}
	}
	else
	{
		Log("Could not prepare the savesystem.");
	}

	return cPres.Load("save_sample.config", [&](Presentation *pres, Renderer *) {
		pCamera = pres->GetViewportByName("MainView")->GetCamera();
		pImage = (Image *)pres->GetRendererByName("MainRenderer")->GetScene()->GetChildByName("Panda");

		pSystem->AddListener(this);
		pInput->AddKeyboardListener(this);
		pInput->AddPointerListener(this);

		MySlotStruct slot;
		pSaveSystem->Load(0, &slot);

		auto v = Vector3f(slot.fPosX, slot.fPosY, pImage->GetPosition().getZ());
		vFrom = vTo = v;
		pImage->SetPosition(v);
	});
}

bool SaveSample::Update(Seconds dt)
{
	if (pImage)
	{
		fElapsed += dt;
		if (fElapsed > 1.0f)
			fElapsed = 1.0f;

		vCurrent = ((1.f - fElapsed) * vFrom) + (fElapsed * vTo);
		pImage->SetPosition(vCurrent);
		if (bRotate)
			pImage->SetRotation(pImage->GetRotation() + fDir);
	}

	return true;
}

bool SaveSample::Shutdown()
{
	MySlotStruct slot;
	pSaveSystem->Load(0, &slot);
	strlcpy(slot.pcName, "Panda Player", MY_GAME_NAME_LEN);
	slot.iLevel = 100;
	slot.fPosX = pImage->GetPosition().getX();
	slot.fPosY = pImage->GetPosition().getY();
	pSaveSystem->Save(0, &slot);

	cPres.Unload();
	pInput->RemovePointerListener(this);
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

void SaveSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

bool SaveSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
	else if (k == eKey::F1)
		pResourceManager->Print();
	else if (k == eKey::F2)
		pResourceManager->GarbageCollect();

	return true;
}

void SaveSample::OnInputPointerRelease(const EventInputPointer *ev)
{
	if (!pCamera)
		return;

	if (ev->GetReleased() == eInputButton::MouseLeft)
	{
		if (pImage)
			vFrom = pImage->GetPosition();

		vTo.setX(f32(ev->GetX()));
		vTo.setY(f32(ev->GetY()));
		vTo += pCamera->GetPosition();
		fElapsed = 0.0f;
	}
	else if (ev->GetReleased() == eInputButton::MouseRight)
	{
		bRotate = !bRotate;
	}
	else if (ev->GetReleased() == eInputButton::MouseUp)
	{
		fDir = 1.0f;
		bRotate = true;
	}
	else if (ev->GetReleased() == eInputButton::MouseDown)
	{
		fDir = -1.0f;
		bRotate = true;
	}
}

bool SaveSample::SaveSystemFlow() const
{
	MySlotStruct slot;
	memset(&slot, '\0', sizeof(slot));

	MySharedStruct shared;
	memset(&shared, '\0', sizeof(shared));

	pSaveSystem->SetTotalSlots(4);
	auto error = pSaveSystem->Initialize(eCartridgeSize::Small);
	if (error == eCartridgeError::None)
		error = pSaveSystem->Prepare(MY_GAME_ID, &slot, sizeof(slot), &shared, sizeof(shared));

	if (error == eCartridgeError::DeviceFull)
	{
		Log("Not enough space available on device.");
		return false;
	}

	if (error == eCartridgeError::InodeFull)
	{
		Log("Not enough inodes available on device.");
		return false;
	}

	if (error == eCartridgeError::NoCard || error == eCartridgeError::AccessDenied)
	{
		Log("Unknown file system error - no card or access denied - system hungup");
		pSystem->HangUp();
		return false;
	}

	if (error == eCartridgeError::NotFormatted)
	{
		Log("Save file doesn't exist or corrupted, creating one now.");
		error = pSaveSystem->FormatCard(&slot, &shared);
	}

	if (error == eCartridgeError::DataCorrupt)
	{
		Log("One or more saved games were corrupted and had to be reset");
		error = eCartridgeError::None;
	}

	if (error == eCartridgeError::FilesystemCorrupt)
	{
		Log("File system became corrupted - system hungup");
		pSystem->HangUp();
		return false;
	}

	if (error == eCartridgeError::None)
	{
		Log("Save data CRC ok.");
		// Do your initial loading here.
		return true;
	}

	return false;
}


