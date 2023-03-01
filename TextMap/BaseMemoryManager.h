#pragma once
const auto MEGABYTE = 1024*1024;


class BaseMemoryManager {
private:
	char* buffer;
	size_t capacity;
	size_t cur = 0;
	BaseMemoryManager* pNext = nullptr;

	BaseMemoryManager(size_t c) {
		capacity = c;
		buffer = new char[c];
	}

	void clear() {
		delete[] buffer;
		if (pNext != nullptr) pNext->clear();
	}

public:
	BaseMemoryManager() : BaseMemoryManager(MEGABYTE) {}
	~BaseMemoryManager() {
		clear();
	}

	void* allocate(size_t size) {
		if (cur + size > capacity) {
			if (pNext != nullptr) return pNext->allocate(size);
			if (size <= capacity)
				pNext = new BaseMemoryManager(capacity);
			else
				pNext = new BaseMemoryManager(capacity * ceil(size/capacity));
			return pNext->allocate(size);
		}
		size_t temp = cur;
		cur += size;
		return &buffer[temp];
	}

	void free(void* ptr) {}
};