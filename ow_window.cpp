#include "ow_window.h"
#include <iostream>

OW_Window::OW_Window() {}
OW_Window::~OW_Window() {}

bool OW_Window::create(int width, int height, const std::string& title)
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    const wchar_t CLASS_NAME[] = L"OWWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = OW_Window::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc))
    {
        std::cout << "Failed to register window class!\n";
        return false;
    }

    hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        std::wstring(title.begin(), title.end()).c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd)
    {
        std::cout << "Failed to create window!\n";
        return false;
    }

    ShowWindow(hwnd, SW_SHOW);
    running = true;
    return true;
}

void OW_Window::pollEvents()
{
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            running = false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void OW_Window::beginFrame()
{
    // clear screen here later (GDI/OpenGL/DirectX)
}

void OW_Window::endFrame()
{
    // present frame here later
}

void OW_Window::destroy()
{
    if (hwnd)
    {
        ::DestroyWindow(hwnd);
        hwnd = nullptr;
    }
}

LRESULT CALLBACK OW_Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
