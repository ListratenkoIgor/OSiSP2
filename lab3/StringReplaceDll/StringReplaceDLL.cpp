#include "StringReplaceDLL.h"
#include "pch.h"
extern "C" {
	char* SearchForSubArray(const char* arrStart, const char* arrEnd, const char* subArr, int subArrLen)
	{
		for (int i = 0; i < arrEnd - arrStart - subArrLen; i++)
			if (!memcmp(arrStart + i, subArr, subArrLen))
				return (char*)arrStart + i;
		return NULL;
	}


	void __declspec(dllexport) __cdecl ReplaceString(const char* stringToFind, const char* stringToReplace)
	{

		char* address = 0x00;
		MEMORY_BASIC_INFORMATION basicInformation;
		int strLen = strlen(stringToFind);
		while (VirtualQuery(address, &basicInformation, sizeof(basicInformation)))
		{
			if (address == 0)
				address = (char*)basicInformation.BaseAddress;
			if (basicInformation.State == MEM_COMMIT && !(basicInformation.Protect & PAGE_GUARD) && (basicInformation.Protect & PAGE_READWRITE))
			{
				char* endPtr = address + basicInformation.RegionSize;
				char* currPointer = SearchForSubArray(address, endPtr, stringToFind, strLen);
				while (currPointer != NULL)
				{
					if (currPointer != stringToFind)
					{
						CopyMemory((LPVOID)currPointer, stringToReplace, strlen(stringToReplace) + 1);
					}
					currPointer = SearchForSubArray(currPointer + strLen, endPtr, stringToFind, strLen);
				}
			}
			address += basicInformation.RegionSize;
		}
	}
}