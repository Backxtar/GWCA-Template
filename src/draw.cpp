#include "draw.h"

// START declare
void HandleKeyState();
void InitImGui(HWND& hWnd, IDirect3DDevice9* device);
void CloseImGui(HWND& hWnd);
// END declare

void DrawUI(IDirect3DDevice9* device)
{
	HWND hWnd = GW::MemoryMgr::GetGWWindowHandle();

	HandleKeyState();
	InitImGui(hWnd, device);

	ImGui::GetIO().AddKeyEvent(ImGuiKey_ModCtrl, (GetKeyState(VK_CONTROL) & 0x8000) != 0);
	ImGui::GetIO().AddKeyEvent(ImGuiKey_ModShift, (GetKeyState(VK_SHIFT) & 0x8000) != 0);
	ImGui::GetIO().AddKeyEvent(ImGuiKey_ModAlt, (GetKeyState(VK_MENU) & 0x8000) != 0);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Draw UI elements
	if (imgui_show)
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

	CloseImGui(hWnd);
}

void HandleKeyState()
{
	if (GetAsyncKeyState(VK_END) & 1)
	{
		dll_running = false;
		GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Template++: Terminating!");
	}
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		imgui_show = !imgui_show;
		GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, (imgui_show ? L"Template++: UI Enabled" : L"Template++: UI Disabled"));
	}
}

void InitImGui(HWND& hWnd, IDirect3DDevice9* device)
{
	if (!imgui_init)
	{
		OldWndProc = SetWindowLongPtr(hWnd, GWL_WNDPROC, reinterpret_cast<long>(SafeWndProc));

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

		imgui_init = true;
	}
}

void CloseImGui(HWND& hWnd)
{
	if (!dll_running && imgui_init)
	{
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		SetWindowLongPtr(hWnd, GWL_WNDPROC, OldWndProc);
		
		imgui_init = false;
	}
}