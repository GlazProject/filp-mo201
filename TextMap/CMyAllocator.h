#pragma once
//#include "BaseMemoryManager.h"
//#include "MemoryManagerWithFree.h"
//#include "GreedyMemoryManager.h"
#include "MemoryManagerTwins.h"

// static GreedyMemoryManager memoryManager;
// static BaseMemoryManager memoryManager;
// static MemoryManagerWithFree memoryManager;
static MemoryManagerTwins memoryManager;

template <class T>
class CMyAllocator
{
public:
	typedef typename T value_type;

	CMyAllocator()
	{

	}

	template <class U>
	CMyAllocator(const CMyAllocator<U>& V)
	{

	}

	T* allocate(size_t Count)
	{
		//printf("Allocate %d\n", (int)(Count * sizeof(T)));

		//return (T*)malloc(sizeof(T) * Count);
		return (T*)memoryManager.allocate(sizeof(T) * Count);
	}

	void deallocate(T* V, size_t Count)
	{
		//printf("Free %d\n", (int)(Count * sizeof(T)));

		memoryManager.free(V);
	}
};