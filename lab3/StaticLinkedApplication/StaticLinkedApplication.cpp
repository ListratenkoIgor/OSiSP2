#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>

#define STRING_TO_FIND "Hello, World"
#define STRING_TO_REPLACE "Hello, Rouge"

extern "C" {
	void __declspec(dllimport) __cdecl ReplaceString(const char* stringToFind, const char* stringToReplace);

	int main()
	{
		const char* requiredString = STRING_TO_FIND;
		const char* replaceString = STRING_TO_REPLACE;
		char* copyStr = (char*)calloc(strlen(requiredString) + 1, sizeof(char));
		strcpy_s((char*)copyStr, strlen(requiredString) + 1, requiredString);
		puts(copyStr);
		ReplaceString(requiredString, replaceString);
		puts(copyStr);
		_getch();
		return 0;
	}
}