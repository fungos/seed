#include "Log.h"
#include "api/net/Socket.h"

#define TAG "[SocketUDP] "

namespace Seed { namespace Net
{

Socket::Socket()
	: cAddress()
	, iHandle(0)
	, bIsOpen(false)
{
	#if defined(_MSC_VER)
	// Initialize the socket
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2,2), &WsaData);
	#endif

	// Creating a socket
	iHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	#if defined(_MSC_VER)
	setsockopt(iHandle, SOL_SOCKET, SO_BROADCAST);
	#elif defined(__APPLE_CC__) || defined(__linux__)
	int optval = 1;
	setsockopt(iHandle, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
	#endif

	if (iHandle <= 0)
	{
		Log(TAG "failed to create socket");
	}
}

Socket::~Socket()
{
	#if defined(_MSC_VER)
	// Finalize the socket
	WSACleanup();
	#endif
}

bool Socket::Open(unsigned short port)
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons((unsigned short) port);

	if (bind(iHandle, (const sockaddr*) &address, sizeof(sockaddr_in)) < 0)
	{
		Log(TAG "failed to bind socket" );
		this->Close();
		bIsOpen = false;
	}
	else
	{
		// Setting the socket as non-blocking
		#if defined(_MSC_VER)
		DWORD nonBlocking = 1;
		if (ioctlsocket(handle, FIONBIO, &nonBlocking ) != 0)
		{
			Log(TAG "failed to set non-blocking socket");
			return false;
		}
		#elif defined(__APPLE_CC__) || defined(__linux__)
		int nonBlocking = 1;
		if (fcntl(iHandle, F_SETFL, O_NONBLOCK, nonBlocking ) == -1)
		{
			Log(TAG "failed to set non-blocking socket");
			return false;
		}
		#endif

		bIsOpen = true;
	}

	return bIsOpen;
}

void Socket::Close()
{
	#if defined(_MSC_VER)
	closesocket(iHandle);
	#elif defined(__APPLE_CC__) || defined(__linux__)
	shutdown(iHandle, SHUT_RDWR);
	#endif

	bIsOpen = false;
}

bool Socket::IsOpen() const
{
	return bIsOpen;
}

bool Socket::Send(const Address &destination, const void *data, int size)
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(destination.GetAddress());
	address.sin_port = htons((unsigned short) destination.GetPort());

	int sentBytes = sendto(iHandle, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));

	return sentBytes == size;
}

int Socket::Receive(Address &sender, void *data, int size)
{
	#if defined(_MSC_VER)
	typedef int socklen_t;
	#endif

	sockaddr_in from;
	socklen_t fromLength = sizeof(from);

	int receivedBytes = recvfrom(iHandle, (char*)data, size, 0, (sockaddr*)&from, &fromLength);

	if (receivedBytes <= 0)
		return 0;

	unsigned int address = ntohl(from.sin_addr.s_addr);
	unsigned int port = ntohs(from.sin_port);

	sender = Address(address, port);

	return receivedBytes;
}

}} // namespace
