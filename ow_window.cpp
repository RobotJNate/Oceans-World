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

    // pass 'this' pointer for WindowProc access
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
        this
    );

    if (!hwnd)
    {
        std::cout << "Failed to create window!\n";
        return false;
    }

    ShowWindow(hwnd, SW_SHOW);

    // create backbuffer
    HDC windowDC = GetDC(hwnd);
    backBufferDC = CreateCompatibleDC(windowDC);
    backBufferBitmap = CreateCompatibleBitmap(windowDC, width, height);
    oldBitmap = (HBITMAP)SelectObject(backBufferDC, backBufferBitmap);
    ReleaseDC(hwnd, windowDC);
    
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
    HDC hdc = GetDC(hwnd);
    RECT rect;
    GetClientRect(hwnd, &rect);
    HBRUSH brush = CreateSolidBrush(RGB(30, 30, 30)); // background color
    FillRect(hdc, &rect, brush);
    FillRect(backBufferDC, &rect, brush); // clear the backbuffer
    DeleteObject(brush);
    ReleaseDC(hwnd, hdc);
}

void OW_Window::endFrame()
{
    HDC windowDC = GetDC(hwnd);
    BitBlt(windowDC, 0, 0, rectWidth, rectHeight, backBufferDC, 0, 0, SRCCOPY);
    ReleaseDC(hwnd, windowDC);
}

bool OW_Window::isKeyPressed(int vk)
{
    auto it = keyStates.find(vk);
    return it != keyStates.end() && it->second;
}

void OW_Window::drawRect(float x, float y, float width, float height, int r, int g, int b)
{
    RECT rect;
    rect.left = static_cast<LONG>(x);
    rect.top = static_cast<LONG>(y);
    rect.right = static_cast<LONG>(x + width);
    rect.bottom = static_cast<LONG>(y + height);

    HBRUSH brush = CreateSolidBrush(RGB(r, g, b));
    FillRect(backBufferDC, &rect, brush); // draw to backbuffer
    DeleteObject(brush);
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
    case WM_KEYDOWN:
        if (window)
            window->keyStates[wParam] = true;
        break;
    case WM_KEYUP:
        if (window)
            window->keyStates[wParam] = false;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void OW_Window::destroy()
{
    if (backBufferDC)
    {
        SelectObject(backBufferDC, oldBitmap);
        DeleteObject(backBufferBitmap);
        DeleteDC(backBufferDC);
        backBufferDC = nullptr;
        backBufferBitmap = nullptr;
        oldBitmap = nullptr;
    }

    if (hwnd)
    {
        ::DestroyWindow(hwnd);
        hwnd = nullptr;
    }
}

