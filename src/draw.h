#pragma once
#include "pch.h"

inline bool imgui_show	= true;
inline bool	imgui_init	= false;

void DrawUI(IDirect3DDevice9* device);
void HandleKeyState();
void InitImGui(HWND& hWnd, IDirect3DDevice9* device);
void CloseImGui(HWND& hWnd);