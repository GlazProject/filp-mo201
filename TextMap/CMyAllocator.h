#pragma once

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

		return (T*)malloc(sizeof(T) * Count);
	}

	void deallocate(T* V, size_t Count)
	{
		//printf("Free %d\n", (int)(Count * sizeof(T)));

		free(V);
	}
};