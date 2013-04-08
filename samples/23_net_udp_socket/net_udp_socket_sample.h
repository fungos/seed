#ifndef _NET_UDP_SOCKET_SAMPLE_H
#define _NET_UDP_SOCKET_SAMPLE_H

#include <Seed.h>
#include "api/net/Socket.h"
#include "api/net/Address.h"

using namespace Seed;
using namespace Seed::Net;

class NetUDPSocketSample : public IGameApp,
							public IEventSystemListener,
							public IEventInputKeyboardListener
{
	public:
		NetUDPSocketSample();
		virtual ~NetUDPSocketSample();

		virtual bool Initialize();
		virtual bool Update(f32 dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

	private:
		SEED_DISABLE_COPY(NetUDPSocketSample);
		Socket cSocket;
		int iPort;
};

#endif // _NET_UDP_SOCKET_SAMPLE_H
