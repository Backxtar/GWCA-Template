#pragma once
#include "pch.h"

namespace Hooks
{
	struct Globals 
	{
		bool	dll_running = false;
		long	OldWndProc;
	};

	inline Globals hVar;
}

DWORD WINAPI ThreadProc(LPVOID lpModule);
LRESULT CALLBACK SafeWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
