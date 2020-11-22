#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include "..\StringReplaceDLL\StringReplaceDLL.h"

#define STRING_TO_FIND "Hello, World"
#define STRING_TO_REPLACE "Hello, Rouge"

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
