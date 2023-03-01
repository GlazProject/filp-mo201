#pragma once
#include <new>


#pragma pack(push, 1)
struct CItem {
	size_t size;
	union
	{
		struct
		{
			CItem* pNext;
			CItem* pPrev;
		};

		char data[1];
	};
};
#pragma pack(pop)

class MemoryManagerWithFree {
private:
	CItem* pHead;
	const size_t sizeBuffer = 1024 * 1024;
	size_t capacity;

	void removeItem(CItem* pDelete) {
		if (pDelete->pNext == pDelete) {
			pHead = nullptr;
			return;
		}

		CItem* pPrev = pDelete->pPrev;
		CItem* pNext = pDelete->pNext;
		pPrev->pNext = pNext;
		pNext->pPrev = pPrev;
		if (pHead == pDelete) pHead = pNext;
	}

	void insertItem(CItem* pItem) {
		if (pHead == nullptr) {
			pHead = pItem;
			pHead->pNext = pHead->pPrev = pHead;
			return;
		}

		CItem* pNext = pHead->pNext;
		pHead->pNext = pItem;
		pNext->pPrev = pItem;
		pItem->pNext = pNext;
		pItem->pPrev = pHead;
	}

public:
	MemoryManagerWithFree() {
		capacity = sizeBuffer;
		char* buffer = new char[capacity];
		pHead = (CItem*)buffer;
		pHead->size = capacity - sizeof(size_t);
		pHead->pNext = pHead->pPrev = pHead;
	}

	~MemoryManagerWithFree() {
		delete[] pHead;
	}

	void* allocate(size_t size) {
		if (pHead == nullptr)
			throw std::bad_alloc();

		size_t requiredSize = size + sizeof(size_t);
		CItem* pCur = pHead;
		while (true) {
			if (pCur->size >= requiredSize) {
				break;
			}
			pCur = pCur->pNext;
			if (pCur == pHead)
				throw std::bad_alloc();
		}
		removeItem(pCur);

		if (pCur->size >= requiredSize + sizeof(CItem)) {
			CItem* half = (CItem*)((char*)pCur + requiredSize);
			half->size = pCur->size - requiredSize;
			pCur->size = requiredSize;
			insertItem(half);
		}

		return pCur->data;
	}

	void free(void* ptr) {
		CItem* pItem = (CItem*)((char*)ptr - sizeof(size_t));
		insertItem(pItem);

		CItem* pLeft = nullptr;
		CItem* pRight = nullptr;

		CItem* pCur = pHead;
		do {
			if ((CItem*)((char*)pCur + pCur->size) == pItem)
				pLeft = pCur;

			if ((CItem*)((char*)pItem + pItem->size) == pCur)
				pRight = pCur;

			pCur = pCur->pNext;
		} while (pCur != pHead);

		if (pLeft != nullptr) {
			removeItem(pItem);
			pLeft->size += pItem->size;
			pItem = pLeft;
		}
		
		if (pRight != nullptr) {
			removeItem(pRight);
			pItem->size += pRight->size;
		}
	}

};