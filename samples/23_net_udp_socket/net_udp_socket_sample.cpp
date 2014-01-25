#include "net_udp_socket_sample.h"

NetUDPSocketSample::NetUDPSocketSample()
	: sPacketData()
	, cSocket()
	, vPlayer()
	, vEnemyPlayer()
	, iPort(3000)
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

	auto bytesRead = 0;
	cSocket.Send(Address(127, 0, 0, 1, iPort), &sPacketData, sizeof(sPacketData));

	while (!bytesRead)
	{
		Address sender;
		bytesRead = cSocket.Receive(sender, &sPacketData, sizeof(sPacketData));
		Log("Waiting ...");
	}

	if (bytesRead)
		cSocket.Send(Address(127, 0, 0, 1, iPort), &sPacketData, sizeof(sPacketData));

	return IGameApp::Initialize();
}

bool NetUDPSocketSample::Update(Seconds dt)
{
	UNUSED(dt)

	Address sender;
	auto bytesRead = cSocket.Receive(sender, &sPacketData, sizeof(sPacketData));

	if (bytesRead)
		vEnemyPlayer = sPacketData.vRemotePlayer;

	Log("Player position (x:%f, y:%f)", vPlayer.x,vPlayer.y);
	Log("Enemy player position (x:%f, y:%f)", vEnemyPlayer.x, vEnemyPlayer.y);

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

bool NetUDPSocketSample::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	auto k = ev->GetKey();

	if (k == eKey::Escape)
		pSystem->Shutdown();
	else if (k == eKey::F1)
		pResourceManager->Print();
	else if (k == eKey::F2)
		pResourceManager->GarbageCollect();
	else if (k == eKey::Up)
	{
		vPlayer += VECTOR_UP;
		sPacketData.vRemotePlayer = vPlayer;
		cSocket.Send(Address(127, 0, 0, 1, iPort), &sPacketData, sizeof(sPacketData));
	}
	else if (k == eKey::Down)
	{
		vPlayer += VECTOR_DOWN;
		sPacketData.vRemotePlayer = vPlayer;
		cSocket.Send(Address(127, 0, 0, 1, iPort), &sPacketData, sizeof(sPacketData));
	}

	return true;
}
