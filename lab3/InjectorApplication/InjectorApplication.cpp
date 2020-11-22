#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <Windows.h>
#include <tlhelp32.h>

#define KERNEL32_PROCESS_NAME L"Kernel32"
#define LOADLIBRARY_FUNCTION_NAME "LoadLibraryA"

#define INJECTED_PROCESS_NAME L"InjectedApplication.exe"
#define DLL_FILENAME "InjectedDll.dll"
#define PROCESS_NOT_FOUND -1

#define PROCESS_NOT_FOUND_MSG "Process with name %ws not found.\n"

int GetProcessIdByName(const wchar_t* processName);
int InjectDllToProcess(int pid, const char* dllFileName);

int main()
{
    int pid = GetProcessIdByName(INJECTED_PROCESS_NAME);
    if (pid == PROCESS_NOT_FOUND)
    {
        printf(PROCESS_NOT_FOUND_MSG, INJECTED_PROCESS_NAME);
        return 0;
    }
    int lastError = InjectDllToProcess(pid, DLL_FILENAME);
    printf("Injecting %s library in %ls proccess with pid = %d. return code: %d\n", DLL_FILENAME, INJECTED_PROCESS_NAME, pid , lastError);
    _getch();
    return 0;
}

int GetProcessIdByName(const wchar_t* processName)
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    int processId = PROCESS_NOT_FOUND;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(snapshot, &entry))
    {
        while (Process32Next(snapshot, &entry))
        {
            if (wcscmp(entry.szExeFile, processName) == 0)
            {
                processId = entry.th32ProcessID;
                break;
            }
        }
    }

    CloseHandle(snapshot);
    return processId;
}

int InjectDllToProcess(int pid, const char* dllFileName)
{
    int lastError;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    lastError = GetLastError();
    if (hProcess == NULL) {
        return lastError;
    }

    LPVOID dllNamePointer = VirtualAllocEx(hProcess, NULL, strlen(dllFileName) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    lastError = GetLastError();
    if (dllNamePointer != NULL) {
        BOOL result = WriteProcessMemory(hProcess, dllNamePointer, dllFileName, strlen(dllFileName) + 1, NULL);

        lastError = GetLastError();
        if (result != 0) {

            LPVOID LoadLib_addr = GetProcAddress(GetModuleHandle(KERNEL32_PROCESS_NAME), LOADLIBRARY_FUNCTION_NAME);

            lastError = GetLastError();
            if (LoadLib_addr != NULL) {
                HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLib_addr, dllNamePointer, 0, NULL);

                lastError = GetLastError();
                if (hRemoteThread != NULL) {

                    WaitForSingleObject(hRemoteThread, INFINITE);
                    CloseHandle(hRemoteThread);

                }                
            }
            else {
                return lastError;
            }
        }
        VirtualFreeEx(hProcess, dllNamePointer, 0, MEM_RELEASE);
    }
    else {
        return lastError;
    }

    CloseHandle(hProcess);
    return lastError;
}