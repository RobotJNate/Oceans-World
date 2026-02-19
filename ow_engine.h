#pragma once
#include "ow_window.h"
#include <vector>
#include <string>

enum class MenuState
{
    MAIN,
    SETTINGS
};

struct MenuItem
{
    std::string name;
    bool enabled = true;        // for Freeplay or disabled buttons
    bool isPlaceholder = false; // * buttons
    std::string texNormal;      // normal image
    std::string texHighlight;   // highlighted image
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

    bool running = false;
    OW_Window window;

    // menu
    MenuState currentMenu = MenuState::MAIN;
    std::vector<MenuItem> mainMenuItems;
    std::vector<MenuItem> settingsMenuItems;
    int selectedIndex = 0;

    // Settings
    int volume = 3;         // 1-10
    bool downscroll = false;

    // menu music
    std::string menuMusicPath;
};
