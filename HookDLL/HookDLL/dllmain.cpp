// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include "detours.h"

// Define the type for the original MessageBoxW function
typedef int (WINAPI* MessageBoxW_t)(HWND, LPCWSTR, LPCWSTR, UINT);

// Pointer to the original MessageBoxW function
MessageBoxW_t RealMessageBoxW = MessageBoxW;

// Hooked version of MessageBoxW
int WINAPI HookedMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
    lpText = L"Hooked!";
    return RealMessageBoxW(hWnd, lpText, lpCaption, uType);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)RealMessageBoxW, HookedMessageBoxW);
        DetourTransactionCommit();
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)RealMessageBoxW, HookedMessageBoxW);
        DetourTransactionCommit();
    }
    return TRUE;
}


