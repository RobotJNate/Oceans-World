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

    bool isKeyPressed(int vk); // check key state
    void drawRect(float x, float y, float width, float height, int r, int g, int b);

private:
    HWND hwnd = nullptr;
    bool running = false;
    std::unordered_map<int, bool> keyStates; // VK key -> pressed

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
