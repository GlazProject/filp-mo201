#pragma once
#include <new>


#pragma pack(push, 1)
struct CItem {
	unsigned char level;
	bool free;
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

template<class T>
class CList {
	T* pHead;

public:
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

	CItem* getHead() {
		return pHead;
	}
};

class MemoryManagerTwins{
private:
	const static int MAX_LEVEL = 24;
	const size_t sizeBuffer = 1 << MAX_LEVEL;
	CList<CItem> lists[MAX_LEVEL + 1];
	char* buffer;
	size_t mask = 0;

	CItem* getBuddy(CItem* item) {
		const size_t dist = ((char*)item - buffer);
		const size_t blockSize = static_cast<size_t>(1) << item->level;

		size_t blockNum = dist / blockSize;

		if (blockNum % 2 == 0) {
			// right
			return (CItem*)((char*)item + blockSize);
		}

		return (CItem*)((char*)item - blockSize);
	}

	
public:
	MemoryManagerTwins() {
		buffer = new char[sizeBuffer];
		CItem* maxItem = (CItem*)buffer;
		maxItem->level = MAX_LEVEL;
		insertItem(maxItem);
	}

	~MemoryManagerTwins() {
		delete[] lists;
	}

	void* allocate(size_t size) {
		size_t requiredSize = size + sizeof(unsigned char) + sizeof(bool);
		unsigned long index;

		BitScanReverse(&index, requiredSize);
		index = 1 << (index + 1);
		index = ~(index - 1);
		BitScanForward(&index, mask & index);

		size_t blockSize = 1 << index;
		if (lists[index].getHead() != nullptr && blockSize >= size) {
			CItem* block = lists[index].getHead();
			removeItem(block);

			while (true) {
				if (blockSize / 2 >= requiredSize) {
					blockSize /= 2;
					block->level--;
					block->free = false;

					CItem* right = (CItem*)((char*)block + blockSize);
					right->level = block->level;

					insertItem(right);
				}
				else
					return block->data;
			}
		}
		throw std::bad_alloc();
	}

	void free(void* ptr) {
		CItem* pItem = (CItem*)((char*)ptr - sizeof(unsigned char) - sizeof(bool));
		
		while (true) {
			if (pItem->level == MAX_LEVEL) break;
			CItem* pBuddy = getBuddy(pItem);

			if (pBuddy->level == pItem->level && pBuddy->free) {
				removeItem(pBuddy);
				if (pItem > pBuddy) pItem = pBuddy;
				pItem->level++;
			}
			else break;
		}

		insertItem(pItem);
	}

	void insertItem(CItem* item) {
		lists[item->level].insertItem(item);
		item->free = true;
		mask |= static_cast<unsigned long long>(1) << item->level;
	}

	void removeItem(CItem* item) {
		lists[item->level].removeItem(item);
		item->free = false;
		mask &= ~(static_cast<unsigned long long>(1) << item->level);
	}
};