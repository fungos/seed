#ifndef _NET_UDP_SOCKET_SAMPLE_H
#define _NET_UDP_SOCKET_SAMPLE_H

#include <Seed.h>
#include "api/net/Socket.h"
#include "api/net/Address.h"
#include <Box2D/Box2D.h>

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
		bool bIsFirstPlayer;

		struct PacketData
		{
			int iID;
			bool bAssignedPlayer1;
			bool bAssignedPlayer2;
			b2Vec2 ball;
			b2Vec2 player;
			b2Vec2 enemyPlayer;
		} sPacketData;
};

#endif // _NET_UDP_SOCKET_SAMPLE_H
