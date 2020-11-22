#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>

typedef void(__cdecl* ReplaceStringProc)(const char*, const char*);

#define DLL_FILENAME TEXT("StringReplaceDll.dll")
#define FIND_STRING_PROC_NAME "ReplaceString"

#define STRING_TO_FIND "Hello, World"
#define STRING_TO_REPLACE "Hello, Rouge"

int main()
{
    ReplaceStringProc ReplaceString = NULL;
    const char* requiredString = STRING_TO_FIND;
    const char* replaceString = STRING_TO_REPLACE;
    char* copyStr = (char*)calloc(strlen(requiredString) + 1, sizeof(char));
    strcpy_s((char*)copyStr, strlen(requiredString) + 1, requiredString);
    puts(copyStr);

    HINSTANCE hinstLib = LoadLibrary(DLL_FILENAME);

    if (hinstLib != NULL)
    {
        ReplaceString = (ReplaceStringProc)GetProcAddress(hinstLib, "ReplaceString");

        if (NULL != ReplaceString)
        {
            ReplaceString(requiredString, replaceString);
        }

        FreeLibrary(hinstLib);
    }

    puts(copyStr);
    _getch();
    return 0;
}