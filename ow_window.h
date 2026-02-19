#pragma once
#include <windows.h>
#include <string>
#include <unordered_map>

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

    bool isKeyPressed(int vk);
    void drawImage(const std::string& path, float x, float y, float width, float height);

private:
    HWND hwnd = nullptr;
    bool running = false;

    HDC backBufferDC = nullptr;
    HBITMAP backBufferBitmap = nullptr;
    HBITMAP oldBitmap = nullptr;

    std::unordered_map<int, bool> keyStates;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
