#pragma once
#include "pch.h"
#include <thread>
#include <string>
#include <sstream>


inline bool				dll_running		= false;
inline long				OldWndProc		= 0;

inline bool				bot_running		= false;
inline long long		timerTick		= 0L;
inline std::string		timerValue		= "00:00:00";

DWORD WINAPI ThreadProc(LPVOID lpModule);
LRESULT CALLBACK SafeWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
