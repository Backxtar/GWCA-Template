// declare method here as recommended by imgui
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

DWORD WINAPI ThreadProc(LPVOID lpModule)
{
    HMODULE hModule = static_cast<HMODULE>(lpModule);
    GW::Initialize();
    GW::GameThread::Enqueue([]()
        {
            GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Template++: Initialized");
        });

    GW::Render::SetRenderCallback(DrawUI);
    GW::Render::SetResetCallback([](IDirect3DDevice9* /*device*/) { ImGui_ImplDX9_InvalidateDeviceObjects(); });

    Hooks::hVar.dll_running = true;
    while (Hooks::hVar.dll_running)
    {
        if (GetAsyncKeyState(VK_END) & 1) 
            Hooks::hVar.dll_running = false;
        if (GetAsyncKeyState(VK_INSERT) & 1)
            Draw::dVar.imgui_show = !Draw::dVar.imgui_show;

        Sleep(100);
    }
    
    GW::GameThread::Enqueue([]()
        {
            GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Template++: Bye!");
        });
    // Hooks are disable from Guild Wars thread (safely), so we just make sure we exit the last hooks
    GW::DisableHooks();
    while (GW::HookBase::GetInHookCount()) 
        Sleep(16);

    GW::Terminate();

    FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
}

LRESULT CALLBACK SafeWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    __try {
        return WndProc(hWnd, Message, wParam, lParam);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return CallWindowProc(reinterpret_cast<WNDPROC>(Hooks::hVar.OldWndProc), hWnd, Message, wParam, lParam);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    static bool right_mouse_down = false;

    if (Message == WM_CLOSE || (Message == WM_SYSCOMMAND && wParam == SC_CLOSE)) 
        return 0;

    if (!Draw::dVar.imgui_show) 
        return CallWindowProc((WNDPROC)Hooks::hVar.OldWndProc, hWnd, Message, wParam, lParam);

    if (Message == WM_RBUTTONUP)
        right_mouse_down = false;
    
    if (Message == WM_RBUTTONDOWN)
        right_mouse_down = true;
    
    if (Message == WM_RBUTTONDBLCLK)
        right_mouse_down = true;
    
    const ImGuiIO& io = ImGui::GetIO();
    const bool skip_mouse_capture = right_mouse_down || GW::UI::GetIsWorldMapShowing() || GW::Map::GetIsInCinematic();

    if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam) && !skip_mouse_capture) return TRUE;

    switch (Message) {
        // Send button up mouse events to everything, to avoid being stuck on mouse-down
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_INPUT:
        break;

        // Other mouse events:
        // - If right mouse down, leave it to gw
        // - ImGui first (above), if WantCaptureMouse that's it
        // - Toolbox module second (e.g.: minimap), if captured, that's it
        // - otherwise pass to gw
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL: {
        if (io.WantCaptureMouse && !skip_mouse_capture)
            return true;
    }
        break;

    // keyboard messages
    case WM_KEYUP:
    case WM_SYSKEYUP: {
        if (io.WantTextInput)
            break; // if imgui wants them, send to imgui (above) and to gw
    } // else fallthrough

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_CHAR:
    case WM_SYSCHAR:
    case WM_IME_CHAR:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONDBLCLK:
    case WM_XBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
    case WM_MBUTTONUP: {
        if (io.WantTextInput)
            return true; // if imgui wants them, send just to imgui (above)
    }

    case WM_ACTIVATE:
    // note: capturing those events would prevent typing if you have a hotkey assigned to normal letters.
    // We may want to not send events to toolbox if the player is typing in-game
    // Otherwise, we may want to capture events.
    // For that, we may want to only capture *successfull* hotkey activations.
        break;

    case WM_SIZE: // ImGui doesn't need this, it reads the viewport size directly
        break;
    default:
        break;
    }

    return CallWindowProc((WNDPROC)Hooks::hVar.OldWndProc, hWnd, Message, wParam, lParam);
}