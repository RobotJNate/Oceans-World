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
    bool enabled = true;
    bool isPlaceholder = false;
    std::string texNormal;
    std::string texHighlight;

    // smooth scaling
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

    bool running = false;
    OW_Window window;

    float menuAlpha = 1.0f;       // current alpha for fade
    float targetAlpha = 1.0f;     // target alpha for fade
    bool fading = false;           // whether a fade is happening
    MenuState nextMenu;            // menu to switch to after fade
    float fadeSpeed = 3.0f;        // speed of fade

    void startFade(MenuState toMenu); // triggers fade to another menu

    MenuItem backButton; // Back button for settings menu

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
