#pragma once
#include "pch.h"
#include <chrono>
#include <ctime>

inline bool				dll_running		= false;
inline long				OldWndProc		= 0;
inline auto				start			= std::chrono::system_clock::now();
inline std::string		timer			= "00:00:00";

DWORD WINAPI ThreadProc(LPVOID lpModule);
LRESULT CALLBACK SafeWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
