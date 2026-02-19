#include "ow_window.h"
#include <iostream>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

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
        this
    );

    if (!hwnd)
    {
        std::cout << "Failed to create window!\n";
        return false;
    }

    ShowWindow(hwnd, SW_SHOW);

    // backbuffer
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
    RECT rect;
    GetClientRect(hwnd, &rect);
    HBRUSH brush = CreateSolidBrush(RGB(30, 30, 30));
    FillRect(backBufferDC, &rect, brush);
    DeleteObject(brush);
}

void OW_Window::endFrame()
{
    HDC windowDC = GetDC(hwnd);
    RECT rect;
    GetClientRect(hwnd, &rect);
    BitBlt(windowDC, 0, 0, rect.right, rect.bottom, backBufferDC, 0, 0, SRCCOPY);
    ReleaseDC(hwnd, windowDC);
}

bool OW_Window::isKeyPressed(int vk)
{
    auto it = keyStates.find(vk);
    return it != keyStates.end() && it->second;
}

void OW_Window::drawImage(const std::string& path, float x, float y, float width, float height, float alpha)
{
    static bool gdiplusStarted = false;
    static ULONG_PTR gdiplusToken;

    if (!gdiplusStarted)
    {
        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
        gdiplusStarted = true;
    }

    Graphics graphics(backBufferDC);
    graphics.SetCompositingMode(CompositingModeSourceOver);

    std::wstring wpath(path.begin(), path.end());
    Image image(wpath.c_str());

    ColorMatrix cm;
    memset(&cm, 0, sizeof(ColorMatrix));
    cm.m[0][0] = 1.0f;
    cm.m[1][1] = 1.0f;
    cm.m[2][2] = 1.0f;
    cm.m[3][3] = alpha; // alpha for fade
    cm.m[4][4] = 1.0f;

    ImageAttributes ia;
    ia.SetColorMatrix(&cm, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

    graphics.DrawImage(&image, RectF(x, y, width, height), 0, 0, image.GetWidth(), image.GetHeight(), UnitPixel, &ia);
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
        if (window) window->keyStates[wParam] = true;
        break;
    case WM_KEYUP:
        if (window) window->keyStates[wParam] = false;
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
