#include "ow_window.h"
#include <iostream>

OW_Window::OW_Window() {}
OW_Window::~OW_Window() {}

bool OW_Window::create(int width, int height, const std::string& title, bool fullScreenMode)
{
    fullscreen = fullScreenMode;
    windowWidth = width;
    windowHeight = height;

    DWORD style = WS_OVERLAPPEDWINDOW;
    if (fullscreen)
    {
        style = WS_POPUP;
        windowWidth  = GetSystemMetrics(SM_CXSCREEN);
        windowHeight = GetSystemMetrics(SM_CYSCREEN);
    }

    hwnd = CreateWindowA("STATIC", title.c_str(), style,
                         CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
                         nullptr, nullptr, GetModuleHandle(nullptr), this);

    if (!hwnd) return false;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    return true;
}

void OW_Window::pollEvents()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void OW_Window::beginFrame()
{
    HDC hdc = GetDC(hwnd);
    RECT rect = {0, 0, windowWidth, windowHeight};
    HBRUSH brush = CreateSolidBrush(RGB(0,0,0));
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
    ReleaseDC(hwnd, hdc);
}

void OW_Window::endFrame()
{
    // nothing needed for GDI placeholder
}

void OW_Window::destroy()
{
    if (hwnd) DestroyWindow(hwnd);
}

bool OW_Window::isKeyPressed(int vk)
{
    return GetAsyncKeyState(vk) & 0x8000;
}

// placeholder drawImage using rectangle, you replace with real texture loading
void OW_Window::drawImage(const std::string& texturePath, float x, float y, float w, float h, float alpha)
{
    HDC hdc = GetDC(hwnd);
    RECT rect = {(LONG)x, (LONG)y, (LONG)(x+w), (LONG)(y+h)};
    HBRUSH brush = CreateSolidBrush(RGB(100,100,255)); // placeholder color
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
    ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK OW_Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    OW_Window* window = reinterpret_cast<OW_Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (uMsg)
    {
        case WM_CREATE:
        {
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
