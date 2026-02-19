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
        window.pollEvents(); // handle messages + update keyStates

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
    // placeholder for engine input handling
}

void OW_Engine::update(float deltaTime)
{
    // move rectangle with arrow keys
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
    window.drawRect(rectX, rectY, rectWidth, rectHeight, 255, 0, 0);

    window.endFrame();
}
