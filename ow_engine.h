#pragma once
#include "ow_window.h"
#include <string>
#include <vector>
#include "ow_lua.h"
OW_Lua lua;

enum class MenuState
{
    MAIN,
    SETTINGS
};

struct MenuItem
{
    std::string name;
    bool enabled = true;
    bool isPlaceholder = false;
    std::string texNormal;
    std::string texHighlight;

    float currentScale = 1.0f;
    float targetScale = 1.0f;
};

class OW_Engine
{
public:
    bool init();
    void run();
    void shutdown();

private:
    void processInput();
    void update(float deltaTime);
    void render();
    void renderMenu();
    void renderSettings();
    void startFade(MenuState toMenu);

    bool running = false;
    OW_Window window;

    std::vector<MenuItem> mainMenuItems;
    std::vector<MenuItem> settingsMenuItems;
    MenuItem backButton;

    int selectedIndex = 0;
    bool settingsBackSelected = false;

    MenuState currentMenu;

    // menu fade
    float menuAlpha = 1.0f;
    float targetAlpha = 1.0f;
    bool fading = false;
    MenuState nextMenu;
    float fadeSpeed = 3.0f;

    // example volume setting
    int volume = 3;

    // menu music
    std::string menuMusicPath;
};
