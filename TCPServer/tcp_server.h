#pragma once

#define MAX_ACTIVE_CONNECTIONS_PER_PROCESSOR 250
#define FD_SETSIZE (MAX_ACTIVE_CONNECTIONS_PER_PROCESSOR + 1)
#define MAX_CONNECTION_PROCESSOR 4
#define WORKER_THREADS_PER_CONNECTION_PROCESSOR 5
#define READ_BUFFER_SIZE 512

#include <WS2tcpip.h>
#include <typedefines.h>
#include <TSMessageQueue.h>



struct connection_acceptor;
struct connection_processor;
struct server_connection;

enum message_bw_threads {
	CREATE_NEW_CONNECTION
};
struct server_connection {
	SOCKET Socket;
	uint32 Id;
	bool IsActive = false;
	tsmessage_queue MessageQueue;
	uchar ByteStream[READ_BUFFER_SIZE];
	uint32 BytesStored = 0;
};

struct connection_acceptor {
	const char* Port;
	HANDLE AcceptorThread;
	uint32 ConnectionProcessorsID = 0;
	connection_processor * ConnectionProcessors[MAX_CONNECTION_PROCESSOR];
	uint32 NumberOfConnectionProcessors = 0;
	connection_acceptor(const char* Port);
	bool start();
	void stop();
};

struct connection_processor {
	volatile bool Running;
	//Id of this connectionProcessor.
	//This ID must be unique and is 
	//set by the connection acceptor
	uint32 ID;
	server_connection Connections[MAX_ACTIVE_CONNECTIONS_PER_PROCESSOR];
	//Read set for select to check for sockets ready for read.
	FD_SET ReadSet;
	//All the sockets
	SOCKET Sockets[FD_SETSIZE];
	//The Listener thread which is listening to new clients
	volatile bool NewConnectionListenerSocketValid;
	SOCKET NewConnectionListener;
	volatile uint32 ActiveConnectionsCount = 0;
	
	HANDLE ReceiverThread;
	HANDLE ConnectionAccepterThread;
	//Semaphore for incoming messages from other components
	HANDLE MessageQueueSemaphore;
	
	tsmessage_queue MessageQueue;
	uint32 ConnectionsID = 0;
	void push_message(message message);
	void Initialize(uint32 ID);
	void close_socket(SOCKET Socket);
	void stop();
};
