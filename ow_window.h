#pragma once
#include <windows.h>
#include <string>
#include <unordered_map>

class OW_Window
{
public:
    OW_Window();
    ~OW_Window();

    bool create(int width, int height, const std::string& title, bool fullscreen = false);
    void pollEvents();
    void beginFrame();
    void endFrame();
    void destroy();

    bool isKeyPressed(int vk);

    // dynamic resolution
    int getWidth()  const { return windowWidth; }
    int getHeight() const { return windowHeight; }

    void drawImage(const std::string& texturePath, float x, float y, float w, float h, float alpha);

private
