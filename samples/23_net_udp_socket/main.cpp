#include "net_udp_socket_sample.h"

int main(int argc, char **argv)
{
	return SeedRun<NetUDPSocketSample>(argc, argv, "net_udp_socket_app.config");
}
