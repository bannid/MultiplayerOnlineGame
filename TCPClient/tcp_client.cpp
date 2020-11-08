#include "tcp_client.h"

static void receive_messages_for_client(client *Client) {
	WaitForSingleObjectEx(Client->ReceiverThreadSemaphore, INFINITE, FALSE);
	for (;;) {
		if (Client->Running) {
			char Buffer[512];
			int BytesReceived = recv(Client->ConnectionSocket, Buffer, 512, 0);
			if(BytesReceived == CONNECTION_CLOSED ||
			   BytesReceived == SOCKET_ERROR){
				break;
			}
		}
	}
	client_on_server_disconnect(Client);
}

bool initialize_client(client * Client) {
	WSADATA WsaData;
	for (;;) {
		Client->ReceiverThread = CreateThread(
			NULL,
			NULL,
			(LPTHREAD_START_ROUTINE)receive_messages_for_client,
			Client,
			NULL,
			NULL);
		Client->ReceiverThreadSemaphore = CreateSemaphoreExA(NULL,
															 0,
															 1,
															 NULL,
															 NULL,
															 SEMAPHORE_ALL_ACCESS);
		if (Client->ReceiverThread == NULL ||
			Client->ReceiverThreadSemaphore == NULL) {
			break;
		}
		return (WSAStartup(MAKEWORD(2, 2), &WsaData) == ERROR_SUCCESS);
	}
	return false;
}

int client_connect_to_server(client * Client,
							 const char *ServerIP,
							 const char *Port) {
	int ErrorCode;
	addrinfo *Result = NULL, Hints;
	ZeroMemory(&Hints, sizeof(Hints));
	Hints.ai_family = AF_UNSPEC;
	Hints.ai_socktype = SOCK_STREAM;
	Hints.ai_protocol = IPPROTO_TCP;
	// Resolve the server address and port
	ErrorCode = getaddrinfo(ServerIP, Port, &Hints, &Result);
	if (ErrorCode != 0) {
		OutputDebugStringA("Failed to connect to server!!");
		return ErrorCode;
	}
	// Create a SOCKET for connecting to server
	Client->ConnectionSocket = socket(Result->ai_family,
									  Result->ai_socktype,
									  Result->ai_protocol);
	if (Client->ConnectionSocket == INVALID_SOCKET) {
		ErrorCode = WSAGetLastError();
		OutputDebugStringA("Failed to connect to server!!");
		return ErrorCode;
	}
	// Connect to server.
	ErrorCode = connect(Client->ConnectionSocket,
						Result->ai_addr,
						(int)Result->ai_addrlen);
	if (ErrorCode == SOCKET_ERROR) {
		freeaddrinfo(Result);
		return ErrorCode;
	}
	
	freeaddrinfo(Result);
	if (Client->ConnectionSocket == INVALID_SOCKET) {
		OutputDebugStringA("Failed to connect to server!!");
		return SOCKET_ERROR;
	}
	return ERROR_SUCCESS;
}

void run_client(client * Client) { 
	Client->Running = true; 
	ReleaseSemaphore(Client->ReceiverThreadSemaphore,
					 1,
					 0);
}

void client_on_server_disconnect(client * Client) { 
	Client->Running = false; 
}

void stop_client(client * Client) {
	shutdown(Client->ConnectionSocket, SD_BOTH);
	closesocket(Client->ConnectionSocket);
	// We dont care here if this function succeeds. Give one second
	// to thread to finish its execution.
	WaitForSingleObjectEx(Client->ReceiverThread,
						  1000,
						  FALSE);
	CloseHandle(Client->ReceiverThreadSemaphore);
	WSACleanup();
}