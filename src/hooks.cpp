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

    Hooks::dll_running = TRUE;
    while (Hooks::dll_running)
    {
        if (GetAsyncKeyState(VK_END) & 1) 
            Hooks::dll_running = FALSE;
        if (GetAsyncKeyState(VK_INSERT) & 1)
            Draw::imgui_show = !Draw::imgui_show;

        Sleep(100);
    }
    
    // Hooks are disable from Guild Wars thread (safely), so we just make sure we exit the last hooks
    GW::DisableHooks();
    while (GW::HookBase::GetInHookCount())
        Sleep(16);

    Sleep(16);
    GW::GameThread::Enqueue([]()
        {
            GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Template++: Bye!");
        });
    GW::Terminate();

    FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
}

LRESULT CALLBACK SafeWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    __try {
        return WndProc(hWnd, Message, wParam, lParam);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return CallWindowProc(reinterpret_cast<WNDPROC>(Hooks::OldWndProc), hWnd, Message, wParam, lParam);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    bool right_mouse_down = FALSE;

    if (Message == WM_RBUTTONUP)
        right_mouse_down = FALSE;
    
    if (Message == WM_RBUTTONDOWN)
        right_mouse_down = TRUE;
    
    if (Message == WM_RBUTTONDBLCLK)
        right_mouse_down = TRUE;
    
    ImGuiIO& io = ImGui::GetIO();
    const bool skip_mouse_capture = right_mouse_down || GW::UI::GetIsWorldMapShowing() || GW::Map::GetIsInCinematic();
    
    if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam) && !skip_mouse_capture)
        return TRUE;

    switch (Message) {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK: {
        if (!right_mouse_down) io.MouseDown[0] = true;
        break;
    }
    case WM_LBUTTONUP: {
        io.MouseDown[0] = false;
        break;
    }
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
        if (!right_mouse_down) {
            io.AddKeyEvent(VK_MBUTTON, TRUE);
            io.MouseDown[2] = true;
        }
        break;
    case WM_MBUTTONUP: {
        io.AddKeyEvent(VK_MBUTTON, FALSE);
        io.MouseDown[2] = false;
        break;
    }
    case WM_MOUSEWHEEL: {
        if (!right_mouse_down) io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
        break;
    }
    case WM_MOUSEMOVE: {
        if (!right_mouse_down) {
            io.MousePos.x = (float)GET_X_LPARAM(lParam);
            io.MousePos.y = (float)GET_Y_LPARAM(lParam);
        }
        break;
    }
    case WM_XBUTTONDOWN: {
        if (!right_mouse_down) {
            if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
                io.AddKeyEvent(VK_XBUTTON1, TRUE);
            if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
                io.AddKeyEvent(VK_XBUTTON2, TRUE);
        }
        break;
    }
    case WM_XBUTTONUP: {
        if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
            io.AddKeyEvent(VK_XBUTTON1, FALSE);
        if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
            io.AddKeyEvent(VK_XBUTTON2, FALSE);
        break;
    }
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN: {
        if (wParam < 256)
            io.AddKeyEvent(wParam, TRUE);
        break;
    }
    case WM_SYSKEYUP:
    case WM_KEYUP: {
        if (wParam < 256)
            io.AddKeyEvent(wParam, FALSE);
        break;
    }
    case WM_CHAR: {// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
        if (wParam > 0 && wParam < 0x10000)
            io.AddInputCharacter((unsigned short)wParam);
        break;
    }
    default:
        break;
    }

    //
    // This second switch is used to determine whether we need to forward the input to Guild Wars.
    //

    switch (Message) {
        // Send button up mouse events to everything, to avoid being stuck on mouse-down
    case WM_LBUTTONUP:
        break;

        // Other mouse events:
        // - If right mouse down, leave it to gw
        // - ImGui first (above), if WantCaptureMouse that's it
        // - Toolbox module second (e.g.: minimap), if captured, that's it
        // - otherwise pass to gw
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL: {
        if (!right_mouse_down && io.WantCaptureMouse)
            return true;
        break;
    }

        // keyboard messages
    case WM_KEYUP:
    case WM_SYSKEYUP: {
        if (io.WantTextInput) break; // if imgui wants them, send to imgui (above) and to gw
        [[fallthrough]];
    }
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
    case WM_MBUTTONUP:
        if (io.WantTextInput) return true; // if imgui wants them, send just to imgui (above)

        // note: capturing those events would prevent typing if you have a hotkey assigned to normal letters. 
        // We may want to not send events to toolbox if the player is typing in-game
        // Otherwise, we may want to capture events. 
        // For that, we may want to only capture *successfull* hotkey activations.
        break;

    case WM_SIZE:
        // ImGui doesn't need this, it reads the viewport size directly
        break;

    default:
        break;
    }

    return CallWindowProc((WNDPROC)Hooks::OldWndProc, hWnd, Message, wParam, lParam);
}