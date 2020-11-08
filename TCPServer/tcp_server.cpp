#include "tcp_server.h"

static int ListenOnPort(const char* Port, int Prototype, SOCKET *ListenSocket) {
	switch (Prototype) {
	case IPPROTO_TCP: {
		struct addrinfo *Result = NULL;
		struct addrinfo Hints;
		int ErrorCode;
		ZeroMemory(&Hints, sizeof(Hints));
		Hints.ai_family = AF_INET;
		Hints.ai_socktype = SOCK_STREAM;
		Hints.ai_protocol = IPPROTO_TCP;
		Hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		ErrorCode = getaddrinfo(NULL, Port, &Hints, &Result);
		if (ErrorCode != 0) {
			WSACleanup();
			return ErrorCode;
		}
		*ListenSocket = socket(Result->ai_family, Result->ai_socktype, Result->ai_protocol);
		if (*ListenSocket == INVALID_SOCKET) {
			ErrorCode = WSAGetLastError();
			return ErrorCode;
		}
		ErrorCode = bind(*ListenSocket, Result->ai_addr, (int)Result->ai_addrlen);
		if (ErrorCode == SOCKET_ERROR) {
			ErrorCode = WSAGetLastError();
			freeaddrinfo(Result);
			closesocket(*ListenSocket);
			WSACleanup();
			return ERROR_SUCCESS;
		}
		freeaddrinfo(Result);
		return ErrorCode;
	}
	case IPPROTO_UDP: {
		//We are using IPPROTO_UDP for creating unbound
		//sockets to interrupt blocking select method.
		*ListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (*ListenSocket == INVALID_SOCKET) {
			return -1;
		}
		return ERROR_SUCCESS;
	}
	default: {
		return -1;
	}
	}
}

connection_acceptor::connection_acceptor(const char* Port) :Port(Port) {}

bool connection_acceptor::start() {
	SOCKET ClientSocket;
	SOCKET ListenSocket;
	int ErrorCode;
	WSADATA WSAData;
	ErrorCode = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (ErrorCode != 0) {
		return ErrorCode;
	}
	ListenOnPort(Port, IPPROTO_TCP, &ListenSocket);
	if (ErrorCode != 0)return false;
	SOCKADDR_IN AddrIn;
	int AddrLength = sizeof(AddrIn);
	while (true) {
		ErrorCode = listen(ListenSocket, SOMAXCONN);
		if (ErrorCode == SOCKET_ERROR) {
			ErrorCode = WSAGetLastError();
			closesocket(ListenSocket);
			return false;
		}
		// Accept a client socket
		OutputDebugStringA("Listening on Socket");
		ClientSocket = accept(ListenSocket, (SOCKADDR*)&AddrIn, &AddrLength);
		if (ClientSocket == INVALID_SOCKET) {
			closesocket(ListenSocket);
			return false;
		}
		//Make socket non-blocking
		u_long mode = 1;
		if (ioctlsocket(ClientSocket, FIONBIO, &mode) != 0) {
			ErrorCode = WSAGetLastError();
			closesocket(ListenSocket);
			return false;
		}
		bool HasSlotsInCurrentProcessors = false;
		uint32 IndexOfConnectionProcessor = 0;
		for (int i = 0; i < this->NumberOfConnectionProcessors; i++) {
			HasSlotsInCurrentProcessors = this->ConnectionProcessors[i]->ActiveConnectionsCount < MAX_ACTIVE_CONNECTIONS_PER_PROCESSOR;
			if (HasSlotsInCurrentProcessors) {
				IndexOfConnectionProcessor = i;
				break;
			}
		}
		if (HasSlotsInCurrentProcessors) {
			connection_processor * Ptr = this->ConnectionProcessors[IndexOfConnectionProcessor];
			message Message;
			Message << ClientSocket << message_bw_threads::CREATE_NEW_CONNECTION;
			Ptr->push_message(Message);
			continue;
		}
		//If HasSlotsInCurrentProcessors is false and we are at maximum capacity
		if (this->NumberOfConnectionProcessors == MAX_CONNECTION_PROCESSOR) {
			//TODO:
			char buffer[] = "FULL";
			send(ClientSocket, buffer, ARRAYSIZE(buffer), 0);
			shutdown(ClientSocket, SD_SEND);
			closesocket(ClientSocket);
		}
		else if (this->NumberOfConnectionProcessors < MAX_CONNECTION_PROCESSOR) {
			this->ConnectionProcessors[this->NumberOfConnectionProcessors++] = new connection_processor;
			connection_processor * Ptr = this->ConnectionProcessors[this->NumberOfConnectionProcessors - 1];
			Ptr->Initialize(this->ConnectionProcessorsID++);
			message Message;
			Message << ClientSocket << message_bw_threads::CREATE_NEW_CONNECTION;
			Ptr->push_message(Message);
		}
	}
	return true;
}

void connection_acceptor::stop() {
	for (int i = 0; i < this->NumberOfConnectionProcessors; i++) {
		this->ConnectionProcessors[i]->stop();
		delete this->ConnectionProcessors[i];
	}
}

void connection_processor::push_message(message Message) {
	this->MessageQueue.push_back(Message);
	ReleaseSemaphore(this->MessageQueueSemaphore, 1, 0);
}
//Connection processor
static void ProcessMessages(connection_processor * ConnectionProcessor) {
	while(ConnectionProcessor->Running) {
		WaitForSingleObjectEx(ConnectionProcessor->MessageQueueSemaphore, INFINITE, FALSE);
		message Message;
		while (ConnectionProcessor->MessageQueue.pop_back(&Message)) {
			message_bw_threads MessageType;
			Message >> MessageType;
			switch (MessageType) {
			case message_bw_threads::CREATE_NEW_CONNECTION: {
				SOCKET Socket;
				Message >> Socket;
				std::cout << "client received with socket id: " << Socket;
				ConnectionProcessor->Sockets[ConnectionProcessor->ActiveConnectionsCount] = Socket;
				FD_SET(Socket, &ConnectionProcessor->ReadSet);
				server_connection* Ptr = &ConnectionProcessor->Connections[ConnectionProcessor->ActiveConnectionsCount];
				Ptr->Socket = Socket;
				Ptr->Id = ConnectionProcessor->ConnectionsID++;
				Ptr->IsActive = true;
				//For preventing race conditions, only update the count
				//after the client has been added.
				ConnectionProcessor->ActiveConnectionsCount++;
				//We are going to spin lock on this variable for it to be true.
				//This variable is changed by another thread and we need to make
				//sure that while other thread is creating the NewConnectionListener,
				//we dont close the NewConnectionListener, otherwise, the receiving thread would never be
				//notified of new incoming connections.
				while (!ConnectionProcessor->NewConnectionListenerSocketValid);
				//This is the only place where we set this flag to be false.
				ConnectionProcessor->NewConnectionListenerSocketValid = false;
				//This call will make select to wake up in ReceiveMessages wake up.
				closesocket(ConnectionProcessor->NewConnectionListener);
				break;
			}
			}
		}
	}
}
static void ReceiveMessages(connection_processor * ConnectionProcessor) {
	while(ConnectionProcessor->Running) {
		FD_SET Set = ConnectionProcessor->ReadSet;
		select(ConnectionProcessor->ActiveConnectionsCount, &Set, NULL, NULL, NULL);
		if (FD_ISSET(ConnectionProcessor->NewConnectionListener, &Set)) {
			if (ListenOnPort("", IPPROTO_UDP, &ConnectionProcessor->NewConnectionListener) != ERROR_SUCCESS) {
				std::cout << "[Debug][FatalError]Could not open socket for listening new connection";
				exit(-1);
			}
			FD_SET Set;
			FD_ZERO(&Set);
			ConnectionProcessor->ReadSet = Set;
			for (int i = 0; i < ConnectionProcessor->ActiveConnectionsCount; i++) {
				FD_SET(ConnectionProcessor->Sockets[i], &ConnectionProcessor->ReadSet);
			}
			FD_SET(ConnectionProcessor->NewConnectionListener, &ConnectionProcessor->ReadSet);
			//This is the only place where we set this flag to be true except 
			//connection_processor::initialize() function.
			ConnectionProcessor->NewConnectionListenerSocketValid = true;
		}
		for (int i = 0; i < ConnectionProcessor->ActiveConnectionsCount; i++) {
			if (FD_ISSET(ConnectionProcessor->Connections[i].Socket, &Set)) {
				server_connection *ConnectionPtr = ConnectionProcessor->Connections + i;
				uchar Buffer[READ_BUFFER_SIZE];// = ConnectionPtr->ByteStream + ConnectionPtr->BytesStored;
				int BytesReceived = recv(ConnectionPtr->Socket , (char*)Buffer, READ_BUFFER_SIZE, 0);
				for (int j = 0; j < BytesReceived; j++) {
					std::cout << Buffer[j];
				}
				if (BytesReceived == 0) {
					std::cout << "Socket with ID: " << ConnectionPtr->Socket << " closed";
					std::cout << "\n";
					ConnectionProcessor->close_socket(ConnectionPtr->Socket);
				}
				else if (BytesReceived == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
					std::cout << "Socket with ID: " << ConnectionPtr->Socket << "failed with: " << WSAGetLastError() << std::endl;
					ConnectionProcessor->close_socket(ConnectionPtr->Socket);
				}
			}
		}
	}
}
void connection_processor::close_socket(SOCKET Socket) {
	uint32 ActiveConnections = this->ActiveConnectionsCount;
	for (int i = 0; i < ActiveConnections; i++) {
		if (this->Connections[i].Socket == Socket) {
			this->Connections[i] = this->Connections[ActiveConnections - 1];
			this->Sockets[i] = this->Sockets[ActiveConnections - 1];
			this->ActiveConnectionsCount--;
			assert(((int)this->ActiveConnectionsCount) >= 0);
			FD_CLR(Socket, &this->ReadSet);
			if (Socket != this->NewConnectionListener) {
				shutdown(Socket, SD_BOTH);
				closesocket(Socket);
			}
		}
	}
}
void connection_processor::Initialize(uint32 ID) {
	this->Running = true;
	if (ListenOnPort("", IPPROTO_UDP, &NewConnectionListener) != ERROR_SUCCESS) {
		std::cout << "[Debug][FatalError]Could not open socket for listening new connection";
		exit(-1);
	}
	else {
		FD_ZERO(&this->ReadSet);
		FD_SET(NewConnectionListener, &this->ReadSet);
		NewConnectionListenerSocketValid = true;
	}
	this->ID = ID;
	this->MessageQueueSemaphore = CreateSemaphoreEx(NULL, 0, 250, NULL, 0, SEMAPHORE_ALL_ACCESS);
	this->ConnectionAccepterThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ProcessMessages, (LPVOID)this, 0, 0);
	this->ReceiverThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReceiveMessages, (LPVOID)this, 0, 0);
}

void connection_processor::stop() {
	this->Running = false;
	HANDLE ThreadsToWait[] = { this->ConnectionAccepterThread,this->ReceiverThread };
	WaitForMultipleObjectsEx(ARRAYSIZE(ThreadsToWait), ThreadsToWait, TRUE, INFINITE,FALSE);
}
