#include "net_udp_socket_sample.h"

NetUDPSocketSample::NetUDPSocketSample()
	: iPort(3000)
{
	vPlayer = VECTOR_ZERO;
	vEnemyPlayer = VECTOR_ZERO;
	sPacketData.ball = VECTOR_ZERO;
	sPacketData.vRemotePlayer = VECTOR_ZERO;
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

	int bytesRead = 0;

	while(!bytesRead)
	{
		cSocket.Send(Address(127, 0, 0, 1, iPort), &sPacketData, sizeof(sPacketData));

		Address sender;
		bytesRead = cSocket.Receive(sender, &sPacketData, sizeof(sPacketData));

		Log("Waiting ...");
	}

	return IGameApp::Initialize();
}

bool NetUDPSocketSample::Update(f32 dt)
{
	UNUSED(dt)

	Address sender;
	cSocket.Receive(sender, &sPacketData, sizeof(sPacketData));

	vEnemyPlayer = sPacketData.vRemotePlayer;
	sPacketData.vRemotePlayer = vPlayer;

	Log("Ball position (x:%f, y:%f)", sPacketData.ball.x, sPacketData.ball.y);
	Log("Player position (x:%f, y:%f)", vPlayer.x,vPlayer.y);
	Log("Enemy player position (x:%f, y:%f)", vEnemyPlayer.x, vEnemyPlayer.y);

	cSocket.Send(Address(127, 0, 0, 1, iPort), &sPacketData, sizeof(sPacketData));

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
		vPlayer += VECTOR_UP;
	else if (k == Seed::KeyDown)
		vPlayer += VECTOR_DOWN;
}

