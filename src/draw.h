#pragma once
#include "pch.h"

namespace Draw
{
	static bool		imgui_show = TRUE;
	static bool		imgui_init = FALSE;
}

void DrawUI(IDirect3DDevice9* device);