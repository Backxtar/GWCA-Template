#include "hooks.h"
#include <thread>

// START declare
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
const std::string GetTimeElapsed();
// END declare

DWORD WINAPI ThreadProc(LPVOID lpModule)
{
    HMODULE hModule = static_cast<HMODULE>(lpModule);
    GW::Initialize();
    GW::GameThread::Enqueue([]()
        {
            GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Template++: Initialized");
        });

    dll_running = true;

    GW::Render::SetRenderCallback(DrawUI);
    GW::Render::SetResetCallback([](IDirect3DDevice9* /*device*/) { ImGui_ImplDX9_InvalidateDeviceObjects(); });

    std::thread clock = std::thread([]() 
        { 
            while (dll_running)
            {
                timer = GetTimeElapsed();
                Sleep(100);
            }
        });

    while (dll_running)
    {
        // Do other stuff
        Sleep(2000);
    }
    
    GW::GameThread::Enqueue([]()
        {
            GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Template++: Bye!");
        });
    
    // Hooks are disable from Guild Wars thread (safely), so we just make sure we exit the last hooks
    GW::DisableHooks();
    while (GW::HookBase::GetInHookCount()) Sleep(16);
    
    GW::Terminate();
    FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
}

LRESULT CALLBACK SafeWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    __try {
        return WndProc(hWnd, Message, wParam, lParam);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return CallWindowProc(reinterpret_cast<WNDPROC>(OldWndProc), hWnd, Message, wParam, lParam);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    static bool right_mouse_down = false;

    if (Message == WM_CLOSE || (Message == WM_SYSCOMMAND && wParam == SC_CLOSE)) 
        return 0;

    if (!imgui_show) 
        return CallWindowProc((WNDPROC)OldWndProc, hWnd, Message, wParam, lParam);

    if (Message == WM_RBUTTONUP)
        right_mouse_down = false;
    
    if (Message == WM_RBUTTONDOWN)
        right_mouse_down = true;
    
    if (Message == WM_RBUTTONDBLCLK)
        right_mouse_down = true;
    
    const ImGuiIO& io = ImGui::GetIO();
    const bool skip_mouse_capture = right_mouse_down || GW::UI::GetIsWorldMapShowing() || GW::Map::GetIsInCinematic();

    if (skip_mouse_capture) return CallWindowProc((WNDPROC)OldWndProc, hWnd, Message, wParam, lParam);
    if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam) && !skip_mouse_capture) return TRUE;

    switch (Message) {

    // Send button up mouse events to everything, to avoid being stuck on mouse-down
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_INPUT:
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL: {
        if (io.WantCaptureMouse && !skip_mouse_capture)
            return true;
        break;
    }

    // if imgui wants them, send to imgui (above) and to gw
    case WM_KEYUP:
    case WM_SYSKEYUP: {
        if (io.WantTextInput)
            break;
    }

    // if imgui wants them, send just to imgui (above)
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
            return true;
    }

    // We may want to not send events to imgui if the player is typing in - game
    case WM_ACTIVATE:
        break;
    case WM_SIZE: // ImGui doesn't need this, it reads the viewport size directly
        break;

    default:
        break;
    }

    return CallWindowProc((WNDPROC)OldWndProc, hWnd, Message, wParam, lParam);
}

const std::string GetTimeElapsed()
{
    auto current = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current - start);

    auto secs = std::chrono::duration_cast<std::chrono::seconds>(elapsed);
    elapsed -= std::chrono::duration_cast<std::chrono::milliseconds>(secs);
    auto mins = std::chrono::duration_cast<std::chrono::minutes>(secs);
    secs -= std::chrono::duration_cast<std::chrono::seconds>(mins);
    auto hour = std::chrono::duration_cast<std::chrono::hours>(mins);
    mins -= std::chrono::duration_cast<std::chrono::minutes>(hour);

    std::stringstream ss;
    ss << (hour.count() < 10 ? "0" + std::to_string(hour.count()) : std::to_string(hour.count())) << ":" << (mins.count() < 10 ? "0" + std::to_string(mins.count()) : std::to_string(mins.count())) << ":" << (secs.count() < 10 ? "0" + std::to_string(secs.count()) : std::to_string(secs.count()));
    return ss.str();
}