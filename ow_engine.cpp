#include "ow_engine.h"
#include <chrono>
#include <iostream>

bool OW_Engine::init()
{
    if (!window.create(1280, 720, "Ocean's Funky World"))
    {
        std::cout << "Failed to create window!\n";
        return false;
    }

    running = true;
    return true;
}

void OW_Engine::run()
{
    using clock = std::chrono::high_resolution_clock;
    auto lastTime = clock::now();

    while (running)
    {
        // handle windows messages
        window.pollEvents();

        // delta time
        auto currentTime = clock::now();
        float deltaTime =
            std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        processInput();
        update(deltaTime);
        render();
    }
}

void OW_Engine::shutdown()
{
    window.destroy();
    std::cout << "Engine shutdown complete.\n";
}

void OW_Engine::processInput()
{
    // input handling, key presses, etc
}

void OW_Engine::update(float deltaTime)
{
    // move a rectangle with arrow keys
    if (window.isKeyPressed(VK_LEFT))
        rectX -= 200.0f * deltaTime;
    if (window.isKeyPressed(VK_RIGHT))
        rectX += 200.0f * deltaTime;
    if (window.isKeyPressed(VK_UP))
        rectY -= 200.0f * deltaTime;
    if (window.isKeyPressed(VK_DOWN))
        rectY += 200.0f * deltaTime;

    // close window on ESC
    if (window.isKeyPressed(VK_ESCAPE))
        running = false;
}

void OW_Engine::render()
{
    window.beginFrame();

    // draw the rectangle
    window.drawRect(rectX, rectY, rectWidth, rectHeight, 255, 0, 0); // red rect

    window.endFrame();
}

