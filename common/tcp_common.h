#pragma once
#include <assert.h>
#include <iostream>
#include "typedefines.h"

#define MAX_MESSAGE_SIZE (64) //Bytes
//Message types over the wire
enum message_types : uint16{
	hello
};
struct message_hello{
	message_types Type;
	const char Body[8] = "Hello";
};

struct message {
	uchar Data[MAX_MESSAGE_SIZE];
	uint32 Size = 0;
	template<typename DataType>
	message& operator<<(DataType Data) {
		assert(this->Size < MAX_MESSAGE_SIZE);
		uchar * Ptr = (uchar*)&Data;
		for (uint32 i = 0; i < sizeof(DataType); i++) {
			this->Data[this->Size] = *(Ptr + i);
			this->Size++;
		}
		assert(this->Size <= MAX_MESSAGE_SIZE);
		return *this;
	}
	template<typename DataType>
	message& operator>>(DataType & Output) {
		assert(this->Size > 0);
		DataType * Ptr = (DataType*)&this->Data[this->Size - sizeof(DataType)];
		Output = *Ptr;
		this->Size -= sizeof(DataType);
		assert(this->Size >= 0);
		return *this;
	}
};
