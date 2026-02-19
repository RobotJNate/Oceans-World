#include "ow_engine.h"
#include <chrono>
#include <iostream>
#include <cmath>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

bool OW_Engine::init()
{
    if (!window.create(1280, 720, "Ocean's Funky World", true)) // fullscreen = true
        return false;

    // main menu
    mainMenuItems = {
        {"Story Mode", true, true, "story_normal.png", "story_highlight.png"},
        {"Freeplay", false, true, "freeplay_normal.png", "freeplay_highlight.png"},
        {"Settings", true, false, "settings_normal.png", "settings_highlight.png"},
        {"Exit", true, false, "exit_normal.png", "exit_highlight.png"}
    };

    // settings menu
    settingsMenuItems = {
        {"Volume: 3", true, false, "volume_normal.png", "volume_highlight.png"},
        {"Downscroll: OFF", true, true, "downscroll_normal.png", "downscroll_highlight.png"},
        {"Keybinds [D F J K]", true, true, "keybinds_normal.png", "keybinds_highlight.png"}
    };

    // back button
    backButton = {"Back", true, false, "back_normal.png", "back_highlight.png"};

    selectedIndex = 0;
    settingsBackSelected = false;
    currentMenu = MenuState::MAIN;

    // menu music
    menuMusicPath = "menu_music.mp3";
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
}

void OW_Engine::startFade(MenuState toMenu)
{
    fading = true;
    targetAlpha = 0.0f;
    nextMenu = toMenu;
}

void OW_Engine::processInput()
{
    static bool upPressedLast = false;
    static bool downPressedLast = false;
    static bool enterPressedLast = false;

    auto& items = (currentMenu == MenuState::MAIN) ? mainMenuItems : settingsMenuItems;
    int maxIndex = (currentMenu == MenuState::SETTINGS) ? settingsMenuItems.size() + 1 : items.size();

    // UP
    if (window.isKeyPressed(VK_UP))
    {
        if (!upPressedLast)
        {
            selectedIndex--;
            if (selectedIndex < 0) selectedIndex = maxIndex - 1;
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
            if (selectedIndex >= maxIndex) selectedIndex = 0;
        }
        downPressedLast = true;
    }
    else downPressedLast = false;

    // ENTER
    if (window.isKeyPressed(VK_RETURN))
    {
        if (!enterPressedLast)
        {
            if (currentMenu == MenuState::MAIN)
            {
                MenuItem& selected = items[selectedIndex];
                if (selected.name == "Exit") running = false;
                else if (selected.name == "Settings") startFade(MenuState::SETTINGS);
            }
            else if (currentMenu == MenuState::SETTINGS)
            {
                if (selectedIndex < settingsMenuItems.size())
                {
                    MenuItem& selected = settingsMenuItems[selectedIndex];
                    if (selected.name.find("Volume") != std::string::npos)
                    {
                        volume++;
                        if (volume > 10) volume = 10;
                        selected.name = "Volume: " + std::to_string(volume);
                    }
                }
                else // Back button
                {
                    startFade(MenuState::MAIN);
                }
            }
        }
        enterPressedLast = true;
    }
    else enterPressedLast = false;

    // ESC
    if (window.isKeyPressed(VK_ESCAPE))
    {
        if (currentMenu == MenuState::SETTINGS) startFade(MenuState::MAIN);
        else running = false;
    }

    // back button selection
    if (currentMenu == MenuState::SETTINGS && selectedIndex == settingsMenuItems.size())
        settingsBackSelected = true;
    else
        settingsBackSelected = false;
}

void OW_Engine::update(float deltaTime)
{
    auto& items = (currentMenu == MenuState::MAIN) ? mainMenuItems : settingsMenuItems;

    float speed = 8.0f;
    for (int i = 0; i < items.size(); i++)
        items[i].targetScale = (i == selectedIndex) ? 1.0145f : 1.0f;

    for (auto& item : items)
    {
        float diff = item.targetScale - item.currentScale;
        item.currentScale += diff * (1 - expf(-speed * deltaTime));
    }

    backButton.targetScale = settingsBackSelected ? 1.0145f : 1.0f;
    float diff = backButton.targetScale - backButton.currentScale;
    backButton.currentScale += diff * (1 - expf(-speed * deltaTime));

    if (fading)
    {
        float diffAlpha = targetAlpha - menuAlpha;
        menuAlpha += diffAlpha * (1 - expf(-fadeSpeed * deltaTime));

        if (menuAlpha <= 0.01f)
        {
            currentMenu = nextMenu;
            targetAlpha = 1.0f;
        }

        if (menuAlpha >= 0.99f && targetAlpha == 1.0f)
            fading = false;
    }
}

void OW_Engine::render()
{
    window.beginFrame();

    if (currentMenu == MenuState::MAIN) renderMenu();
    else if (currentMenu == MenuState::SETTINGS) renderSettings();

    window.endFrame();
}

void OW_Engine::renderMenu()
{
    float btnW = window.getWidth()  * 0.375f;
    float btnH = window.getHeight() * 0.083f;
    float startY = window.getHeight() * 0.208f;
    f
