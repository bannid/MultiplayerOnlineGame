#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
#include "win32_includes.h"
#include <TSMessageQueue.h>
#include <WS2tcpip.h>
#include <tcp_common.h>

#define CONNECTION_CLOSED 0

struct client {
	SOCKET ConnectionSocket;
	HANDLE ReceiverThread = NULL;
	HANDLE ReceiverThreadSemaphore = NULL;
	volatile bool Running;
	tsmessage_queue IncomingMessages;
	tsmessage_queue OutgoingMessages;
};

bool initialize_client(client * Client);
int client_connect_to_server(client * Client,
							 const char* ServerIP,
							 const char* PortNumber);
void run_client(client * Client);
void client_on_server_disconnect(client * Client);
void stop_client(client * Client);

#endif