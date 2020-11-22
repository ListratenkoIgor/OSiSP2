#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <Windows.h>

#define OUTPUT_STRING "Hello, World!"

int main()
{
    const char* outputString = OUTPUT_STRING;
    volatile char* copyStr = (char*)calloc(strlen(outputString) + 1, sizeof(char));
    strcpy_s((char*)copyStr, strlen(outputString) + 1, outputString);
    while (true)
    {
        puts((char*)copyStr);
        Sleep(100);
    }
    _getch();

}
