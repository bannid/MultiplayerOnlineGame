#pragma once
#include <Windows.h>
#include "typedefines.h"
#include "tcp_common.h"

struct tsmessage_queue {
	message * Data;
	uint32 Size;
	uint32 AllocatedMemory;
	HANDLE Mutex;
	bool Initialized = false;
	void push_back(message Item);
	bool pop_back(message * Output);
	tsmessage_queue();
	~tsmessage_queue();
};