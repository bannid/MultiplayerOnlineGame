#include "TSMessageQueue.h"
#define SIZECALC(size) (size) * (sizeof(message))

void InitializeMessageQueue(tsmessage_queue * MessageQueue) {
	MessageQueue->AllocatedMemory = 1000;
	MessageQueue->Initialized = true;
	MessageQueue->Size = 0;
	MessageQueue->Data = (message*)VirtualAlloc(NULL, SIZECALC(MessageQueue->AllocatedMemory), MEM_COMMIT, PAGE_READWRITE);
	MessageQueue->Mutex = CreateMutexExA(
		NULL,
		NULL,
		NULL,
		MUTEX_ALL_ACCESS
	);
}

tsmessage_queue::tsmessage_queue() {
	InitializeMessageQueue(this);
}
tsmessage_queue::~tsmessage_queue() {
	VirtualFree((LPVOID)this->Data, SIZECALC(this->AllocatedMemory), MEM_RELEASE);
	CloseHandle(this->Mutex);
}

void tsmessage_queue::push_back(message Item) {
	DWORD Result = WaitForSingleObjectEx(this->Mutex, INFINITE, FALSE);
	assert(Result == WAIT_OBJECT_0);
	if (this->Size < this->AllocatedMemory) {
		this->Data[Size++] = Item;
	}
	else if (this->Size == this->AllocatedMemory) {
		message * TransactionalMemory = (message*)VirtualAlloc(NULL, SIZECALC(this->AllocatedMemory + this->AllocatedMemory * 2), MEM_COMMIT, PAGE_READWRITE);
		if (TransactionalMemory == NULL) {
			int LastErrorCode = GetLastError();
			std::cout << LastErrorCode;
		}
		CopyMemory(TransactionalMemory, this->Data, SIZECALC(this->AllocatedMemory));
		VirtualFree((LPVOID)this->Data, SIZECALC(this->AllocatedMemory), MEM_RELEASE);
		this->Data = TransactionalMemory;
		this->AllocatedMemory = this->AllocatedMemory + this->AllocatedMemory * 2;
		this->Data[Size++] = Item;
	}
	ReleaseMutex(this->Mutex);
}

bool tsmessage_queue::pop_back(message * Output) {
	DWORD Result = WaitForSingleObjectEx(this->Mutex, INFINITE, FALSE);
	assert(Result == WAIT_OBJECT_0);
	bool Success = false;
	if (this->Size > 0) {
		*Output = this->Data[Size - 1];
		this->Size--;
		Success = true;
	}
	ReleaseMutex(this->Mutex);
	return Success;
}
