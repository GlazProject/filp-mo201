// TextMap.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <iostream>
#include <fstream>
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
	return item != ' ';
}

template <class T> // T is map
char* CreateTextMapFromFileAndGetTextPtr(T& testMap, char* bookName) {
	//Please, don't forget to delete the text from the returned ptr

	char* text = ReadTextFromFile((char*)"test.txt");
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


void TextMapTest()
{
	map<char*, size_t, CStringComparator, CMyAllocator<char*>> myMap;
	map<char*, size_t> defaultMap;

	char* book = (char*)"test.txt";

	ULONGLONG Start = GetCurrentTimeMs();
	char* text_1 = CreateTextMapFromFileAndGetTextPtr(defaultMap, book);
	delete text_1;
	text_1 = nullptr;
	ULONGLONG End = GetCurrentTimeMs();
	printf("Time with cstom allocator (ms) %d\n", (int)(End - Start));
	
	ULONGLONG Start = GetCurrentTimeMs();
	char* text_2 = CreateTextMapFromFileAndGetTextPtr(myMap, book);
	delete text_2;
	text_2 = nullptr;
	ULONGLONG End = GetCurrentTimeMs();
	printf("Time with default allocator (ms) %d\n", (int)(End - Start));

	for (auto Entry : myMap)
	{
		printf("Word \"%s\", count %I64d\n", Entry.first, (uint64_t)Entry.second);
	}
}


int main()
{
	ULONGLONG Start = GetCurrentTimeMs();

	TextMapTest();

	ULONGLONG End = GetCurrentTimeMs();

	printf("Total time (ms) %d\n", (int)(End - Start));

	getchar();

	//C++, where is your LinkedHashMap?
	return 0;
}
