#include "Window.h"
#include "Utils.h"

INT window::init(const HINSTANCE h_instance, const UINT width, const UINT height)
{
    WNDCLASS wc = {};
    wc.hInstance = h_instance;
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.lpszClassName = TEXT("Direct3D 11");
    wc.lpfnWndProc = wnd_proc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    if (RegisterClass(&wc) == 0)
        return 10;

    const UINT screen_width = GetSystemMetrics(SM_CXSCREEN);
    const UINT screen_height = GetSystemMetrics(SM_CYSCREEN);

    const RECT wr = {
        static_cast<LONG>(screen_width - width) / 2, static_cast<LONG>(screen_height - height) / 2,
        static_cast<LONG>(screen_width + width) / 2, static_cast<LONG>(screen_height + height) / 2
    };
    constexpr DWORD style = WS_OVERLAPPEDWINDOW;

    h_wnd_ = CreateWindow(wc.lpszClassName, wc.lpszClassName, style, wr.left, wr.top,
                          wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, h_instance, nullptr);

    CHECK_NULL(h_wnd_, 12);

    ShowWindow(h_wnd_, SW_SHOW);
    SetFocus(h_wnd_);

    return 0;
}

BOOL window::run()
{
    static MSG msg = {};

    if (PeekMessage(&msg, nullptr, 0, UINT_MAX, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.message != WM_QUIT;
}

void window::de_init()
{
}

LRESULT CALLBACK wnd_proc(const HWND h_wnd, const UINT msg, const WPARAM w_param, const LPARAM l_param)
{
    switch (msg)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        if (w_param == VK_ESCAPE)
            DestroyWindow(h_wnd);
        return 0;

    default:
        return DefWindowProc(h_wnd, msg, w_param, l_param);
    }
}
