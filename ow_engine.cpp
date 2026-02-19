#include "ow_engine.h"
#include <chrono>
#include <iostream>
#include <mmsystem.h>
#include <cmath>
#pragma comment(lib, "winmm.lib")

bool OW_Engine::init()
{
    if (!window.create(1280, 720, "Ocean's Funky World"))
        return false;

    // MAIN MENU
    mainMenuItems = {
        {"Story Mode", true, true, "story_normal.png", "story_highlight.png"},
        {"Freeplay", false, true, "freeplay_normal.png", "freeplay_highlight.png"},
        {"Settings", true, false, "settings_normal.png", "settings_highlight.png"},
        {"Exit", true, false, "exit_normal.png", "exit_highlight.png"}
    };

    // SETTINGS MENU
    settingsMenuItems = {
        {"Volume: 3", true, false, "volume_normal.png", "volume_highlight.png"},
        {"Downscroll: OFF", true, true, "downscroll_normal.png", "downscroll_highlight.png"},
        {"Keybinds [D F J K]", true, true, "keybinds_normal.png", "keybinds_highlight.png"}
    };

    selectedIndex = 0;
    currentMenu = MenuState::MAIN;
    menuMusicPath = "menu_music.mp3";

    // play menu music loop
    std::string cmd = "open \"" + menuMusicPath + "\" type mpegvideo alias menuMusic";
    mciSendString(cmd.c_str(), nullptr, 0, nullptr);
    mciSendString("play menuMusic repeat", nullptr, 0, nullptr);

    running = true;
    return true;
}

void OW_Engine::run()
{
    using clock = std::chrono::high_resolution_clock;
    auto lastTime = clock::now();

    while (running)
    {
        window.pollEvents();

        auto currentTime = clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        processInput();
        update(deltaTime);
        render();
    }
}

void OW_Engine::shutdown()
{
    mciSendString("stop menuMusic", nullptr, 0, nullptr);
    mciSendString("close menuMusic", nullptr, 0, nullptr);

    window.destroy();
    std::cout << "Engine shutdown complete.\n";
}

void OW_Engine::processInput()
{
    static bool upPressedLast = false;
    static bool downPressedLast = false;
    static bool enterPressedLast = false;

    auto& items = (currentMenu == MenuState::MAIN) ? mainMenuItems : settingsMenuItems;

    // UP
    if (window.isKeyPressed(VK_UP))
    {
        if (!upPressedLast)
        {
            selectedIndex--;
            if (selectedIndex < 0) selectedIndex = items.size() - 1;
        }
        upPressedLast = true;
    }
    else upPressedLast = false;

    // DOWN
    if (window.isKeyPressed(VK_DOWN))
    {
        if (!downPressedLast)
        {
            selectedIndex++;
            if (selectedIndex >= items.size()) selectedIndex = 0;
        }
        downPressedLast = true;
    }
    else downPressedLast = false;

    // ENTER
    if (window.isKeyPressed(VK_RETURN))
    {
        if (!enterPressedLast)
        {
            MenuItem& selected = items[selectedIndex];

            if (currentMenu == MenuState::MAIN)
            {
                if (selected.name == "Exit") running = false;
                else if (selected.name == "Settings")
                {
                    currentMenu = MenuState::SETTINGS;
                    selectedIndex = 0;
                }
            }
            else if (currentMenu == MenuState::SETTINGS)
            {
                if (selected.name.find("Volume") != std::string::npos)
                {
                    volume++;
                    if (volume > 10) volume = 10;
                    selected.name = "Volume: " + std::to_string(volume);
                }
            }
        }
        enterPressedLast = true;
    }
    else enterPressedLast = false;

    // ESC
    if (window.isKeyPressed(VK_ESCAPE))
    {
        if (currentMenu == MenuState::SETTINGS)
        {
            currentMenu = MenuState::MAIN;
            selectedIndex = 0;
        }
        else
        {
            running = false;
        }
    }
}

// update handles easing scaling for buttons
void OW_Engine::update(float deltaTime)
{
    auto& items = (currentMenu == MenuState::MAIN) ? mainMenuItems : settingsMenuItems;

    // set target scale based on selection
    for (int i = 0; i < items.size(); i++)
        items[i].targetScale = (i == selectedIndex) ? 1.0145f : 1.0f;

    // easing out tween
    float speed = 8.0f; // tweak for faster/slower easing
    for (auto& item : items)
    {
        float diff = item.targetScale - item.currentScale;
        item.currentScale += diff * (1 - expf(-speed * deltaTime));
    }
}

void OW_Engine::render()
{
    window.beginFrame();

    if (currentMenu == MenuState::MAIN)
        renderMenu();
    else if (currentMenu == MenuState::SETTINGS)
        renderSettings();

    window.endFrame();
}

void OW_Engine::renderMenu()
{
    int startY = 150;
    int spacing = 80;

    for (int i = 0; i < mainMenuItems.size(); i++)
    {
        MenuItem& item = mainMenuItems[i];

        std::string tex = (i == selectedIndex) ? item.texHighlight : item.texNormal;
        float scale = item.currentScale;

        float w = 480 * scale;
        float h = 60 * scale;
        float x = 400 - (w - 480)/2;
        float y = startY + i * spacing - (h - 60)/2;

        window.drawImage(tex, x, y, w, h);
    }
}

void OW_Engine::renderSettings()
{
    int startY = 150;
    int spacing = 80;

    for (int i = 0; i < settingsMenuItems.size(); i++)
    {
        MenuItem& item = settingsMenuItems[i];

        std::string tex = (i == selectedIndex) ? item.texHighlight : item.texNormal;
        float scale = item.currentScale;

        float w = 480 * scale;
        float h = 60 * scale;
        float x = 400 - (w - 480)/2;
        float y = startY + i * spacing - (h - 60)/2;

        window.drawImage(tex, x, y, w, h);
    }
}
