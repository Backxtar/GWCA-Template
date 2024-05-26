#pragma once
#include "pch.h"

inline bool		dll_running		= false;
inline long		OldWndProc		= 0;

DWORD WINAPI ThreadProc(LPVOID lpModule);
LRESULT CALLBACK SafeWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
