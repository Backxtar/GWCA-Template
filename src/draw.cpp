#include "draw.h"

void DrawUI(IDirect3DDevice9* device)
{
	HWND hWnd = GW::MemoryMgr::GetGWWindowHandle();

	if (!Draw::dVar.imgui_init)
	{
		Hooks::hVar.OldWndProc = SetWindowLongPtr(hWnd, GWL_WNDPROC, reinterpret_cast<long>(SafeWndProc));

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplDX9_Init(device);
		ImGui_ImplWin32_Init(hWnd);

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = false;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		ImGuiStyle& style = ImGui::GetStyle();
		style.TabRounding = 8.f;
		style.FrameRounding = 8.f;
		style.GrabRounding = 8.f;
		style.WindowRounding = 8.f;
		style.PopupRounding = 8.f;
		style.FrameBorderSize = 1.00;
		style.WindowBorderSize = 1.00;

		Draw::dVar.imgui_init = true;
	}

	ImGui::GetIO().AddKeyEvent(ImGuiKey_ModCtrl, (GetKeyState(VK_CONTROL) & 0x8000) != 0);
	ImGui::GetIO().AddKeyEvent(ImGuiKey_ModShift, (GetKeyState(VK_SHIFT) & 0x8000) != 0);
	ImGui::GetIO().AddKeyEvent(ImGuiKey_ModAlt, (GetKeyState(VK_MENU) & 0x8000) != 0);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Draw UI elements
	if (Draw::dVar.imgui_show)
	{
		//ImGui::ShowDemoWindow();
		ImGui::SetNextWindowBgAlpha(0.7f);
		ImGui::Begin("Hello Guild Wars!", 0, ImGuiWindowFlags_NoResize);
		ImGui::Text("This is a mod made by Backxtar!");
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();

	ImDrawData* draw_data = ImGui::GetDrawData();
	ImGui_ImplDX9_RenderDrawData(draw_data);

	if (!Hooks::hVar.dll_running && Draw::dVar.imgui_init)
	{
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		SetWindowLongPtr(hWnd, GWL_WNDPROC, Hooks::hVar.OldWndProc);
		Draw::dVar.imgui_init = false;
	}
}