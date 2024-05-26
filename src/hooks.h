#pragma once
#include "pch.h"

namespace Hooks
{
	static bool     dll_running = false;
	static long		OldWndProc;
}

DWORD WINAPI ThreadProc(LPVOID lpModule);
LRESULT CALLBACK SafeWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
