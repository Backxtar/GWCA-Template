#pragma once
#include "pch.h"

namespace Draw
{
	struct Globals 
	{
		bool	imgui_show = true;
		bool	imgui_init = false;
	};

	inline Globals dVar;
}

void DrawUI(IDirect3DDevice9* device);