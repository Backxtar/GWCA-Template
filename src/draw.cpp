#include "draw.h"

void DrawUI(IDirect3DDevice9* device)
{
	HWND hWnd = GW::MemoryMgr::GetGWWindowHandle();

	if (!Draw::imgui_init)
	{
		Hooks::OldWndProc = SetWindowLongPtr(hWnd, GWL_WNDPROC, reinterpret_cast<long>(SafeWndProc));

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplDX9_Init(device);
		ImGui_ImplWin32_Init(hWnd);

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = false;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		Draw::imgui_init = true;
	}

	ImGui::GetIO().AddKeyEvent(ImGuiKey_ModCtrl, (GetKeyState(VK_CONTROL) & 0x8000) != 0);
	ImGui::GetIO().AddKeyEvent(ImGuiKey_ModShift, (GetKeyState(VK_SHIFT) & 0x8000) != 0);
	ImGui::GetIO().AddKeyEvent(ImGuiKey_ModAlt, (GetKeyState(VK_MENU) & 0x8000) != 0);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Draw UI elements
	if (Draw::imgui_show)
	{
		ImGui::Begin("Hello, World!");
		ImGui::Text("This is a sample text.");
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();

	ImDrawData* draw_data = ImGui::GetDrawData();
	ImGui_ImplDX9_RenderDrawData(draw_data);

	if (!Hooks::dll_running && Draw::imgui_init)
	{
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		SetWindowLongPtr(hWnd, GWL_WNDPROC, Hooks::OldWndProc);
		Draw::imgui_init = false;
	}
}