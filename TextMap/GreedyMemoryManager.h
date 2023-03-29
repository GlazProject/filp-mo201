#pragma once

class GreedyMemoryManager {
private:
	char* buffer;
	const size_t sizeBuffer = 1024;
	size_t capacity;
	size_t cur = sizeof(char*);

public:
	GreedyMemoryManager() {
		capacity = sizeBuffer;
		buffer = new char[capacity];
		*(char**)buffer = nullptr;
	}

	~GreedyMemoryManager() {
		while (buffer != nullptr)
		{
			char* pNextBuffer = *(char**)buffer;
			delete[] buffer;
			buffer = pNextBuffer;
		};
	}

	void* allocate(size_t size) {
		if (cur + size > capacity) {
			capacity = max(sizeBuffer, size + sizeof(char*));
			char* newBuffer = new char[capacity];
			*(char**)newBuffer = buffer;
			buffer = newBuffer;
			cur = sizeof(char*);
		}
		size_t temp = cur;
		cur += size;
		return &buffer[temp];
	}
	void free(void* ptr) {

	}

};