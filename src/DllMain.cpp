#include "pch.h"

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);
    DisableThreadLibraryCalls(hModule);

    if (dwReason == DLL_PROCESS_ATTACH) 
    {
        HANDLE handle = CreateThread(0, 0, ThreadProc, hModule, 0, 0);
        if (handle) CloseHandle(handle);
    }

    return TRUE;
}
