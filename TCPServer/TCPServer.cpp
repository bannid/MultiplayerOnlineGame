#define WIN32_LEAN_AND_MEAN
#include "tcp_server.h"
int main() {
#if 1
	connection_acceptor Acceptor("27015");
	Acceptor.start();
	Acceptor.stop();
#endif
}