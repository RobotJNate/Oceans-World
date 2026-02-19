#pragma once
#include <windows.h>
#include <string>

class OW_Window
{
public:
    OW_Window();
    ~OW_Window();

    bool create(int width, int height, const std::string& title);
    void pollEvents();
    void beginFrame();
    void endFrame();
    void destroy();

private:
    HWND hwnd = nullptr;
    bool running = false;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
