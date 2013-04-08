#include "net_udp_socket_sample.h"

NetUDPSocketSample::NetUDPSocketSample()
	: iPort(0)
{
}

NetUDPSocketSample::~NetUDPSocketSample()
{
}

bool NetUDPSocketSample::Initialize()
{
	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	// Create socket
	iPort = 3000;
	cSocket.Open(iPort);

	return IGameApp::Initialize();
}

bool NetUDPSocketSample::Update(f32 dt)
{
	UNUSED(dt)

	const char data[] = "hello world!";

	cSocket.Send(Address(127, 0, 0, 1, iPort), data, sizeof(data));

	Address sender;
	unsigned char buffer[256];
	int bytes_read = cSocket.Receive(sender, buffer, sizeof( buffer ));
	if (!bytes_read)
		return false;

	Log("received packet from %d.%d.%d.%d:%d (%d bytes)",
		sender.GetA(), sender.GetB(), sender.GetC(), sender.GetD(),
		sender.GetPort(), bytes_read);

	return true;
}

bool NetUDPSocketSample::Shutdown()
{
	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);



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
}
