#ifndef _NET_UDP_SOCKET_SAMPLE_H
#define _NET_UDP_SOCKET_SAMPLE_H

#include <Seed.h>
#include <Box2D/Box2D.h>
#include "api/net/Socket.h"
#include "api/net/Address.h"

#define VECTOR_UP		b2Vec2(0, -1)
#define VECTOR_DOWN		b2Vec2(0, 1)
#define VECTOR_LEFT		b2Vec2(-1, 0)
#define VECTOR_RIGHT	b2Vec2(1, 0)
#define VECTOR_ZERO		b2Vec2(0, 0)

using namespace Seed;
using namespace Seed::Net;

class NetUDPSocketSample : public IGameApp,
							public IEventSystemListener,
							public IEventInputKeyboardListener
{
	SEED_DISABLE_COPY(NetUDPSocketSample)

	public:
		NetUDPSocketSample();
		virtual ~NetUDPSocketSample();

		virtual bool Initialize();
		virtual bool Update(Seconds dt);
		virtual bool Shutdown();

		// IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev) override;

		// IEventInputKeyboardListener
		virtual bool OnInputKeyboardRelease(const EventInputKeyboard *ev) override;

	private:
		struct PacketData
		{
			b2Vec2 ball;
			b2Vec2 vRemotePlayer;
		} sPacketData;

		Socket cSocket;

		b2Vec2 vPlayer;
		b2Vec2 vEnemyPlayer;

		u32 iPort;
};

#endif // _NET_UDP_SOCKET_SAMPLE_H
