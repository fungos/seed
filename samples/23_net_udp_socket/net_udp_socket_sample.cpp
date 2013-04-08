#include "net_udp_socket_sample.h"

NetUDPSocketSample::NetUDPSocketSample()
	: iPort(3000)
	, bIsFirstPlayer(false)
{
	sPacketData.ball = VECTOR_ZERO;
	sPacketData.player = VECTOR_ZERO;
	sPacketData.enemyPlayer = VECTOR_ZERO;
	sPacketData.bAssignedPlayer1 = false;
	sPacketData.bAssignedPlayer2 = false;
}

NetUDPSocketSample::~NetUDPSocketSample()
{
}

bool NetUDPSocketSample::Initialize()
{
	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	// Create socket
	cSocket.Open(iPort);

	return IGameApp::Initialize();
}

bool NetUDPSocketSample::Update(f32 dt)
{
	UNUSED(dt)

	cSocket.Send(Address(10, 0, 1, 4, iPort), &sPacketData, sizeof(sPacketData));

	Address sender;
	int bytesRead = cSocket.Receive(sender, &sPacketData, sizeof(sPacketData));

	if (!bytesRead)
	{
		if(!sPacketData.bAssignedPlayer1 && bIsFirstPlayer)
		{
			sPacketData.bAssignedPlayer1 = true;
			bIsFirstPlayer = true;
		}

		Log("Waiting for player connection ...");
		return false;
	}
	else
	{
		if(!sPacketData.bAssignedPlayer2)
		{
			sPacketData.bAssignedPlayer2 = true;
		}
	}

	Log("received packet from %d.%d.%d.%d:%d (%d bytes)",
		sender.GetA(), sender.GetB(), sender.GetC(), sender.GetD(),
		sender.GetPort(), bytesRead);

	Log("Ball position (x:%f, y:%f)", sPacketData.ball.x, sPacketData.ball.y);
	Log("Player position (x:%f, y:%f)", sPacketData.player.x, sPacketData.player.y);
	Log("Enemy player position (x:%f, y:%f)", sPacketData.enemyPlayer.x, sPacketData.enemyPlayer.y);

	return true;
}

bool NetUDPSocketSample::Shutdown()
{
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	// Close socket
	cSocket.Close();

	return IGameApp::Shutdown();
}

void NetUDPSocketSample::OnSystemShutdown(const EventSystem *ev)
{
	UNUSED(ev)
	pSystem->Shutdown();
}

void NetUDPSocketSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (k == Seed::KeyEscape)
		pSystem->Shutdown();
	else if (k == Seed::KeyF1)
		pResourceManager->Print();
	else if (k == Seed::KeyF2)
		pResourceManager->GarbageCollect();
	else if (k == Seed::KeyUp)
		(bIsFirstPlayer) ? sPacketData.player += VECTOR_UP : sPacketData.enemyPlayer += VECTOR_UP;
	else if (k == Seed::KeyDown)
		(bIsFirstPlayer) ? sPacketData.player += VECTOR_DOWN : sPacketData.enemyPlayer += VECTOR_DOWN;
}

