// TextMap.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "CStringComparator.h"
#include "CMyAllocator.h"

using namespace std;


char* ReadTextFromFile(char* filename) {
	ifstream file;
	file.open(filename, ios::in);

	size_t begin = file.tellg();
	file.seekg(0, ios::end);
	size_t end = file.tellg();
	size_t size = end - begin;
	file.seekg(0);

	char* text = new char[size+1];
	file.read(text, size);
	text[size] = '\0';
	file.close();

	return text;
}

bool IsNotSeparator(char item) {
	return item != ' ' && item != '\n\t' && item != '\n';
}

template <class T> // T is map
char* CreateTextMapFromFileAndGetTextPtr(T& testMap, char* bookName) {
	//Please, don't forget to delete the text from the returned ptr

	char* text = ReadTextFromFile(bookName);
	char* word;
	size_t iter = -1;
	size_t begin = 0;
	bool needBreak = false;

	while (iter++ || true) {
		if (IsNotSeparator(text[iter]) && text[iter] != '\0') continue;
		if (text[iter] == '\0') needBreak = true;
		text[iter] = '\0';
		word = text + begin;
		begin = iter + 1;

		auto It = testMap.find(word);
		if (It == testMap.end())
			testMap.insert(make_pair(word, 1));
		else
			It->second++;
		if (needBreak) break;
	}

	return text;
}


ULONGLONG GetCurrentTimeMs()
{
	SYSTEMTIME S = { 0 };
	FILETIME F = { 0 };
	GetSystemTime(&S);
	SystemTimeToFileTime(&S, &F);
	LARGE_INTEGER Int;
	Int.HighPart = F.dwHighDateTime;
	Int.LowPart = F.dwLowDateTime;
	return Int.QuadPart / 10000;
}


template <class T> // T is map
vector<pair<char*, size_t>> SortTextMapDescendingCount(T& textMap) {
	vector<pair<char*, size_t>> pairs;
	pairs.reserve(textMap.size());
	for (auto entry : textMap)
		pairs.push_back(entry);
	sort(pairs.begin(), pairs.end(), [](const std::pair<char*, size_t>& left, const std::pair<char*, size_t>& right) {
		return left.second > right.second;
		});
	return pairs;
}


void TextMapTest()
{
	map<char*, size_t, CStringComparator, CMyAllocator<char*>> myMap;
	map<char*, size_t> defaultMap;
	//char* book = (char*)"test.txt";
	char* book = (char*)"book.txt";


// ----------------- Calculate time of map with allocator -----------------
	ULONGLONG Start = GetCurrentTimeMs();
	char* text = CreateTextMapFromFileAndGetTextPtr(myMap, book);
	ULONGLONG End = GetCurrentTimeMs();
	printf("Time with custom allocator (ms) %d\n", (int)(End - Start));
// ----------------- Calculate time of map with allocator -----------------

	

// -------------------- Calculate time of default map ----------------------
	Start = GetCurrentTimeMs();
	text = CreateTextMapFromFileAndGetTextPtr(defaultMap, book);
	End = GetCurrentTimeMs();
	printf("Time with default allocator (ms) %d\n", (int)(End - Start));
// -------------------- Calculate time of default map ----------------------


// ------------------ Print all words and their count ----------------------
	/*for (auto Entry : SortTextMapDescendingCount(myMap))
	{
		printf("Word \"%s\", count %I64d\n", Entry.first, (uint64_t)Entry.second);
	}*/
// ------------------ Print all words and their count ----------------------

	delete text;
	text = nullptr;
}


int main()
{
	TextMapTest();
	printf("Finished");
	getchar();

	//C++, where is your LinkedHashMap?
	return 0;
}
