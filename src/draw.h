#pragma once
#include "pch.h"

namespace Draw
{
	static bool		imgui_show = true;
	static bool		imgui_init = false;
}

void DrawUI(IDirect3DDevice9* device);