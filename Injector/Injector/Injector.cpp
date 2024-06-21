#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>

DWORD GetTargetProcessId(const wchar_t* processName)
{
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(snapshot, &processEntry) == TRUE)
    {
        while (Process32Next(snapshot, &processEntry) == TRUE)
        {
            if (_tcsicmp(processEntry.szExeFile, processName) == 0)
            {
                CloseHandle(snapshot);
                return processEntry.th32ProcessID;
            }
        }
    }

    CloseHandle(snapshot);
    return 0;
}

int wmain(int argc, wchar_t* argv[])
{
    if (argc != 3)
    {
        std::wcout << L"Usage: Injector.exe <DLL Path> <Target Process Name>" << std::endl;
        return 1;
    }

    const wchar_t* dllPath = argv[1];
    const wchar_t* targetProcessName = argv[2];

    // Print the full path of the DLL
    std::wcout << L"DLL Path: " << dllPath << std::endl;

    DWORD processId = GetTargetProcessId(targetProcessName);
    if (processId == 0)
    {
        wprintf(L"Could not find process %s\n", targetProcessName);
        return 1;
    }

    // Print the PID of the target process
    std::wcout << L"Target Process ID: " << processId << std::endl;

    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (processHandle == NULL)
    {
        wprintf(L"Could not open process %s\n", targetProcessName);
        return 1;
    }

    LPVOID allocatedMemory = VirtualAllocEx(processHandle, NULL, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (allocatedMemory == NULL)
    {
        wprintf(L"Could not allocate memory in process %s\n", targetProcessName);
        CloseHandle(processHandle);
        return 1;
    }

    if (!WriteProcessMemory(processHandle, allocatedMemory, dllPath, (wcslen(dllPath) + 1) * sizeof(wchar_t), NULL))
    {
        wprintf(L"Could not write to process memory\n");
        VirtualFreeEx(processHandle, allocatedMemory, 0, MEM_RELEASE);
        CloseHandle(processHandle);
        return 1;
    }

    HANDLE remoteThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, allocatedMemory, 0, NULL);
    if (remoteThread == NULL)
    {
        wprintf(L"Could not create remote thread in process %s\n", targetProcessName);
        VirtualFreeEx(processHandle, allocatedMemory, 0, MEM_RELEASE);
        CloseHandle(processHandle);
        return 1;
    }

    WaitForSingleObject(remoteThread, INFINITE);

    VirtualFreeEx(processHandle, allocatedMemory, 0, MEM_RELEASE);
    CloseHandle(remoteThread);
    CloseHandle(processHandle);

    wprintf(L"DLL successfully injected into process %s\n", targetProcessName);
    return 0;
}
